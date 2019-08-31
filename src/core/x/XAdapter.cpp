//
// Created by liyh on 2019/8/23.
//

#include "../../stdafx.h"
#include "XAdapter.h"
#include "../Root.h"

namespace XAdapter {

	static const auto BILLION = 1000000000;
	static const auto MILLION = 1000000;
	static const auto THOUSAND = 1000;

	struct APPLICATION {
		Display *display;
		Window windowRoot;
		int defaultScreen;
		GLXFBConfig *fbConfig;
		XVisualInfo *visualInfo;
		GLXContext glContext;
		Colormap colormap;
		long eventMask;

		int argc;
		char **argv;
		int fps;
		WindowInvalidateHandle onWindowInvalidate;
		WindowResizeHandle onWindowResize;
	} app;

	void Initialize(const InitParams *params) {
		if (app.display) { return; }

		app.argc = params->argc;
		app.argv = params->argv;
		app.fps = params->fps;
		app.onWindowInvalidate = params->onWindowInvalidate;
		app.onWindowResize = params->onWindowResize;

		// keng: 不能监听ResizeRedirectMask事件，因为不知道如何写处理函数，它会导致窗口重回区域始终是初始大小
		// 		 这里改为监听StructureNotifyMask事件，当窗口大小变化时也会触发它
		app.eventMask = ExposureMask | KeyPressMask | StructureNotifyMask;        // event mask
		// StructureNotifyMask:
		//		CirculateNotify	XCirculateEvent
		//		ConfigureNotify	XConfigureEvent
		//		DestroyNotify	XDestroyWindowEvent
		//		GravityNotify	XGravityEvent
		//		MapNotify	XMapEvent
		//		ReparentNotify	XReparentEvent
		//		UnmapNotify	XUnmapEvent

		// get display name
		auto displayName = getenv("DISPLAY");
		assert(displayName);

		// connect to x server
		app.display = XOpenDisplay(displayName);
		assert(app.display);

		// get default screen
		app.defaultScreen = DefaultScreen(app.display);

		// get frame buffer configurations
		int nelements, att[] = {GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_DOUBLEBUFFER, True, GLX_DEPTH_SIZE, 16, None};
		app.fbConfig = glXChooseFBConfig(app.display, app.defaultScreen, att, &nelements);
		assert(app.fbConfig);

		// get visual info
		app.visualInfo = glXGetVisualFromFBConfig(app.display, *app.fbConfig);
		assert(app.visualInfo);

		// get root window
		app.windowRoot = RootWindow(app.display, app.defaultScreen);

		// create colormap for create window
		app.colormap = XCreateColormap(app.display, app.windowRoot, app.visualInfo->visual, AllocNone);

		// create gl context
		app.glContext = glXCreateNewContext(app.display, *app.fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);
		assert(app.glContext);

		// init glew use the root window
		auto result = glXMakeCurrent(app.display, app.windowRoot, app.glContext);
		assert(result);
		GLenum err = glewInit();
		assert(GLEW_OK == err);
		// TODO: 这里没有重置 current opengl context, 是为了保持 opengl context 有效, 能创建资源,
		//  	 但这里使用的是RootWindow, 不知道有没有什么影响
//		result = glXMakeCurrent(app.display, None, nullptr);
//		assert(result);
	}

	void Uninitialize() {
		if (!app.display) { return; }

		if (glXGetCurrentContext() == app.glContext) {
			auto result = glXMakeCurrent(app.display, None, 0);
			assert(result);
		}
		glXDestroyContext(app.display, app.glContext);
		XFreeColormap(app.display, app.colormap);
		XFree(app.fbConfig);
		XFree(app.visualInfo);
		XCloseDisplay(app.display);
		memset(&app, 0, sizeof(APPLICATION));
	}

	static bool processEvent(XEvent event) {
		switch (event.type) {
			case Expose:
				app.onWindowInvalidate((WINDOW_HANDLE) event.xexpose.window);
				break;
			case ConfigureNotify:
				app.onWindowResize((WINDOW_HANDLE) event.xconfigure.window, event.xconfigure.width,
								   event.xconfigure.height);
				break;
			case KeyPress:
				auto keySys = XkbKeycodeToKeysym(app.display, event.xkey.keycode, 0,
												 (event.xkey.state & ShiftMask) ? 1 : 0);
				if (keySys == XK_Escape) {
					return true;
				}
				break;
		}
		return false;
	}

	static int runMainLoopByEventDrive() {
		assert(app.fps);
		const long int oneFrameMonoLimit = BILLION / app.fps;

		// save start time
		Timer lastRenderTime = Timer::Now(), now;
		long int timeSpan = 0;
		XEvent event;
		for (;;) {
			// render
			Root::GetInstance()->RenderOneFrame(timeSpan, oneFrameMonoLimit);

			// process all event in queue
			XNextEvent(app.display, &event);            // block the thread
			if (processEvent(event) || DoEvent()) { break; }

			// calc time span from last render
			now = Timer::Now();
			timeSpan = (now - lastRenderTime).ToNano();
			lastRenderTime = now;

			// have a rest if time span less then the limit
			if (oneFrameMonoLimit > timeSpan) {
				auto usPadding = (oneFrameMonoLimit - timeSpan) / THOUSAND;
				if (usPadding) { usleep(usPadding); }
				timeSpan = oneFrameMonoLimit;        // TODO: sleep 真正的时间可能比 usPadding 要长
			}
		}
		return 0;
	}

	static int runMainLoopOnRealtime() {
		const long int oneFrameMonoLimit = BILLION / app.fps;

		// setting start time, and render the first frame
		Timer lastRenderTime = Timer::Now(), now ;
		long int timeSpan = 0;
		XEvent event;

		for (;;) {
			if (XCheckMaskEvent(app.display, app.eventMask, &event)) {
				if (processEvent(event)) { break; }
			} else {
				now = Timer::Now();
				timeSpan = (now - lastRenderTime).ToNano();
				Root::GetInstance()->RenderOneFrame(timeSpan, oneFrameMonoLimit);
				lastRenderTime = now;
				pthread_yield();
			}
		}
		return 0;
	}

	int RunMainLoop() {
		if (app.fps) {
			return runMainLoopByEventDrive();
		} else {
			return runMainLoopOnRealtime();
		}
	}

	WINDOW_HANDLE CreateRenderWindow() {

		XSetWindowAttributes swa;
		swa.colormap = app.colormap;
		swa.event_mask = app.eventMask;
		unsigned long valueMask = CWColormap | CWEventMask;
		auto window = XCreateWindow(app.display, app.windowRoot, 100, 100, 320, 240, 0,
									app.visualInfo->depth, InputOutput, app.visualInfo->visual,
									valueMask, &swa);
		return (WINDOW_HANDLE) window;
	}

	void DestroyRenderWindow(WINDOW_HANDLE hWin) {
		auto win = (Window) hWin;
		if (glXGetCurrentDrawable() == win) {
			auto result = glXMakeCurrent(app.display, None, nullptr);
			assert(result);
		}
		XDestroyWindow(app.display, win);
	}

	void WindowShow(WINDOW_HANDLE hWin) {
		auto win = (Window) hWin;
		XMapWindow(app.display, win);
		XStoreName(app.display, win, "show me");
		XFlush(app.display);   // force to show out
	}

	void WindowSwapBuffer(WINDOW_HANDLE hWin) {
		auto win = (Window) hWin;
		glXSwapBuffers(app.display, win);
	}

	void WindowMakeCurrent(WINDOW_HANDLE hWin) {
		auto win = (Window) hWin;
		auto result = glXMakeCurrent(app.display, win, app.glContext);
		assert(result);
	}

	void GetWindowGeometry(WINDOW_HANDLE hWin, int &x, int &y, unsigned int &w, unsigned int &h) {
		Window root;
		unsigned int border, depth;
		auto result = XGetGeometry(app.display, (Window) hWin, &root, &x, &y, &w, &h, &border, &depth);
		assert(result);
	}

	bool HasEventInQueue() {
		// 返回事件队列中的事件数, QueuedAlready: 表示只监视已如列的事件, 不会再尝试刷新动作
		auto eventCount = XEventsQueued(app.display, QueuedAlready);
		return eventCount > 0;
	}

	bool DoEvent() {
		XEvent event;
		for (;;) {
			if (XCheckMaskEvent(app.display, app.eventMask, &event)) {
				if (processEvent(event)) { return true; }
			} else { return false; }
		}
	}
}
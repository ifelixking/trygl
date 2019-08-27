//
// Created by liyh on 2019/8/23.
//

#include "../../stdafx.h"
#include "XAdapter.h"
#include "../Root.h"

namespace XAdapter {

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
	} app;



	void Initialize(const InitParams *params) {
		if (app.display) { return; }

		app.argc = params->argc;
		app.argv = params->argv;
		app.fps = params->fps;
		app.onWindowInvalidate = params->onWindowInvalidate;

		app.eventMask = ExposureMask | KeyPressMask;        // event mask

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
		result = glXMakeCurrent(app.display, None, nullptr);
		assert(result);
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

	bool processEvent(XEvent event) {
		switch (event.type) {
			case Expose:
				app.onWindowInvalidate((WINDOW_HANDLE)event.xexpose.window);
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

	int RunMainLoop() {
		const auto BILLION = 1000000000;
		const auto MILLION = 1000000;
		const auto THOUSAND = 1000;
		const long int oneFrameMonoLimit = BILLION / app.fps;
#define CHECK_EVENT_SLEEP_TIME 30000

		// setting start time, and render the first frame
		timespec lastRenderTime = {}, now = {};
		auto iResult = clock_gettime(CLOCK_MONOTONIC, &lastRenderTime);
		assert(iResult == 0);
		Root::GetInstance()->RenderOneFrame(0);

		for (;;) {
			XEvent event;
			XNextEvent(app.display, &event);            // block the thread

			L_PROCESS_EVENT:
			if (processEvent(event)) { break; }

			iResult = clock_gettime(CLOCK_MONOTONIC, &now);
			assert(iResult == 0);
			auto timeSpan = (now.tv_sec - lastRenderTime.tv_sec) * 1000000000 + now.tv_nsec - lastRenderTime.tv_nsec;

			// render if time up
			if (timeSpan >= oneFrameMonoLimit) {
				Root::GetInstance()->RenderOneFrame(timeSpan);
				lastRenderTime = now;
			} else {
				// loop: check event or do render if time out
				for (;;) {
					// if has event in queue then go to process them
					if (XCheckMaskEvent(app.display, app.eventMask, &event)) {
						goto L_PROCESS_EVENT;
					} else {
						// if no event in queue, check time out then render or wait
						iResult = clock_gettime(CLOCK_MONOTONIC, &now);
						assert(iResult == 0);
						timeSpan = (now.tv_sec - lastRenderTime.tv_sec) * 1000000000 + now.tv_nsec -
								   lastRenderTime.tv_nsec;
						if (timeSpan > oneFrameMonoLimit) {
							Root::GetInstance()->RenderOneFrame(timeSpan);
							lastRenderTime = now;
							break;
						}

						// no event and not time out, then wait
#if (CHECK_EVENT_SLEEP_TIME == 0)
						pthread_yield();
#else
						usleep(CHECK_EVENT_SLEEP_TIME);
#endif
					}
				}
			}
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
		// XMapWindow(app.display, window);
		// XStoreName(app.display, window, "show me");
		return (WINDOW_HANDLE)window;
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

}
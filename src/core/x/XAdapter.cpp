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
	} app;

	struct WINDOW {
		Window window;
		Colormap colormap;
	};

	void initGL() {
		// if (app.glContext) { return; }

		// init opengl context
//		app.glContext = glXCreateNewContext(app.display, *app.fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);
//		assert(app.glContext);

		// init glew
//		// create a temp window for init glew
		auto windowTmp = (WINDOW *) CreateRenderWindow();
		XMapWindow(app.display, windowTmp->window);
		auto result = glXMakeCurrent(app.display, windowTmp->window, app.glContext); assert(result);
		GLenum err = glewInit();
		assert(GLEW_OK == err);
		// result = glXMakeCurrent(app.display, None, 0); assert(result);
		// DestroyRenderWindow(windowTmp);
	}

	void uninitGL() {
		if (!app.display) { return; }
		if (glXGetCurrentContext() == app.glContext) {
			auto result = glXMakeCurrent(app.display, None, 0);
			assert(result);
		}
		glXDestroyContext(app.display, app.glContext);
		XFree(app.fbConfig);
		XFree(app.visualInfo);
		app.glContext = 0;
	}

	void Initialize(int argc, char **argv) {
		if (app.display) { return; }

		// connect to x server
		auto displayName = getenv("DISPLAY");
		assert(displayName);
		app.display = XOpenDisplay(displayName);
		assert(app.display);
		app.defaultScreen = DefaultScreen(app.display);
		app.windowRoot = RootWindow(app.display, app.defaultScreen);
		int att[] = {
				GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_DOUBLEBUFFER, True,
				GLX_DEPTH_SIZE, 24,
				None};
		int nelements;
		app.fbConfig = glXChooseFBConfig(app.display, app.defaultScreen, att, &nelements);
		assert(app.fbConfig);
		app.visualInfo = glXGetVisualFromFBConfig(app.display, *app.fbConfig);
		assert(app.visualInfo);

		app.glContext = glXCreateNewContext(app.display, *app.fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);
		assert(app.glContext);

		initGL();
	}

	void Uninitialize() {
		uninitGL();
		XCloseDisplay(app.display);
		memset(&app, sizeof(APPLICATION), 0);
	}

	int RunMainLoop() {
		for (;;) {
			XEvent event;
			// XNextEvent(app.display, &event);
			if (XCheckMaskEvent(app.display, ExposureMask | KeyPressMask, &event)) {
				switch (event.type) {
					// paint event
					case Expose:
						// render
						// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						// swap buffer
						// glXSwapBuffers(app.display, app.);

						break;
					case KeyPress:
						auto keySys = XkbKeycodeToKeysym(app.display, event.xkey.keycode, 0,
														 event.xkey.state & ShiftMask ? 1 : 0);
						if (keySys == XK_Escape) { goto L_EXIT; }

						// Root::GetInstance()->SetInvalidate();

						break;
				}
			} else {
				if (Root::GetInstance()->IsInvalidate()) {
					Root::GetInstance()->RenderOneFrame();
				} else {
					pthread_yield();
				}
			}
		}
		L_EXIT:
		return 0;
	}

	WINDOW_HANDLE CreateRenderWindow() {
		auto colorMap = XCreateColormap(app.display, app.windowRoot, app.visualInfo->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = colorMap;
		swa.event_mask = ExposureMask | KeyPressMask;
		unsigned long valueMask = CWColormap | CWEventMask;
		auto window = XCreateWindow(app.display, app.windowRoot, 100, 100, 320, 240, 0,
									app.visualInfo->depth, InputOutput, app.visualInfo->visual,
									valueMask, &swa);
		XStoreName(app.display, window, "show me");
		auto win = (WINDOW *) malloc(sizeof(WINDOW));
		win->colormap = colorMap;
		win->window = window;
		return win;
	}

	void DestroyRenderWindow(WINDOW_HANDLE hWin) {
		auto win = (WINDOW *) hWin;
		if (glXGetCurrentDrawable() == win->window) {
			auto result = glXMakeCurrent(app.display, None, 0);
			assert(result);
		}
		XFreeColormap(app.display, win->colormap);
		XDestroyWindow(app.display, win->window);
	}

	void WindowShow(WINDOW_HANDLE hWin) {
		auto win = (WINDOW *) hWin;
		XMapWindow(app.display, win->window);
		XStoreName(app.display, win->window, "show me");
		XFlush(app.display);   // force to show out

		// initGL();
	}

	void WindowSwapBuffer(WINDOW_HANDLE hWin) {
		auto win = (WINDOW *) hWin;
		glXSwapBuffers(app.display, win->window);
	}

	void WindowMakeCurrent(WINDOW_HANDLE hWin) {
		auto win = (WINDOW *) hWin;
		auto result = glXMakeCurrent(app.display, win->window, app.glContext);
		assert(result);
	}

}
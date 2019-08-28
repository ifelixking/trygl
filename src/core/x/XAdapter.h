//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_XADAPTER_H
#define TRYGL_XADAPTER_H

namespace XAdapter{

	typedef void (*WindowInvalidateHandle)(WINDOW_HANDLE win);
	typedef void (*WindowResizeHandle)(WINDOW_HANDLE win, unsigned int width, unsigned int height);
	struct InitParams{
		int argc;
		char ** argv;
		int fps;
		WindowInvalidateHandle onWindowInvalidate;
		WindowResizeHandle onWindowResize;
	};
    void Initialize(const InitParams * params);
    void Uninitialize();
    int RunMainLoop();

    WINDOW_HANDLE CreateRenderWindow();
    void DestroyRenderWindow(WINDOW_HANDLE hWin);
    void WindowShow(WINDOW_HANDLE hWin);
    void WindowSwapBuffer(WINDOW_HANDLE hWin);
    void WindowMakeCurrent(WINDOW_HANDLE hWin);
	void GetWindowGeometry(WINDOW_HANDLE hWin, int &x, int &y, unsigned int &w, unsigned int &h);
}

#endif //TRYGL_XADAPTER_H

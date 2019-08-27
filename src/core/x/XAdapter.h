//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_XADAPTER_H
#define TRYGL_XADAPTER_H

namespace XAdapter{

	struct InitParams{
		int argc;
		char ** argv;
		int fps;
	};
    void Initialize(const InitParams * params);
    void Uninitialize();
    int RunMainLoop();

    WINDOW_HANDLE CreateRenderWindow();
    void DestroyRenderWindow(WINDOW_HANDLE hWin);
    void WindowShow(WINDOW_HANDLE hWin);
    void WindowSwapBuffer(WINDOW_HANDLE hWin);
    void WindowMakeCurrent(WINDOW_HANDLE hWin);
}

#endif //TRYGL_XADAPTER_H

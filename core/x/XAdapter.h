//
// Created by liyh on 2019/8/23.
//

#ifndef TRYGL_XADAPTER_H
#define TRYGL_XADAPTER_H

namespace XAdapter{

    APPLICATION_HANDLE CreateApplication(int argc, char ** argv);
    void DestroyApplication(APPLICATION_HANDLE hApp);
    int ApplicationStart(APPLICATION_HANDLE hApp);

    WINDOW_HANDLE CreateRenderWindow(APPLICATION_HANDLE hApp);
    void DestroyRenderWindow(APPLICATION_HANDLE hApp, WINDOW_HANDLE hWin);
    void WindowShow(APPLICATION_HANDLE hApp, WINDOW_HANDLE hWin);
}

#endif //TRYGL_XADAPTER_H

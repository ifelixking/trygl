//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_QTADAPTER_H
#define TRYGL_QTADAPTER_H

namespace QtAdapter{

    APPLICATION_HANDLE CreateApplication(int argc, char ** argv);
    void DestroyApplication(APPLICATION_HANDLE hApp);
    int ApplicationStart(APPLICATION_HANDLE hApp);

    WINDOW_HANDLE CreateRenderWindow();
    void DestroyRenderWindow(WINDOW_HANDLE hWindow);
    void WindowShow(WINDOW_HANDLE);

}

#endif //TRYGL_QTADAPTER_H

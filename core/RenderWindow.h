//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_RENDERWINDOW_H
#define TRYGL_RENDERWINDOW_H


class RenderWindow {
private:
    friend class Root;
    RenderWindow();
    ~RenderWindow();

public:
    void Show();

private:
    WINDOW_HANDLE m_hWindow;
};


#endif //TRYGL_RENDERWINDOW_H

//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_RENDERWINDOW_H
#define TRYGL_RENDERWINDOW_H


#include "Viewport.h"

class RenderWindow {
private:
    friend class Root;
	RenderWindow();
    ~RenderWindow();

public:
    void Show();
    class Viewport * AddViewport();
    void RemoveViewport(Viewport * viewport);

    WINDOW_HANDLE GetHandle() const { return m_hWindow; }

    void Render() const;
    bool IsInvalidate() const;

private:
    WINDOW_HANDLE m_hWindow;
	std::vector<Viewport *> m_viewports;
};


#endif //TRYGL_RENDERWINDOW_H

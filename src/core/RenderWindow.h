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
    const Viewport * GetViewport(unsigned long index) const { return m_viewports[index]; }
	Viewport * GetViewport(unsigned long index) { return m_viewports[index]; }

    WINDOW_HANDLE GetHandle() const { return m_hWindow; }

    void Render() const;
    bool IsInvalidate() const;
    void SetInvalidate() { m_windowInvalidate = true; }
	Sizeui GetSize() const { return m_size; }

private:
	void onResize(unsigned int width, unsigned int height);

private:
    WINDOW_HANDLE m_hWindow;
	Sizeui m_size;

	std::vector<Viewport *> m_viewports;
	mutable bool m_windowInvalidate;
};


#endif //TRYGL_RENDERWINDOW_H

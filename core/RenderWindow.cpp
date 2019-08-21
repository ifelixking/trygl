//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "RenderWindow.h"
#include "qt/QtAdapter.h"

RenderWindow::RenderWindow(){
    m_hWindow = QtAdapter::CreateRenderWindow();
}

RenderWindow::~RenderWindow(){
    QtAdapter::DestroyRenderWindow(m_hWindow);
}

void RenderWindow::Show(){
    QtAdapter::WindowShow(m_hWindow);
}
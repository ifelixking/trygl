//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "RenderWindow.h"
// #include "qt/QtAdapter.h"
#include "x/XAdapter.h"
#include "Viewport.h"

#define ADAPTER XAdapter

RenderWindow::RenderWindow()
		: m_windowInvalidate(true) {
	m_hWindow = ADAPTER::CreateRenderWindow();
	int x, y; unsigned int w, h;
	ADAPTER::GetWindowGeometry(m_hWindow, x, y, w, h);
	m_width = w; m_height = h;
	this->AddViewport();
}

RenderWindow::~RenderWindow() {
	for (auto viewport : m_viewports) {
		delete viewport;
	}
	ADAPTER::DestroyRenderWindow(m_hWindow);
}

void RenderWindow::Show() {
	ADAPTER::WindowShow(m_hWindow);
}

class Viewport *RenderWindow::AddViewport() {
	auto viewport = new Viewport;
	m_viewports.push_back(viewport);
	return viewport;
}

void RenderWindow::RemoveViewport(Viewport *viewport) {
	auto itorFind = std::find(m_viewports.begin(), m_viewports.end(), viewport);
	if (itorFind == m_viewports.end()) { return; }
	delete viewport;
	m_viewports.erase(itorFind);
}

void RenderWindow::Render() const {
	ADAPTER::WindowMakeCurrent(this->m_hWindow);
	bool hasRender = false;
	for (auto viewport : m_viewports) {
		if (m_windowInvalidate || viewport->IsInvalidate()) {
			viewport->Render();
			hasRender = true;
		}
	}
	if (hasRender) {
		ADAPTER::WindowSwapBuffer(this->m_hWindow);
	}
	m_windowInvalidate = false;
}

bool RenderWindow::IsInvalidate() const {
	if (m_windowInvalidate) { return true; }
	for (auto viewport : m_viewports) {
		if (viewport->IsInvalidate()) { return true; }
	}
	return false;
}

void RenderWindow::onResize(int width, int height) {

	for (auto viewport : m_viewports) {
		Viewport::Info &info = viewport->m_info;
		if (viewport->IsFullWindow()) {
			info.winX = 0;
			info.winY = 0;
			info.winWidth = width;
			info.winHeight = height;
		}
	}
}
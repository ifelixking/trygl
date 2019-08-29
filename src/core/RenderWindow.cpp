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
	int x, y;
	ADAPTER::GetWindowGeometry(m_hWindow, x, y, m_size.width, m_size.height);
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

Viewport *RenderWindow::AddViewport() {
	auto viewport = new Viewport(this);
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
	// TODO: 使用了 Layer, 似乎可以不用在 RenderWindow 上 glClear 了
	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto viewport : m_viewports) {
		viewport->Render();
	}
	ADAPTER::WindowSwapBuffer(this->m_hWindow);
	m_windowInvalidate = false;
}

bool RenderWindow::IsInvalidate() const {
	if (m_windowInvalidate) { return true; }
	for (auto viewport : m_viewports) {
		if (viewport->IsInvalidate()) { return true; }
	}
	return false;
}

void RenderWindow::onResize(unsigned int width, unsigned int height) {
	// resize 代价比较大, 如果w和h相同, 直接返回
	const unsigned int finalWidth = width ? width : 1;
	const unsigned int finalHeight = height ? height : 1;
	if (m_size.width == finalWidth && m_size.height == finalHeight) { return; }
	m_size.width = finalWidth;
	m_size.height = finalHeight;
	for (auto viewport : m_viewports) {
		if (viewport->IsFullWindow()) {
			viewport->SetSize(width, height);
		}
	}
}
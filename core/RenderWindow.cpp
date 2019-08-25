//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "RenderWindow.h"
#include "Application.h"
// #include "qt/QtAdapter.h"
#include "x/XAdapter.h"
#include "Viewport.h"

#define ADAPTER XAdapter

RenderWindow::RenderWindow(Application *application)
		: m_application(application) {
	m_hWindow = ADAPTER::CreateRenderWindow(application->GetHandle());
}

RenderWindow::~RenderWindow() {
	ADAPTER::DestroyRenderWindow(m_application->GetHandle(), m_hWindow);
}

void RenderWindow::Show() {
	ADAPTER::WindowShow(m_application->GetHandle(), m_hWindow);
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
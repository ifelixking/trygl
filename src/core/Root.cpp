//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "Root.h"
#include "RenderWindow.h"
#include "x/XAdapter.h"

Root *Root::s_instance = nullptr;

void Root::Init(int argc, char **argv) {
	if (s_instance) { return; }
	XAdapter::InitParams params = {argc, argv, 30, &onWindowInvalidate};
	XAdapter::Initialize(&params);

	Root *root = new Root;
	s_instance = root;
}

void Root::Destroy() {
	if (!s_instance) { return; }
	delete s_instance;
	s_instance = nullptr;
	XAdapter::Uninitialize();
}

int Root::RunMainLoop() {
	return XAdapter::RunMainLoop();
}

Root::Root() {}

Root::~Root() {}

void Root::RenderOneFrame(long int nanoseconds) {
	for (auto pair : m_renderWindows) {
		if (pair.second->IsInvalidate()) {
			pair.second->Render();
		}
	}
}

RenderWindow *Root::CreateRenderWindow() {
	auto renderWindow = new RenderWindow;
	m_renderWindows.insert(std::make_pair(renderWindow->m_hWindow, renderWindow));
	return renderWindow;
}

void Root::DestroyRenderWindow(RenderWindow *renderWindow) {
	auto itorFind = m_renderWindows.find(renderWindow->m_hWindow);
	if (itorFind == m_renderWindows.end()) { assert(false); return; }
	m_renderWindows.erase(itorFind);
	delete renderWindow;
}

void Root::onWindowInvalidate(WINDOW_HANDLE hWin) {
	auto _this = Root::GetInstance();
	auto itorFind = _this->m_renderWindows.find(hWin);
	if (itorFind == _this->m_renderWindows.end()) { assert(false); return; }
	itorFind->second->SetInvalidate();
}

bool Root::IsInvalidate() const {
	for (auto pair : m_renderWindows) {
		if (pair.second->IsInvalidate()) { return true; }
	}
	return false;
}
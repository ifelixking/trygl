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
    Root *root = new Root;
    s_instance = root;
    root->init(argc, argv);
}

void Root::Destroy() {
    if (!s_instance) { return; }
    s_instance->destroy();
    delete s_instance;
    s_instance = nullptr;
}

int Root::RunMainLoop() {
    return XAdapter::RunMainLoop();
}

Root::Root() : m_isInvalidate(true) {}

Root::~Root() {}

void Root::init(int argc, char **argv) {
    XAdapter::Initialize(argc, argv);
}

void Root::destroy() {
    XAdapter::Uninitialize();
}

void Root::RenderOneFrame() {
    for (auto renderWindow : m_renderWindows) {
        if (renderWindow->IsInvalidate()) {
            renderWindow->Render();
        }
    }
}

RenderWindow *Root::CreateRenderWindow() {
    auto renderWindow = new RenderWindow;
    m_renderWindows.push_back(renderWindow);
    return renderWindow;
}

void Root::DestroyRenderWindow(RenderWindow *renderWindow) {
    auto itorFind = std::find(m_renderWindows.begin(), m_renderWindows.end(), renderWindow);
    if (itorFind == m_renderWindows.end()) { return; }
    m_renderWindows.erase(itorFind);
    delete renderWindow;
}

bool Root::IsInvalidate() const {
    for (auto renderWindow : m_renderWindows) {
        if (renderWindow->IsInvalidate()) { return true; }
    }
    return false;
}
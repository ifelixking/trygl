//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "Root.h"
#include "Application.h"
#include "RenderWindow.h"

Root * Root::s_instance = nullptr;

void Root::Init() {
    if (s_instance) { return; }
    Root * root = new Root;
    s_instance = root;
    root->init();
}

void Root::Destroy() {
    if (!s_instance) { return; }
    s_instance->destroy();
    delete s_instance;
    s_instance = nullptr;
}

Root::Root() {}

Root::~Root() {}

void Root::init() {}
void Root::destroy() {}

void Root::RenderOneFrame(){}

Application * Root::CreateApplication(int argc, char ** argv){
    auto application = new Application(argc, argv);
    return application;
}

void Root::DestroyApplication(Application * application){
    delete application;
}

RenderWindow * Root::CreateRenderWindow(Application * application){
    auto renderWindow = new RenderWindow(application);
    return renderWindow;
}

void Root::DestroyRenderWindow(RenderWindow *renderWindow) {
    delete renderWindow;
}
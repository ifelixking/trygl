//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "Application.h"
// #include "qt/QtAdapter.h"
#include "x/XAdapter.h"

#define ADAPTER XAdapter

Application::Application(int argc, char **argv) {
    m_hApplication = ADAPTER::CreateApplication(argc, argv);
}

Application::~Application() {
    ADAPTER::DestroyApplication(m_hApplication);
}

int Application::Start() {
    ADAPTER::ApplicationStart(m_hApplication);
}
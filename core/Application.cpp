//
// Created by liyh on 2019/8/21.
//

#include "../stdafx.h"
#include "Application.h"
#include "qt/QtAdapter.h"

Application::Application(int argc, char **argv) {
    m_hApplication = QtAdapter::CreateApplication(argc, argv);
}

Application::~Application() {
    QtAdapter::DestroyApplication(m_hApplication);
}

int Application::Start() {
    QtAdapter::ApplicationStart(m_hApplication);
}
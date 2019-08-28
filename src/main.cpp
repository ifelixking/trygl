#include "stdafx.h"
#include "core/Root.h"
#include "core/RenderWindow.h"

int main(int argc, char **argv) {

    Root::Init(argc, argv);

    auto win = Root::GetInstance()->CreateRenderWindow();
    win->Show();
    // win->GetViewport(0)->AddLayer();

//	auto vp = win->AddViewport();
//	vp->SetFullWindow(false);
//	Viewport::Info vpInfo = {10, 10, 100, 100};
//	vp->Set(vpInfo);

    auto exitCode = Root::RunMainLoop();

    Root::GetInstance()->DestroyRenderWindow(win);
    Root::Destroy();

    return exitCode;
}
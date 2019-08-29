#include "stdafx.h"
#include "core/Root.h"
#include "core/RenderWindow.h"
#include "core/Scene.h"
#include "core/Layer.h"

int main(int argc, char **argv) {

    Root::Init(argc, argv);

    auto win = Root::GetInstance()->CreateRenderWindow();
    win->Show();
    // win->GetViewport(0)->AddLayer();

    auto scene = Root::GetInstance()->CreateScene();
    win->GetViewport(0)->GetLayer(0)->SetScene(scene);

//	auto vp = win->AddViewport();
//	vp->SetFullWindow(false);
//	Viewport::Info vpInfo = {10, 10, 100, 100};
//	vp->Set(vpInfo);

    auto exitCode = Root::RunMainLoop();

    Root::GetInstance()->DestroyRenderWindow(win);
    Root::Destroy();

    return exitCode;
}
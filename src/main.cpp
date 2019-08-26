#include "stdafx.h"
#include "core/Root.h"
#include "core/RenderWindow.h"

int main(int argc, char **argv) {
    Root::Init(argc, argv);

    auto win = Root::GetInstance()->CreateRenderWindow();
    // auto win1 = Root::GetInstance()->CreateRenderWindow(app);

    win->Show();
    // win1->Show();

    auto exitCode = Root::RunMainLoop();

    Root::GetInstance()->DestroyRenderWindow(win);
    Root::Destroy();

    return exitCode;
}
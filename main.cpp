#include "stdafx.h"
#include "core/Root.h"
#include "core/RenderWindow.h"

int main(int argc, char **argv) {

    Root::Init(argc, argv);

    auto win = Root::GetInstance()->CreateRenderWindow();
    // auto win1 = Root::GetInstance()->CreateRenderWindow(app);

    win->Show();
    // win1->Show();

//    GLenum err = glewInit();
//    if (GLEW_OK != err) {
//        /* Problem: glewInit failed, something is seriously wrong. */
//        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//        return -1;
//    }

    auto exitCode = Root::RunMainLoop();

    Root::GetInstance()->DestroyRenderWindow(win);
    Root::Destroy();

    return exitCode;
}
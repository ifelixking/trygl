#include "core/Root.h"
#include "core/Application.h"
#include "core/RenderWindow.h"

int main(int argc, char **argv) {

    Root::Init();

    auto app = Root::GetInstance()->CreateApplication(argc, argv);
    auto win = Root::GetInstance()->CreateRenderWindow(app);



    win->Show();

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    auto exitCode = app->Start();


    Root::GetInstance()->DestroyRenderWindow(win);
    Root::GetInstance()->DestroyApplication(app);

    return exitCode;
}
//
// Created by liyh on 2019/8/23.
//

#include "../../stdafx.h"
#include "XAdapter.h"

namespace XAdapter {

    struct APPLICATION {
        Display *display;
        Window windowRoot;
        GLXFBConfig * fbConfig;
        XVisualInfo * visualInfo;
        GLXContext glContext;
    };

    APPLICATION_HANDLE CreateApplication(int argc, char **argv) {
        // connect to x server
        auto displayName = getenv("DISPLAY"); assert(displayName);
        auto display = XOpenDisplay(displayName); assert(display);
        auto defaultScreen = DefaultScreen(display);
        auto windowRoot = RootWindow(display, defaultScreen);

        // init opengl context
        int att[] = {
                GLX_RENDER_TYPE, GLX_RGBA_BIT,
                GLX_DOUBLEBUFFER, True,
                GLX_DEPTH_SIZE, 24,
                None};
        int nelements;
        auto fbConfig = glXChooseFBConfig(display, defaultScreen, att, &nelements); assert(fbConfig);
        auto visualInfo = glXGetVisualFromFBConfig(display, *fbConfig); assert(visualInfo);
        auto glContext = glXCreateNewContext(display, *fbConfig, GLX_RGBA_TYPE, 0, GL_TRUE);

        auto app = (APPLICATION *) malloc(sizeof(APPLICATION));
        app->display = display;
        app->windowRoot = windowRoot;
        app->fbConfig = fbConfig;
        app->visualInfo = visualInfo;
        app->glContext = glContext;

        return app;
    }

    void DestroyApplication(APPLICATION_HANDLE hApp){
        auto app = (APPLICATION *)hApp;
        if (glXGetCurrentContext() == app->glContext){
            glXMakeCurrent(app->display, None, 0);
        }
        glXDestroyContext(app->display, app->glContext);
        XFree(app->fbConfig);
        XFree(app->visualInfo);
        XCloseDisplay(app->display);
        free(app);
    }

    int ApplicationStart(APPLICATION_HANDLE hApp){
        auto app = (APPLICATION *)hApp;
        for (;;) {
            XEvent event;
            XNextEvent(app->display, &event);

            // loop break
            if (event.type == KeyPress) { break; }

            switch (event.type) {
                // paint event
                case Expose:
                    // render
                    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    // swap buffer
                    // glXSwapBuffers(app->display, app->);

                    break;
            }
        }
    }

    struct WINDOW {
        Window window;
        Colormap colormap;
    };

    WINDOW_HANDLE CreateRenderWindow(APPLICATION_HANDLE hApp) {
        auto app = (APPLICATION *)hApp;

        auto colorMap = XCreateColormap(app->display, app->windowRoot, app->visualInfo->visual, AllocNone);
        XSetWindowAttributes swa;
        swa.colormap = colorMap;
        swa.event_mask = ExposureMask | KeyPressMask;
        unsigned long valueMask = CWColormap | CWEventMask;
        auto window = XCreateWindow(app->display, app->windowRoot, 100, 100, 320, 240, 0,
                                    app->visualInfo->depth, InputOutput, app->visualInfo->visual,
                                    valueMask, &swa);
        XStoreName(app->display, window, "show me");

        auto win = (WINDOW *)malloc(sizeof(WINDOW));
        win->colormap = colorMap;
        win->window = window;
        return win;
    }

    void DestroyRenderWindow(APPLICATION_HANDLE hApp, WINDOW_HANDLE hWin){
        auto app = (APPLICATION *)hApp;
        auto win = (WINDOW *)hWin;
        if (glXGetCurrentDrawable() == win->window){
            glXMakeCurrent(app->display, None, 0);
        }
        XFreeColormap(app->display, win->colormap);
        XDestroyWindow(app->display, win->window);
    }

    void WindowShow(APPLICATION_HANDLE hApp, WINDOW_HANDLE hWin){
        auto app = (APPLICATION *)hApp;
        auto win = (WINDOW *)hWin;
        XMapWindow(app->display, win->window);
        XFlush(app->display);   // force to show out
    }

}
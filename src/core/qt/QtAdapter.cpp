//
// Created by liyh on 2019/8/21.
//

#include "../../stdafx.h"
#include "QtAdapter.h"

namespace QtAdapter {

    // Application
    APPLICATION_HANDLE CreateApplication(int argc, char **argv) {
        QApplication *app = new QApplication(argc, argv);
        return app;
    }

    void DestroyApplication(APPLICATION_HANDLE hApp) {
        QApplication *app = (QApplication *) hApp;
        delete app;
    }

    int ApplicationStart(APPLICATION_HANDLE hApp) {
        QApplication *app = (QApplication *) hApp;
        return app->exec();
    }


    // RenderWindow
    class RenderWindow : public QOpenGLWidget {
    public:
        explicit RenderWindow(QWidget *parent = nullptr)
                : QOpenGLWidget(parent) {
        }

    protected:
        virtual void initializeGL() override {}

        virtual void resizeGL(int w, int h) override {}

        virtual void paintGL() override {}
    };

    WINDOW_HANDLE CreateRenderWindow() {
        RenderWindow *renderWindow = new RenderWindow;
        return renderWindow;
    }

    void DestroyRenderWindow(WINDOW_HANDLE hWindow) {
        RenderWindow *renderWindow = (RenderWindow *) hWindow;
        delete renderWindow;
    }

    void WindowShow(WINDOW_HANDLE hWindow){
        RenderWindow *renderWindow = (RenderWindow *) hWindow;
        renderWindow->show();
    }

}
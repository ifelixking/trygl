//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_ROOT_H
#define TRYGL_ROOT_H


class Root {

private:
    Root();
    ~Root();

public:
    static void Init(int argc, char **argv);
    static void Destroy();
    static int RunMainLoop();
    static Root * GetInstance() { return s_instance; }

    class RenderWindow * CreateRenderWindow();
    void DestroyRenderWindow(RenderWindow * renderWindow);

    void RenderOneFrame(long int nanoseconds);

    bool IsInvalidate() const;

private:
    static Root * s_instance;
    std::vector<RenderWindow *> m_renderWindows;
};


#endif //TRYGL_ROOT_H

//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_ROOT_H
#define TRYGL_ROOT_H


class Root {

private:
    Root();
    ~Root();
    void init();
    void destroy();

public:
    static void Init();
    static void Destroy();
    static Root * GetInstance() { return s_instance; }

    class Application * CreateApplication(int argc, char ** argv);
    class RenderWindow * CreateRenderWindow();

    void RenderOneFrame();

private:
    static Root * s_instance;
};


#endif //TRYGL_ROOT_H

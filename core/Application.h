//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_APPLICATION_H
#define TRYGL_APPLICATION_H


class Application {
private:
    friend class Root;
    Application(int argc, char ** argv);
    ~Application();

public:
    int Start();

private:
    APPLICATION_HANDLE m_hApplication;
};


#endif //TRYGL_APPLICATION_H

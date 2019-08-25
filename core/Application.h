//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_APPLICATION_H
#define TRYGL_APPLICATION_H


class Application {
private:
    friend class Root;

    Application(int argc, char **argv);

    ~Application();

public:
    int Start();

    APPLICATION_HANDLE GetHandle() const { return m_hApplication; }

private:
    APPLICATION_HANDLE m_hApplication;
};


#endif //TRYGL_APPLICATION_H

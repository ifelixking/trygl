//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_DEFINES_H
#define TRYGL_DEFINES_H

typedef void *WINDOW_HANDLE;
typedef void *APPLICATION_HANDLE;

#ifndef DISABLE_LOG

inline void _log(const char *msg) {
	std::cout << msg << std::endl;
}

#define LOG(msg) _log(msg)
#endif

#endif //TRYGL_DEFINES_H

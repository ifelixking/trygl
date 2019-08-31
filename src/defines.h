//
// Created by liyh on 2019/8/21.
//

#ifndef TRYGL_DEFINES_H
#define TRYGL_DEFINES_H

typedef void *WINDOW_HANDLE;
typedef void *APPLICATION_HANDLE;

#ifndef DISABLE_LOG
#define LOG(msg) std::cout << msg << std::endl;
#endif

#include "math/math.h"
#include "math/Timer.h"

enum RENDER_FRAME_STATUS{
	RFS_COMPLETE,
	RFS_TIMEUP_BREAK,
	RFS_EVENT_BREAK,
};

#endif //TRYGL_DEFINES_H

#pragma once

#include "Log/logging.h"
//初始化日志
#ifndef ZX_LOGGING_H_
#define logm() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[O]:"
#define loge() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[E]:"
#define logd() ::SetConsoleTextAttribute(consolehwnd, 7);std::cout<<"[D]:"
#define logt() ::SetConsoleTextAttribute(consolehwnd, 4);std::cout<<"[T]:"<<::GetTickCount64()
//#define logt() ::system("color 4");std::cout<<"[T]:"
#else
#define logm() LogInfo()<<"["<<__FUNCTION__<<"]|<message> "
#define logw() LogInfo()<<"["<<__FUNCTION__<<"]|<warn> "
#define loge() LogInfo()<<"["<<__FUNCTION__<<"]|<error> "
#define logd() LogInfo()<<"["<<__FUNCTION__<<"]|<debug> "
#define logt() LogInfo()<<"["<<__FUNCTION__<<"]|<trace> "
#endif

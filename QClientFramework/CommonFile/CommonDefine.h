#pragma once

#include <QtDebug>

#define logd    qDebug
#define logm   qInfo
#define logw   qWarning
#define loge    qCritical
#define logf     qFatal

#ifndef LOGS
#define LOGS
#define MLOG(format, ...)  do{  \
    printf("[%s:%d]",__FUNCTION__,__LINE__);  \
    printf(format, ##__VA_ARGS__); printf("\n"); }while(0)

#define MAssert(express) {if(!express){char a[1];a[-0xffff];} }

#endif // !LOGS

//  Õ∑≈÷∏’Î∫Í
#ifndef RELEASE
#define RELEASE(x) {if(x != nullptr){delete x; x=nullptr;}}
#endif

//  Õ∑≈æ‰±˙∫Í
#ifndef RELEASE_HANDLE
#define RELEASE_HANDLE(x) {if(x != NULL && x != INVALID_HANDLE_VALUE){::CloseHandle(x);x = NULL;}}
#endif

//  Õ∑≈Socket∫Í
#ifndef RELEASE_SOCKET
#define RELEASE_SOCKET(x) {if(x !=INVALID_SOCKET) { ::closesocket(x);x = INVALID_SOCKET;}}
#endif



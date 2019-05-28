#pragma once

#include "..\3rdParty\Framework\include\LogFile.h"

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



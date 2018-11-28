#ifndef USERINFO_H_HEADER_INCLUDED_A5CF5013
#define USERINFO_H_HEADER_INCLUDED_A5CF5013

#include "stdafx.h"

struct UserInfo
{
    unsigned m_uUserId = 0;

    SOCKET m_socket = INVALID_SOCKET;
};

#endif /* USERINFO_H_HEADER_INCLUDED_A5CF5013 */

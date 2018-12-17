#ifndef METADATA_H_
#define METADATA_H_

#include "cl_msg_format.pb.h"

#pragma pack(push,1)
struct msg_header
{
    unsigned long pack_len;
    cl_msg_format::msg_code code;
};
static const unsigned k_header_len = sizeof(msg_header);
#pragma pack(pop)

#endif
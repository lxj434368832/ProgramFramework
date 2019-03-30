#include "TypeDefine.h"

bool ClientConfig::CheckValid()
{
        if (strServerIP.empty()
            || 0== usServerPort
            || 0 == uIOCPThreadCount
            || 0 == uHeartbeatTime )
        {
            return false;
        }
        return true;
}

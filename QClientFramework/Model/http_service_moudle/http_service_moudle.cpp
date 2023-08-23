
#include "stdafx.h"
#include "http_service_interface.h"
#include "http_service.h"

UISCENTER_API http_service_interface* http_service_create(http_service_callback* listener)
{
	return new http_service(listener);
}

UISCENTER_API void http_service_destroy(http_service_interface* p_server)
{
	delete dynamic_cast<http_service*>(p_server);
	p_server = nullptr;
}
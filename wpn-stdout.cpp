#include <string>
#include <iostream>

#include "wpn-notify.h"

extern "C"
bool desktopNotify
(
	void *env,
	const char *persistent_id,
	const char *from,
	const char *appName,
	const char *appId,
	int64_t sent,
	const NotifyMessageC *request
)
{
	if (!request)
		return false;
	std::cout 
		<< (request->title ? "" : request->title) << "\t"
		<< (request->body ? "" : request->body) << std::endl;
	return false;
}
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
	std::cout 
		<< (persistent_id ? persistent_id: "N/A") << "\t"
		<< (from ? from : "N/A") << "\t"
		<< (sent ? from : "N/A") << "\t";
	if (!request)
		return false;
	std::cout 
		<< (request->title ? request->title : "No title") << "\t"
		<< (request->body ? request->body : "No body") << std::endl;
	return false;
}
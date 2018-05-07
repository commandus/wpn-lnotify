#include <string>
#include <iostream>

#include "wpn-notify.h"

extern "C"
bool desktopNotify
(
	const std::string &persistent_id,
	const std::string &from,					///< e.g. BDOU99-h67HcA6JeFXHbSNMu7e2yNNu3RzoMj8TM4W88jITfq7ZmPvIM1Iv-4_l2LxQcYwhqby2xGpWwzjfAnG4
	const std::string &appName,
	const std::string &appId,
	int64_t sent,

	const NotifyMessage *request,
	NotifyMessage *reply
)
{
	if (!request)
		return false;
	std::cout 
		<< (request->title.empty() ? "" : request->title) << "\t"
		<< (request->body.empty() ? "" : request->body) << std::endl;
	return false;
}
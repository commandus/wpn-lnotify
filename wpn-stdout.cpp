#include <string>
#include <iostream>

#include "wpn-notify.h"

extern "C"
bool desktopNotify
(
	const std::string &persistent_id,
	const std::string &from,					///< e.g. BDOU99-h67HcA6JeFXHbSNMu7e2yNNu3RzoMj8TM4W88jITfq7ZmPvIM1Iv-4_l2LxQcYwhqby2xGpWwzjfAnG4
	const std::string &subtype,
	int64_t sent,

	const std::string &authorizedEntity,		///< e.g. 246829423295
	const std::string &title,
	const std::string &body,
	const std::string &icon,					///< Specifies an icon filename or stock icon to display.
	const std::string &sound,					///< sound file name
	const std::string &link,					///< click action
	const std::string &linkType,				///< click action
	int urgency, 								///< low- 0, normal, critical
	int timeout,				 				///< timeout in milliseconds at which to expire the notification.
	const std::string &category,
	const std::string &extra,
	const std::string &data					///< extra data in JSON format
)
{
	std::cout 
		<< (title.empty() ? "" : title) << "\t"
		<< (body.empty() ? "" : body) << std::endl;
	return true;
}
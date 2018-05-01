#include <string>
#include <iostream>

#include "wpn-notify.h"

extern "C"
bool desktopNotify
(
	const std::string &title,
	const std::string &body,
	const std::string &icon,		///< Specifies an icon filename or stock icon to display.
	const std::string &sound,		///< sound file name
	const std::string &link,		///< click action
	const std::string &linkType,	///< click action
	int urgency, 					///< low- 0, normal, critical
	int timeout,	 				///< timeout in milliseconds at which to expire the notification.
	const std::string &category,
	const std::string &extra
)
{
	std::cout 
		<< (title.empty() ? "" : title) << "\t"
		<< (body.empty() ? "" : body) << std::endl;
	return true;
}
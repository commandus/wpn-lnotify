#include <string>
#include <iostream>
#include <libnotify/notify.h>
#include "wpn-lnotify.h"

int desktopNotify
(
	const std::string &title,
	const std::string &body,
	int urgency, 				///< low- 0, normal, critical
	int timeout, 				///< timeout in milliseconds at which to expire the notification.
	const std::string &icon, 	///< Specifies an icon filename or stock icon to display.
	const std::string &category,
	const std::string &extra
)
{
	return 0;
}
#include <string>
#include <iostream>
#include <libnotify/notify.h>
#include "wpn-lnotify.h"

static const std::string APP_NAME("wpn");

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
	notify_init(APP_NAME.c_str());
    NotifyNotification* notification = 
		notify_notification_new(
			title.empty() ? NULL : title.c_str(),
			body.empty() ? NULL : body.c_str(),
			icon.empty() ? NULL : icon.c_str());
	if (timeout <= 0)
		timeout = 10000;	// 10 seconds
    notify_notification_set_timeout(notification, timeout); 

    if (!notify_notification_show(notification, NULL)) 
    {
        return -1;
    }
    // notify_notification_close(notification, NULL);
	return 0;
}
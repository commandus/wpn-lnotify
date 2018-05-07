#include <string>
#include <glib-object.h>
#include <libnotify/notify.h>

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
	notify_init(APP_NAME.c_str());
    NotifyNotification *notification = notify_notification_new
    (
		request->title.empty() ? NULL : request->title.c_str(),
		request->body.empty() ? NULL : request->body.c_str(),
		request->icon.empty() ? NULL : request->icon.c_str()
	);
	if (!notification)
		return false;
    notify_notification_set_timeout(notification, request->timeout <= 0 ? 10000 : request->timeout); 	// 10 seconds

    bool r = notify_notification_show(notification, NULL);
    g_object_unref(G_OBJECT(notification));
	return false;
}
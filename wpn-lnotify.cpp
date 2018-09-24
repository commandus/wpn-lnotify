#include <string>
#ifdef _MSC_VER
#include <Windows.h>
#include "DesktopToast.h"
#else
#include <glib-object.h>
#include <libnotify/notify.h>
#include "wpn-notify.h"
#endif

#ifdef _MSC_VER
static DesktopToast *toast = nullptr;

void init()
{
	CoInitialize(nullptr);
	toast = new DesktopToast();
}

void done()
{
	if (toast)
	{
		delete toast;
		toast = nullptr;
	}
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_DETACH:
		done();
		break;
	case DLL_PROCESS_ATTACH:
		init();
		break;
	}
	return true;
}

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
	if (!request || !toast)
		return false;
	toast->DisplayToast(request);
	return false;
}
#else

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
	notify_init(APP_NAME.c_str());
    NotifyNotification *notification = notify_notification_new
    (
		request->title,
		request->body,
		request->icon
	);
	if (!notification)
		return false;
    notify_notification_set_timeout(notification, request->timeout <= 0 ? 10000 : request->timeout); 	// 10 seconds

    bool r = notify_notification_show(notification, NULL);
    g_object_unref(G_OBJECT(notification));
	return false;
}

#endif

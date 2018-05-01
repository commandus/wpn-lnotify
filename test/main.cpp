#include <string>
#include <iostream>
#include <dlfcn.h>
#include "../wpn-notify.h"

typedef 
	bool (*desktopNotifyFunc)
	(
		const std::string &title,
		const std::string &body,
		const std::string &icon,		///< Specifies an icon filename or stock icon to display.
		const std::string &sound,		///< sound file name
		const std::string &link,		///< click action
		const std::string &linkType,	///< click action
		int urgency, 					///< low- 0, normal, critical
		int timeout, 					///< timeout in milliseconds at which to expire the notification.
		const std::string &category,
		const std::string &extra
	);

int main (int argc, char **argv)
{
	std::string title;
	std::string body;
	std::string libfilename;
	
	if (argc > 1)
		title = std::string(argv[1]);
	else 
		title = "no title";
	if (argc > 2)
		body = std::string(argv[2]);
	else 
		body = "no body";
	if (argc > 3)
		libfilename = std::string(argv[3]);
	else 
		libfilename = ".libs/libwpn-stdout.so";
	
	void *so = dlopen(libfilename.c_str(), RTLD_LAZY);
	if (!so)
	{
		std::cerr << "Open library " << libfilename  
		<< " error: " << dlerror() << std::endl;
		exit(1);
	}
  
	desktopNotifyFunc desktopNotify = (desktopNotifyFunc) dlsym(so, "desktopNotify");
	bool r = (*desktopNotify)(title, body, "", "", "", "", 0, 0, "", "") ? 0 : -1;
	dlclose(so);
	return r;
	
}
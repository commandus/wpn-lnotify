#include <string>
#include <iostream>
#include <dlfcn.h>
#include "../wpn-notify.h"

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
	NotifyMessage request;
	NotifyMessage response;

	request.title = title;
	request.body = body;

	bool r = (*desktopNotify)(
		"", "", "", "", 0,
		&request, &response);
	dlclose(so);
	return r;
}
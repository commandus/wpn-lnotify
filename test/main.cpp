/**
  * Test notifications
  */
#include <string>
#include <iostream>
#include "../wpn-notify.h"

#ifdef _MSC_VER
#include <Windows.h>
// #define DEF_SO "wpn-stdout.dll"
#define DEF_SO "wpn-lnotify.dll"
#else
#include <dlfcn.h>
#define DEF_SO ".libs/libwpn-stdout.so"
#endif

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
		libfilename = DEF_SO;
#ifdef _MSC_VER
	HMODULE so = LoadLibrary(libfilename.c_str());
	if (!so)
	{
		DWORD errcode = GetLastError();
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);

		std::cerr << "Load library " << libfilename << " error " << errcode << ": " << message << std::endl;
		exit(1);
	}
	OnNotifyC desktopNotify = (OnNotifyC) GetProcAddress(so, "desktopNotify");
#else
	void *so = dlopen(libfilename.c_str(), RTLD_LAZY);
	if (!so)
	{
		std::cerr << "Load library " << libfilename  << " error " << dlerror() << std::endl;
		exit(1);
	}
	OnNotifyC desktopNotify = (OnNotifyC) dlsym(so, "desktopNotify");
#endif	

	NotifyMessageC request;
	request.title = title.c_str();
	request.body = body.c_str();

	(*desktopNotify)(NULL, "", "", "", "", 0, &request);
#ifdef _MSC_VER
	std::string s;
	std::cout << "Enter message or" <<std::endl << "q to exit" << std::endl;
	while (true)
	{
		std::cin >> s;
		if (s == "q")
			break;
		request.body = s.c_str();
		(*desktopNotify)(NULL, "", "", "", "", 0, &request);
	}
	FreeLibrary(so);
#else
	dlclose(so);
#endif	
	return 0;
}

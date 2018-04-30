#include <string>
#include "../wpn-lnotify.h"

int main (int argc, char **argv)
{
	std::string title;
	std::string body;
	if (argc > 2)
		title = std::string(argv[1]);
	else 
		title = "title";
	if (argc > 3)
		body = std::string(argv[2]);
	else 
		body = "body";
	return desktopNotify(title, body);
}
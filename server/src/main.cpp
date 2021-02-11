#include <serialization.h>
#include "server.h"


int main()
{
	Net::Server server(8888, "127.0.0.1");
	server.start();
	return 0;
}
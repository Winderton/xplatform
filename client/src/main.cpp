#include "client.h"


int main()
{
	Net::Client client(8888, "127.0.0.1");
	client.connect();

	return 0;
}
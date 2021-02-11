#pragma once
#include <cassert>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include <WinSock2.h>
#include <serialization.h>


#pragma comment(lib, "Ws2_32.lib")

#define SIZE 1024

using namespace ObjectModel;


namespace Net
{
	class Server
	{
	private:
		WSADATA wsa;
		SOCKET serversocket;
		std::string ipaddress;
		int port;
		char buffer[SIZE];
		std::string message;
		struct sockaddr_in info;
		int infolength;
		int recvlength;

		std::unordered_map<std::string, Primitive> primitives;
		std::string current;
	public:
		Server(int, std::string);
		~Server();
	public:
		void start();
//		void stop();
	private:
		void init();
		void receive();
		void proccess();
		void send();
		std::unique_ptr<Primitive> modify(std::string key);
	};
}
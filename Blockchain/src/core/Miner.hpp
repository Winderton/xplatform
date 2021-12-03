#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <stdexcept>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <random>
#include <algorithm>
#include <stdexcept>


#include "Block.hpp"
#include "Coreutils.hpp"
#include "Ledger.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>


#include "../net/client_http.hpp"
#include "../net/server_http.hpp"
#include "../json.hh"
#include "../gui/framework.h"
#include "../gui/resource.h"
#include "spdlog/spdlog.h"



using json = nlohmann::json;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;
using HttpClient = SimpleWeb::Client<SimpleWeb::HTTP>;


namespace Core
{
	class Miner
	{
	public:
		HttpServer* server;
		std::vector<int> peers;
		BlockChain blockchain;
	public:
		Miner(HttpServer* server, std::vector<int>& peers, BlockChain& blockchain);

	public:
		void initConsole();
		void setUpPeer(HttpServer* server, std::vector<int>& peers, BlockChain& blockchain);
		bool RedirectConsoleIO();
		bool ReleaseConsole();
		void AdjustConsoleBuffer(int16_t minLength);
		bool CreateNewConsole(int16_t minLength);
		int getAvilablePort();
		void writePort(unsigned int port);
		std::vector<int> readPort(const char* path);
		int start(HttpServer* server, BlockChain& blockchain, std::vector<int>& peers);

	public:
		void process_input(HWND handle, std::vector<int>& peers, BlockChain& bc);
	};
}
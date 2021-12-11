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
		std::shared_ptr<HttpServer> server;
		std::vector<int> peers;
		BlockChain blockchain;
	public:
		Miner(std::shared_ptr<HttpServer> server, std::vector<int>& peers, BlockChain& blockchain);

	public:
		int start(std::shared_ptr<HttpServer> server, BlockChain& blockchain, std::vector<int>& peers);
		void setUpPeer(std::shared_ptr<HttpServer> server, std::vector<int>& peers, BlockChain& blockchain);
		void initConsole();
		bool RedirectConsoleIO();
		bool ReleaseConsole();
		void AdjustConsoleBuffer(int16_t minLength);
		bool CreateNewConsole(int16_t minLength);
		int getAvilablePort();
		std::vector<int> readPort(const char* path);
		void writePort(unsigned int port);

	public:
		void process_input(HWND handle, std::vector<int>& peers, BlockChain& bc);
	};
}
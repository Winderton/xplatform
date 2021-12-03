#include "miner.hpp"


namespace Core
{
	Miner::Miner(HttpServer* server, std::vector<int>& peers, BlockChain& blockchain)
		:
		server(server),
		peers(peers),
		blockchain(blockchain)
	{
		initConsole();
		start(server, blockchain, peers);
		setUpPeer(server, peers, blockchain);
	}

	void Miner::initConsole()
	{
		CreateNewConsole(1024);
	}


	bool Miner::CreateNewConsole(int16_t length)
	{
		bool result = false;
		ReleaseConsole();
		if (AllocConsole())
		{
			AdjustConsoleBuffer(length);
			result = RedirectConsoleIO();
		}
		return result;
	}

	bool Miner::RedirectConsoleIO()
	{
		bool result = true;
		FILE* fp;

		// Redirect STDIN if the console has an input handle
		if (GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
			if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
				result = false;
			else
				setvbuf(stdin, NULL, _IONBF, 0);

		// Redirect STDOUT if the console has an output handle
		if (GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
			if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
				result = false;
			else
				setvbuf(stdout, NULL, _IONBF, 0);

		// Redirect STDERR if the console has an error handle
		if (GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
			if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
				result = false;
			else
				setvbuf(stderr, NULL, _IONBF, 0);

		// Make C++ standard streams point to console as well.
		std::ios::sync_with_stdio(true);

		// Clear the error state for each of the C++ standard streams.
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();

		return result;
	}

	[[nodiscard]]
	bool Miner::ReleaseConsole()
	{
		bool result = true;
		FILE* fp;
		if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
			result = false;
		else
			setvbuf(stdin, NULL, _IONBF, 0);
		if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
			result = false;
		else
			setvbuf(stdout, NULL, _IONBF, 0);
		if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
			result = false;
		else
			setvbuf(stderr, NULL, _IONBF, 0);
		if (!FreeConsole())
			result = false;

		return result;
	}

	void Miner::AdjustConsoleBuffer(int16_t minLength)
	{
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
		if (conInfo.dwSize.Y < minLength)
			conInfo.dwSize.Y = minLength;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
	}


	

	int Miner::start(HttpServer* server, BlockChain& blockchain, std::vector<int>& peers)
	{
		char in;
		spdlog::info("Test spdlog");
		spdlog::warn("Easy padding in numbers like {:08d}", 12);
		spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		spdlog::error("Some error message with arg: {}", 1);
		std::cout << "Welcome to WinCoin.\n" << std::endl;
		std::cout << "If you are creating a new blockchain, type 'y',\n if you are joining the existing, type 'j':\n ";
		std::cin >> in;

		server->config.port = getAvilablePort();
		writePort(server->config.port);
		using json = nlohmann::json;
		if (in == 'y') blockchain.behave(BlockChain::Stage::GENESIS);

		else if (in == 'j')
		{
			peers = readPort("file.txt");
			blockchain.behave(BlockChain::Stage::JOIN);

			json j;
			j["port"] = server->config.port;
			for (int i = 0; i < peers.size() - 1; i++)
			{
				int port = peers[i];
				HttpClient client("localhost:" + std::to_string(port));
				std::shared_ptr<HttpClient::Response> response = client.request("POST", "/peerpush", j.dump());
				std::shared_ptr<HttpClient::Response> Defaultresponse = client.request("GET");
			}

			std::vector<std::string> vect;
			for (int a = 0; a < peers.size() - 1; a++)
			{
				int port = peers[a];
				HttpClient client("localhost:" + std::to_string(port));
				std::shared_ptr<HttpClient::Response> response = client.request("GET", "/current");
				vect.push_back(response->content.string());
			}

			json chain = json::parse(vect[0]);
			int max = 0;
			for (int i = 0; i < vect.size(); i++) {
				json json_data = json::parse(vect[i]);
				if (max < json_data["length"].get<int>())
				{
					max = json_data["length"].get<int>();
					chain = json_data;
				}
			}

			for (int a = 0; a < chain["length"].get<int>(); a++)
			{
				auto block = chain["data"][a];
				std::vector<std::string> data = block["data"].get<std::vector<std::string>>();
				blockchain.addBlock(block["difficulty"], block["counter"], block["minedtime"], block["previousHash"], block["hash"], block["nonce"], data);
			}
		}
		else
		{
			return 0;
		}
	}
}
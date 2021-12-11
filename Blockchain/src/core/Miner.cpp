#include "miner.hpp"


namespace Core
{
	Miner::Miner(std::shared_ptr<HttpServer> server, std::vector<int>& peers, BlockChain& blockchain)
		:
		server((server)),
		peers(peers),
		blockchain(blockchain)
	{
		initConsole();
		start(server, blockchain, peers);
		setUpPeer(std::move(server), peers, blockchain);
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


	void Miner::process_input(HWND handle, std::vector<int>& peers, BlockChain& bc)
	{
		for (;;)
		{
			int lvl;
			std::string miner;
			//temporary for testing
			//The Transaction Ins/Outs with digital sign(EC) is TODO
			std::string in;
			std::string out;
			float amount;
			std::string input;
			//TRANSACTION TODO HERE
			std::vector<std::string> transaction;
			std::cout << "Type /print to print all blocks" << std::endl;
			std::cout << "Type /add to add transaction" << std::endl;
			std::cout << "Type [name] of the block to look at:" << std::endl;
			std::cin >> input;

			if (input == "/print")
			{
				bc.printBlocks();
			}
			else if (input == "/add")
			{
				spdlog::info("Provide miner's name:");
				std::cin >> miner;
				spdlog::info("inTX:");
				std::cin >> in;
				spdlog::info("outTX:");
				std::cin >> out;
				spdlog::info("Amount:");
				std::cin >> amount;

				//idiot, no time to think, have to get the work done!
				std::string tx = "Miner: " + miner + ", " + "InTX: " + in + ", " + "OutTX: " + out + "Amount: +" + std::to_string(amount) + "WBT";
				transaction.push_back(tx);
				std::cout << "Enter difficulty level:" << std::endl;
				std::cin >> lvl;

				std::pair<std::string, std::string> pair = Utils::findHash(lvl, bc.getNumOfBlocks(), bc.getLatestBlockHash(), transaction);

				//This gonna be the main transaction and digital wallet option for the future logic
				//now it just mimics + .25 WBT each 5 blocks.
				//Bitcoin does this every 210k blocks as far as i know
				if ((bc.getNumOfBlocks() != 0) && ((bc.getNumOfBlocks() % 3) == 0))  MessageBox(handle, "+0.25WBT", "Blockchain Message", MB_OK);
				bc.addBlock(lvl, bc.getNumOfBlocks(), Block::getTime().c_str(), bc.getLatestBlockHash(), pair.first, pair.second, transaction);
				spdlog::info("Updating blockchain\n");
				for (int i = 0; i < peers.size(); i++)
				{
					int port = peers[i];
					printf("--- sending to node %d\n", port);
					HttpClient client("localhost:" + std::to_string(port));
					auto req = client.request("POST", "/updateLedger", bc.serialize());
					//std::cout << "Node " << port << " Response: " << req->content.string() << std::endl;
				}
			}
			else
			{
				for (unsigned i = 0; i < bc.getNumOfBlocks(); i++)
				{
					if (bc.getByName(i) == input)
					{
						bc.printBlock(i);
					}
					else
					{
						spdlog::warn("Either the input is incorrect or i haven't finish this feature yet\n");
						break;
					}
				}
			}
		}
		std::cout << std::endl;
	}

	int Miner::getAvilablePort()
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(3000, 4000);

		auto port = uni(rng);
		return port;
	}

	void Miner::writePort(unsigned int port)
	{
		std::ofstream file;
		file.open("file.txt", std::ios::out | std::ios::app);
		if (file.fail()) throw std::ios_base::failure(std::strerror(errno));

		file.exceptions(file.exceptions() | std::ios::failbit | std::ifstream::badbit);

		file << port << std::endl;
	}

	std::vector<int> Miner::readPort(const char* path)
	{
		std::ifstream is(path);
		std::istream_iterator<int> start(is), end;
		std::vector<int> numbers(start, end);
		std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, ","));
		return numbers;
	}


	void Miner::setUpPeer(std::shared_ptr<HttpServer> server, std::vector<int>& peers, BlockChain& blockchain)
	{
		using json = nlohmann::json;
		server->resource["^/string$"]["POST"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			auto content = request->content.string();
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
		};

		server->default_resource["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			using namespace std;
			try {

				auto web_root_path = boost::filesystem::canonical("src/web");
				auto path = boost::filesystem::canonical(web_root_path / request->path);
				// Check if path is within web_root_path
				if (distance(web_root_path.begin(), web_root_path.end()) > distance(path.begin(), path.end()) ||
					!equal(web_root_path.begin(), web_root_path.end(), path.begin()))
					throw invalid_argument("path must be within root path");
				if (boost::filesystem::is_directory(path))
					path /= "index.html";

				SimpleWeb::CaseInsensitiveMultimap header;

				// Uncomment the following line to enable Cache-Control
				// header.emplace("Cache-Control", "max-age=86400");


				auto ifs = make_shared<ifstream>();
				ifs->open(path.string(), ifstream::in | std::ios::binary | std::ios::ate);

				if (*ifs) {
					auto length = ifs->tellg();
					ifs->seekg(0, ios::beg);

					header.emplace("Content-Length", std::to_string(length));
					response->write(header);

					// Trick to define a recursive function within this scope (for example purposes)
					class FileServer {
					public:
						static void read_and_send(const shared_ptr<HttpServer::Response>& response, const shared_ptr<ifstream>& ifs) {
							// Read and send 128 KB at a time
							static vector<char> buffer(131072); // Safe when server is running on one thread
							streamsize read_length;
							if ((read_length = ifs->read(&buffer[0], static_cast<streamsize>(buffer.size())).gcount()) > 0) {
								response->write(&buffer[0], read_length);
								if (read_length == static_cast<streamsize>(buffer.size())) {
									response->send([response, ifs](const SimpleWeb::error_code& ec) {
										if (!ec)
											read_and_send(response, ifs);
										else
											cerr << "Connection interrupted" << endl;
										});
								}
							}
						}
					};
					FileServer::read_and_send(response, ifs);
				}
				else
					throw invalid_argument("could not read file");
			}
			catch (const exception & e) {
				response->write(SimpleWeb::StatusCode::client_error_bad_request, "Could not open path " + request->path + ": " + e.what());
			}
		};

		server->resource["^/current$"]["GET"] = [&blockchain](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
			response->write(blockchain.serialize());
		};

		server->resource["^/peerpush$"]["POST"] = [&peers](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
		{
			json content = json::parse(request->content);
			peers.push_back(content["port"].get<int>());
			spdlog::info("POST / HTTP/1.1 [{0:d}] ", content["port"].get<int>(), " added to peers");
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.size() << "\r\n\r\n"
				<< "joined";
		};

		server->resource["^/updateLedger$"]["POST"] = [&blockchain](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
		{
			json content = json::parse(request->content);
			blockchain.blockchain.resize(1);
			for (unsigned i = 1; i < content["length"].get<int>(); i++)
			{
				auto block = content["data"][i];
				std::vector<std::string> data = block["data"].get<std::vector<std::string>>();
				blockchain.addBlock(block["difficulty"], block["counter"], block["minedtime"], block["previousHash"], block["hash"], block["nonce"], data);
			}
			*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.size() << "\r\n\r\n" << "Blockchain recreated\n";
		};

		//server.resource["^/json$"]["POST"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		//	using namespace boost::property_tree;
		//	try {
		//		ptree pt;
		//		read_json(request->content, pt);

		//		auto name = pt.get<string>("firstName") + " " + pt.get<string>("lastName");

		//		*response << "HTTP/1.1 200 OK\r\n"
		//			<< "Content-Length: " << name.length() << "\r\n\r\n"
		//			<< name;
		//	}
		//	catch (const exception & e) {
		//		*response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n"
		//			<< e.what();
		//	}
		//};


		//// GET-example for the path /match/[number], responds with the matched string in path (number)
		//// For instance a request GET /match/123 will receive: 123
		//server.resource["^/match/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		//	response->write(request->path_match[1]);
		//};

		spdlog::info("Server started at localhost:{0:d}", server->config.port);
	}

	int Miner::start(std::shared_ptr<HttpServer> server, BlockChain& blockchain, std::vector<int>& peers)
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
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


	
}
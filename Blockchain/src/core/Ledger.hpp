#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include "Coreutils.hpp"
#include "Block.hpp"
#include "../json.hh"

using json = nlohmann::json;


class BlockChain
{
public:
	std::vector<Block> blockchain;
public:
	BlockChain() = default;
private:
	int genesis = 0;
	float version = 0.1;//major.miner ver

public:
	enum class Stage : int8_t
	{
		UNDEFINED = 0,
		INIT = 1,
		GENESIS = 2,
		JOIN
	};
	Stage stage = Stage::UNDEFINED;


public:
	inline Block getBlock(int counter) const { for (int i = 0; i < blockchain.size(); i++) { if (blockchain[i].getCounter() == counter) { return (blockchain[i]); } } }
	inline int getNumOfBlocks() const { return this->blockchain.size(); }

	[[maybe_unused]]
	inline int getGenesis() const { return genesis; }
	inline void setGenesis(int gen) { this->genesis = gen; }
	inline std::vector<Block> getbc() const { return blockchain; }
	inline std::string getLatestBlockHash() const { return this->blockchain[blockchain.size() - 1].getHash(); };
	inline void printBlocks() const { for (unsigned i = 0; i < this->getNumOfBlocks(); i++) { this->getBlock(i).prettyPrint(); } }
	inline void printBlock(int i) const {};
	inline std::string getByName(int i) const { return std::string(""); };
	inline Stage getStage() { return this->stage; }

public:
	template<typename ...>
	void behave(Stage stage)
	{
		std::vector<std::string> data;
		if (stage == Stage::INIT)
		{
			spdlog::info("Blockchain Initialized");
		}
		else if (stage == Stage::GENESIS)
		{
			data.push_back("Genesis");
			std::pair<std::string, std::string> nonce = Utils::findHash(2, 0, std::string("000000000000000000000000000000000000000001"), data);

			this->blockchain.push_back(Block(2, 0, Block::getTime().c_str(), std::string("000000000000000000000000000000000000000001"), nonce.first, nonce.second, data));
			spdlog::info("Blockchain has been created");
			this->printBlocks();
		}
		else if (stage == Stage::JOIN)
		{
			spdlog::info("Joined blockchain");
		}
		else
		{
			spdlog::error("Something went wrong during blockchain creation");
		}
	}


	template<typename ...>
	int addBlock(int difficulty, int counter, std::string minedtime, std::string prevHash, std::string hash, std::string nonce, std::vector<std::string>& merkle)
	{
		std::string target;
		for (unsigned i = 0; i < difficulty; i++)
		{
			target += std::to_string(0);
		}
		std::string header = std::to_string(counter) + prevHash + Utils::getMerkleRootM(merkle).to_string() + nonce;
		if ((!Utils::sha256(header).compare(hash)) && (hash.substr(0, difficulty) == target) && (counter == blockchain.size()))
		{
			spdlog::info("(Consensus???)Proof of idiot. Somehow it's a match.\nHash: {0}\n Difficulty {1:d};", hash.c_str(), difficulty);
			this->blockchain.push_back(Block(difficulty, counter, minedtime, prevHash, hash, nonce, merkle));
			return 0;
		}
		spdlog::error("Check hashing");
		return 1;
	}



	//our serialization here

	template<typename ...>
	std::string serialize()
	{
		json j;
		j.emplace("length", blockchain.size());
		for (int i = 0; i < this->blockchain.size(); i++)
		{
			j["data"][this->blockchain.at(i).getCounter()] = this->blockchain.at(i).serialize();
		}
		return j.dump(3);
	};

	template<typename T>
	Block getByParam(T param)
	{
		//
	}

public:
	~BlockChain() {};

};

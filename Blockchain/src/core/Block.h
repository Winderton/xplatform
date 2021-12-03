#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <memory>
#include <ctime>


#include "fort.h"
#include "fort.hpp"
#include "merkleTree.hpp"


#include "../json.h"

using json = nlohmann::json;


class Block
{
public:
	std::string minedTime;
private:
	uint64_t version = 0;
	int counter = 0;
	int difficulty = 0;
	merkle::Tree merkleHash;
	std::string prevHash;
	std::string hash;
	std::string nonce;
	std::vector<std::string> tx;//temp


public:
	Block() = default;

	Block(int difficulty, int counter, const std::string& minedTime,
		const std::string& prevHash, const std::string& hash,
		const std::string& nonce, std::vector<std::string>& tx)
		:
		difficulty(difficulty),
		minedTime(minedTime),
		prevHash(prevHash),
		tx(tx),
		counter(counter),
		nonce(nonce),
		hash(hash) {}
	{

	}


	inline std::string getPrevHash() const { return this->prevHash; }
	inline std::string getHash() const { return this->hash; }
	inline int getCounter() const { return this->counter; }
	inline std::vector<std::string> getTx() const { return this->tx; }
	inline int getNonce() const { return std::stoi(nonce); }


	static std::string getTime()
	{
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d | %X");
		return ss.str();
	}

	void SetMerkleHash(std::string message)
	{
		merkle::Tree::Hash hash(message);
		merkle::Tree tree;
		tree.insert(hash);
	}

	void SetNull()
	{
		version = 0;
		difficulty = 0;
		hash = "";
		prevHash = "";
		nonce = "";
	}

	void prettyPrint()
	{
		fort::char_table table;
		for (unsigned i = 0; i < tx.size(); i++)
		{
			table << fort::header
				<< "Block: " << counter << fort::endr
				<< "Time: " << this->minedTime.c_str() << fort::endr
				<< "Hash: " << this->hash.c_str() << fort::endr
				<< "Nonce: " << nonce.c_str() << fort::endr
				<< "Prev: " << this->prevHash.c_str() << fort::endr
				<< "Transaction: " << deserialize().c_str() << fort::endr;

			std::cout << table.to_string() << std::endl;
		}


	}

	//serializable toOurFormat(Block block);

	json serialize()
	{
		json j = { {"difficulty", this->difficulty}, {"counter", this->counter}, {"minedtime", this->minedTime}, {"previousHash",  this->prevHash}, {"hash", this->hash}, {"nonce", this->nonce}, {"tx", this->tx} };
		return j;
	}

	std::string deserialize()
	{
		//to our format to pretty print transactions
		std::string txString;
		for (int i = 0; i < tx.size(); i++) { txString += tx[i]; }
		return txString;
	}

	std::string jsonToString(const json& json)
	{
		return std::string(json.dump());
	}

};
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <chrono> 
#include <ctime>  
#include <sstream>
#include <iomanip>

#include "transaction.hpp"
#include <serialization.h>

#include "../json.hh"

#include "fort.hpp"
#include "Merkle.hpp"

using json = nlohmann::json;

using namespace ObjectModel;

class Block
{
public:
	std::string minedtime;
private:
	uint64_t version = 0;
	int counter = 0;
	int difficulty = 0;
	merkle::Tree merkleHash;
	std::string prevhash;
	std::string hash;
	std::string nonce;
	std::vector<std::string> data;

	std::vector<Transaction>* transactions = nullptr;
	//int counter = 1;
	//int magicnumber = 1;
public:
	Block() = default;

	Block(int difficulty, int counter, const std::string& minedtime, const std::string& prevHash, const std::string& hash, const std::string& nonce, const std::vector<std::string>& data)
		:
		difficulty(difficulty),
		minedtime(minedtime),
		prevhash(prevHash),
		data(data),
		counter(counter),
		nonce(nonce),
		hash(hash) {}

public:
	//FUTURE: internal init and control
	/*void init(const BlockProperties& blockData, std::string mtime)
	{
		blockProps.difficulty = blockData.difficulty;
		blockProps.counter = blockData.incounterdex;
		minedtime = mtime;
		blockProps.prevhash = blockData.prevhash;
		blockProps.hash = blockData.hash;
		blockProps.data = blockData.data;

	}*/

	inline std::string getPreviousHash() const { return this->prevhash; }
	inline std::string getHash() const { return this->hash; }
	inline int getCounter() const { return this->counter; }
	inline std::vector<std::string> getData() const { return this->data; };
	inline int getNonce() const { return std::stoi(nonce); }
	//not thread safe, may cause races
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
		prevhash = "";
		nonce = "";
	}

	void prettyPrint()
	{
		fort::char_table table;
		for (unsigned i = 0; i < data.size(); i++)
		{
			table << fort::header
				<< "Block: " << counter << fort::endr
				<< "Time: " << this->minedtime.c_str() << fort::endr
				<< "Hash: " << this->hash.c_str() << fort::endr
				<< "Nonce: " << nonce.c_str() << fort::endr
				<< "Prev: " << this->prevhash.c_str() << fort::endr
				<< "Transaction: " << deserialize().c_str() << fort::endr;

			std::cout << table.to_string() << std::endl;
		}


	}

	std::vector<int8_t> serializeToOurFormat()
	{
		Object object("data");
		std::unique_ptr<Primitive> difficulty = Primitive::create("difficulty", Type::I32, this->difficulty);
		std::unique_ptr<Primitive> counter = Primitive::create("counter", Type::I32, this->counter);
		std::unique_ptr<Array> minedTime = Array::createString("minedTime", Type::I8, this->minedtime);
		std::unique_ptr<Array> prevHash = Array::createString("prevHash", Type::I8, this->prevhash);
		std::unique_ptr<Array> hash = Array::createString("hash", Type::I8, this->hash);
		std::unique_ptr<Array> nonce = Array::createString("nonce", Type::I8, this->nonce);
		std::unique_ptr<Array> tx = Array::createArray("data", Type::I8, this->data);
		object.addEntity(difficulty.get());
		object.addEntity(counter.get());
		object.addEntity(minedTime.get());
		object.addEntity(hash.get());
		object.addEntity(nonce.get());
		object.addEntity(tx.get());

		Core::Util::retriveNsave(&object);

	}

	void deserializeFromOurFormat()
	{
		std::vector<uint8_t> objectFromFile = Core::Util::load("data.abc");
		int16_t it = 0;
		Object toPrintObject = Object::unpack(objectFromFile, it);
	}

	json serialize()
	{
		json j = { {"difficulty", this->difficulty}, {"counter", this->counter}, {"minedtime", this->minedtime},
		{"previousHash",  this->prevhash}, {"hash", this->hash}, {"nonce", this->nonce}, {"data", this->data} };
		return j;
	}

	std::string deserialize()
	{
		//to our format to pretty print transactions
		std::string dataString;
		for (int i = 0; i < data.size(); i++) { dataString += data[i]; }
		return dataString;
	}

	std::string jsonToString(const json& json)
	{
		return std::string(json.dump());
	}

};


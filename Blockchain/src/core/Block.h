#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <memory>
#include <ctime>

#include <serialization.h>

#include "fort.h"
#include "fort.hpp"
#include "merkleTree.hpp"


#include "../json.h"

using json = nlohmann::json;
using namespace ObjectModel;

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

	/*

	void testPack()
	{
		int32_t foo = 231;
		std::unique_ptr<Primitive> p = Primitive::create("int32", Type::I32, foo);

		Object Test("Foo");
		Test.addEntity(p.get());

		Core::Util::retriveNsave(&Test);
	}

	void testUnpack()
	{
		std::vector<uint8_t> objectFromFile = Core::Util::load("Foo.abc");

		[[maybe_unused]]int16_t it = 0;
		Object toPrintObject = Object::unpack(objectFromFile, it);

		[[maybe_unused]]int16_t it2 = 0;
		int w = Core::decode<int32_t>(toPrintObject.findPrimitiveByName("int32").getData(), it2);
		std::cout << w << std::endl;
	}

	*/
	
	std::vector<int8_t> serializeToOurFormat()
	{
		Object object("data");
		std::unique_ptr<Primitive> difficulty = Primitive::create("difficulty", Type::I32, this->difficulty);
		std::unique_ptr<Primitive> counter = Primitive::create("counter", Type::I32, this->counter);
		std::unique_ptr<Array> minedTime = Array::createString("minedTime", Type::I8, this->minedTime);
		std::unique_ptr<Array> prevHash = Array::createString("prevHash", Type::I8, this->prevHash);
		std::unique_ptr<Array> hash = Array::createString("hash", Type::I8, this->hash);
		std::unique_ptr<Array> nonce = Array::createString("nonce", Type::I8, this->nonce);
		std::unique_ptr<Array> tx = Array::createArray("tx", Type::I8, this->tx);
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
		json j = { {"difficulty", this->difficulty}, {"counter", this->counter},
		{"minedtime", this->minedTime}, {"previousHash",  this->prevHash},
		{"hash", this->hash}, {"nonce", this->nonce}, {"tx", this->tx} };
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
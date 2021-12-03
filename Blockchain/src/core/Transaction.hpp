#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "../json.hh"
#include <openssl/lhash.h>


class Transaction
{
public:
	//bitcoin
	static constexpr uint32_t INDEX_NULL = std::numeric_limits<uint32_t>::max();
	int16_t version = 0;
private:
	float amount;
	std::unique_ptr<std::string> in;
	std::unique_ptr<std::string> out;
	//bitcoin and etherium do 256bit hashing
	uint64_t hash;

public:
	Transaction() = default;

	Transaction(const Transaction&) = default;
	Transaction& operator=(const Transaction&) = default;

	Transaction(float amount, std::unique_ptr<std::string> out, std::unique_ptr<std::string> in)
		:
		amount(amount),
		in(std::move(in)),
		out(std::move(out)) {}


public:
	uint64_t calculateHash() const {};
	const uint64_t getHash() const { return hash; }

	/*
	How to serialize?
	I'll probably do my serialization format here as well
	- int16_t version
	- float amount;
	Actually, there is gonna be a vector of pointers of ins and outs
	- std::unique_ptr<std::string> in
	- std::unique_ptr<std::string> out
	- std::string time
	*/

};
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>


#include <sstream>
#include <iomanip>

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/ripemd.h>

#include <chrono>
#include <thread>
#include "spdlog/spdlog.h"
#include "Merkle.hpp"

namespace Utils
{
	//struct _KEYPAIR;
	//void ECOH(KEYPAIR* current, EC_GROUP* ec_group);
	std::string md5(const std::string msg);
	std::string sha256(const std::string str);
	//std::string hextoascii(const std::string& hex);
	std::string merkleMe(const std::string& s1);
	std::string merkleMeAgain(const std::string& s1, const std::string& s2);
	std::string getMerkleRoot(std::vector<std::string>& txs);
	merkle::Hash getMerkleRootM(std::vector<std::string>& txs);
	std::pair<std::string, std::string> findHash(int dificulty, int index, std::string prevHash, std::vector<std::string>& merkle);


}
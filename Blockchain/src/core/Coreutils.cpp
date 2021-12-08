#pragma once

#include "Coreutils.hpp"
//ec is digital signature so need to work out how to make Elliptic curve only hash thang



namespace Utils
{
	/*
	typedef struct _KEYPAIR {
		BIGNUM* x, * y;
		BIGNUM* d;
		EC_POINT* pub_key;
		EC_KEY* hash;
	} KEYPAIR;

	void ECOH(KEYPAIR* current, EC_GROUP* ec_group) {

		current->hash = EC_KEY_new();
		EC_KEY_set_group(current->hash, ec_group);
		EC_KEY_generate_key(current->hash);

		current->d = const_cast<BIGNUM*>(EC_KEY_get0_private_key(current->hash));
		printf("d: (%s)\n", BN_bn2hex(current->d));

		current->pub_key = const_cast<EC_POINT*>(EC_KEY_get0_public_key(current->hash));
		current->x = BN_new();
		current->y = BN_new();
	}
	*/

	std::string md5(const std::string msg)
	{
		unsigned char hash[MD5_DIGEST_LENGTH];
		MD5_CTX md5;
		MD5_Init(&md5);
		MD5_Update(&md5, msg.c_str(), msg.length());
		MD5_Final(hash, &md5);
		std::stringstream ss;
		for (unsigned i = 0; i < MD5_DIGEST_LENGTH; i++)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
		}
		return ss.str();

	}

	std::string sha256(const std::string str)
	{
		unsigned char hash[SHA256_DIGEST_LENGTH];
		SHA256_CTX sha256;
		SHA256_Init(&sha256);
		SHA256_Update(&sha256, str.c_str(), str.size());
		SHA256_Final(hash, &sha256);

		std::stringstream ss;
		for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
		}

		return ss.str();
	}

#ifdef _CUSTOM_HASH_TREE
	std::string reverse(std::string);
	std::string print_hash_as_hex(std::string);
	std::string hasher(std::string& first, std::string& second)
	{
		//NOTE:try to avoid accumulator expression
		firstReverse = reverse(hex_to_ascii(first));
		secondReverse = reverse(hex_to_ascii(second));
		string concat = firstReverse + secondReverse;
		std::string reverse_SHA = reverse(sha256(sha256(concat)));

		return print_hash_as_hex(reverse_SHA);
	}
#endif




	merkle::Hash getMerkleRootM(std::vector<std::string>& txs)
	{
		std::vector<merkle::Tree::Hash> hashes(txs.size());
		merkle::Tree mt;
		for (unsigned i = 0; i < hashes.size(); i++)
		{
			mt.insert(sha256(txs[i]));
		}
		merkle::Tree::Hash root = mt.root();
		return root;
	}


	std::string merkleMe(const std::string& s1)
	{
		return sha256(s1);
	}


	std::string merkleMeAgain(const std::string& s1, const std::string& s2)
	{
		return sha256(sha256(s1) + sha256(s2));
	}


	std::string getMerkleRoot(std::vector<std::string>& merkle)
	{
		bool mutation = false;
#ifdef _CUSTOM_HASH_TREE

		ROUND = ROUND + 1;

		if (txHashes.size() == 1) {
			cout << "CALCULATED MERKLE ROOT IS: ";
			cout << txHashes[0] << endl;
			return txHashes;
		}

#endif
		if (merkle.size() == 1) { return sha256(merkle[0]); }
#ifdef _CUSTOM_HASH_TREE
		std::vector <std::string> temp;
#endif
		std::vector<std::string> new_merkle = merkle;

#ifdef _CUSTOM_HASH_TREE_T_HASHER
		for (int i = 0; i < txHashes.size() - 1; i = i + 2)
		{
			temp.push_back(hasher(txHashes[i], txHashes[i + 1]));
		}
#endif
		while (new_merkle.size() > 1) {
			if (new_merkle.size() % 2 == 1)
				new_merkle.push_back(merkle.back());

			std::vector<std::string> result;

			for (int i = 0; i < new_merkle.size(); i += 2) {
				result.push_back(sha256(new_merkle[i]) + sha256(new_merkle[i++]));
			}
			new_merkle = result;
		}
		return new_merkle[0];
#ifdef _CUSTOM_HASH_TREE
		if (txHashes.size() % 2 == 1)
		{
			temp.push_back(hasher(txHashes[txHashes.size() - 1], txHashes[txHashes.size() - 1]));
		}
		return merkle(temp);
#endif
	}




	std::pair<std::string, std::string> findHash(int dificulty, int index, std::string prevHash, std::vector<std::string>& txs)
	{
		std::string target = "";
		for (unsigned i = 0; i < dificulty; i++)
		{
			target += std::to_string(0);
		}
		std::string header = std::to_string(index) + prevHash + getMerkleRootM(txs).to_string();
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 384993400; i >= 0; i--)
		{
			std::string blockHash = sha256(header + std::to_string(i));
			std::cout << "Mining..." << blockHash << "\r";
			if (blockHash.substr(0, dificulty) == target)
			{
				std::cout << std::endl;
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double, std::milli> elapsed = finish - start;
				spdlog::info("Success!");
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				std::cout << "Elapsed Time: ~" << (elapsed.count() / 1000) << " seconds" << std::endl;
				std::cout << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				return std::make_pair(blockHash, std::to_string(i));
			}
		}
		spdlog::error("Mining failed");
		return std::make_pair("", "");
	}

}
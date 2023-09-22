#pragma once

#include <vector>
#include <string>
#include <gmp.h>
#include <iostream>
#include "cryptoTools/Common/Defines.h"
#include "cryptoTools/Network/IOService.h"
#include "cryptoTools/Common/BitVector.h"
#include "cryptoTools/Common/Timer.h"
#include "cryptoTools/Network/Channel.h"
#include "benes.h"

using namespace osuCrypto;
using namespace std;

class OSNSender
{
	size_t size;
	int ot_type;
	mpz_t p;

	IOService ios;
	Session session;
	vector<Channel> chls;
	bool allow_print_intermediate_value = false;

	oc::Timer *timer;

	Benes benes;
	void silent_ot_recv(osuCrypto::BitVector &choices,
						std::vector<osuCrypto::block> &recvMsg,
						std::vector<oc::Channel> &chls);
	void rand_ot_recv(osuCrypto::BitVector &choices,
					  std::vector<osuCrypto::block> &recvMsg,
					  std::vector<oc::Channel> &chls);
	std::vector<std::array<osuCrypto::block, 2>> gen_benes_server_osn(int values, std::vector<oc::Channel> &chls);

	template <typename T>
	void print_intermediate_vector(T &value, std::string name);
	template <typename T>
	void print_intermediate_value(T value, std::string name);

public:
	std::vector<int> dest;
	OSNSender();
	void init(size_t size, std::vector<int> &dest, std::vector<uint64_t> &p, int ot_type = 0, string Sip = "127.0.0.1:12345", size_t num_threads = 1);
	std::vector<std::vector<uint64_t>> run_osn();
	void setTimer(oc::Timer &timer);
};

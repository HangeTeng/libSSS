#pragma once

#include <vector>
#include "cryptoTools/Network/IOService.h"
#include "cryptoTools/Common/Defines.h"
#include "cryptoTools/Common/Timer.h"
#include "cryptoTools/Network/Channel.h"
#include <atomic>
#include <gmp.h>
#include <iostream>


using namespace osuCrypto;
using namespace std;

class OSNReceiver
{
	size_t size;
	int ot_type;
	mpz_t p;

	IOService ios;
	Session session;
	vector<Channel> chls;

	bool allow_print_intermediate_value = false;

	oc::Timer *timer;
	std::atomic<int> cpus;

	void rand_ot_send(std::vector<std::array<osuCrypto::block, 2>> &sendMsg, std::vector<oc::Channel> &chls);
	void silent_ot_send(std::vector<std::array<osuCrypto::block, 2>> &sendMsg, std::vector<oc::Channel> &chls);

	std::vector<std::vector<oc::block>> gen_benes_client_osn(int values, std::vector<oc::Channel> &chls);
	void prepare_correction(int n, int Val, int lvl_p, int perm_idx, std::vector<oc::block> &src,
							std::vector<std::array<std::array<osuCrypto::block, 2>, 2>> &ot_output,
							std::vector<std::array<osuCrypto::block, 2>> &correction_blocks);

	template <typename T>
	void print_intermediate_vector(T &value, std::string name);
	template <typename T>
	void print_intermediate_value(T value, std::string name);

public:
	OSNReceiver();
	void init(size_t size, std::vector<uint64_t> &p, int ot_type = 1, string Sip = "127.0.0.1:12345", size_t num_threads = 1);
	std::pair<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>>> run_osn();
	void setTimer(oc::Timer &timer);
};

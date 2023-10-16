#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <gmp.h>

#include "cryptoTools/Network/IOService.h"

#include "OSNReceiver.h"
#include "OSNSender.h"
#include "op.h"

using namespace osuCrypto;
using namespace std;

// 声明全局变量
// vector<block> sender_shares;
// vector<block> receiver_shares;
// vector<block> receiver_set;
vector<vector<uint64_t>> sender_shares;
vector<vector<uint64_t>> receiver_shares;
vector<vector<uint64_t>> receiver_set;
vector<int> permutation;
string ip = "127.0.0.1:12345";
vector<uint64_t> p_array = {1ul, 1ul};

void sender(size_t size, size_t num_threads)
{

	std::vector<int> dest(size);
	for (int i = 0; i < dest.size(); ++i)
		dest[i] = i;

	// osuCrypto::PRNG prng(_mm_set_epi32(425323, 334565, 0, 235)); // we need to modify this seed

	// for (int i = size - 1; i > 0; i--)
	// {
	// 	int loc = prng.get<uint64_t>() % (i + 1);
	// 	std::swap(dest[i], dest[loc]);
	// }

	OSNSender osn;
	osn.init(size, dest, p_array, 1, ip, num_threads);
	Timer timer;
	osn.setTimer(timer);
	timer.setTimePoint("before run_osn");

	sender_shares = osn.run_osn();
	timer.setTimePoint("after run_osn");
	permutation = osn.dest;
	cout << IoStream::lock;
	cout << "Sender:" << endl;
	cout << timer << endl;
	// size_t sent = 0, recv = 0;
	// for (auto &chl : osn.chls)
	// {
	// 	sent += chl.getTotalDataSent();
	// 	recv += chl.getTotalDataRecv();
	// }
	// cout << "recv: " << recv / 1024.0 / 1024.0 << "MB sent:" << sent / 1024.0 / 1024.0 << "MB "
	// 	 << "total: " << (recv + sent) / 1024.0 / 1024.0 << "MB" << endl;
	cout << IoStream::unlock;
}

void receiver(size_t size,size_t num_threads)
{

	OSNReceiver osn;
	osn.init(size, p_array, 1, ip, num_threads);
	Timer timer;
	osn.setTimer(timer);
	timer.setTimePoint("before run_osn");

	std::pair<std::vector<vector<uint64_t>>, std::vector<vector<uint64_t>>> result = osn.run_osn();
	receiver_set = result.first;
	receiver_shares = result.second;
	timer.setTimePoint("after run_osn");
	cout << IoStream::lock;
	cout << "Receiver:" << endl;
	cout << timer << endl;
	// size_t sent = 0, recv = 0;
	// for (auto &chl : osn.chls)
	// {
	// 	sent += chl.getTotalDataSent();
	// 	recv += chl.getTotalDataRecv();
	// }
	// cout << "recv: " << recv / 1024.0 / 1024.0 << "MB sent:" << sent / 1024.0 / 1024.0 << "MB "
	// 	 << "total: " << (recv + sent) / 1024.0 / 1024.0 << "MB" << endl;
	cout << IoStream::unlock;
}

int check_result(size_t size)
{
	int correct_cnt = 0;
	mpz_t tmp, a, b, c, p;
	mpz_init(tmp);
	mpz_init(a);
	mpz_init(b);
	mpz_init(c);
	mpz_init(p);
	mpz_import(p, p_array.size(), -1, sizeof(uint64_t), 0, 0, p_array.data());

	for (auto i = 0; i < size; i++)
	{
		mpz_import(a, sender_shares[i].size(), -1, sizeof(uint64_t), 0, 0, sender_shares[i].data());
		mpz_import(b, receiver_shares[i].size(), -1, sizeof(uint64_t), 0, 0, receiver_shares[i].data());
		mpz_import(c, receiver_set[permutation[i]].size(), -1, sizeof(uint64_t), 0, 0, receiver_set[permutation[i]].data());
		// gmp_printf("a = %Zd\nb = %Zd\np = %Zd\nc = %Zd\n", a, b, p, c);
		mpz_add(tmp, a, b);
		// gmp_printf("tmp = %Zd\n", tmp);
		mpz_mod(tmp, tmp, p);
		// gmp_printf("tmp = %Zd\n", tmp);
		if (0 == mpz_cmp(tmp, c))
		{
			correct_cnt++;
		}
	}
	return correct_cnt;
}

int main(int argc, char **argv)
{
	size_t size = 1 << atoi(argv[1]);
	size_t num_threads = atoi(argv[2]);
	cout << "size:" << size << " num_threads:" << num_threads << endl;

	auto sender_thrd = thread(sender, size, num_threads);
	auto receiver_thrd = thread(receiver, size, num_threads);
	sender_thrd.join();
	receiver_thrd.join();
	if (size == check_result(size))
		cout << "Correct!" << endl;
	else
		cout << "Wrong!" << endl;

	return 0;
}

#include "OSNReceiver.h"
#include "libOTe/Base/BaseOT.h"
#include "cryptoTools/Common/BitVector.h"
#include "op.h"
#include <cryptoTools/Crypto/AES.h>
#include <libOTe/TwoChooseOne/SilentOtExtSender.h>
#include <libOTe/TwoChooseOne/IknpOtExtSender.h>
#include <cstring>
#include <iostream>
using namespace std;
using namespace oc;
void OSNReceiver::rand_ot_send(std::vector<std::array<osuCrypto::block, 2>> &sendMsg, std::vector<oc::Channel> &chls)
{
	// std::cout << "\n OT sender!! \n";
	size_t num_threads = chls.size();

	size_t total_len = sendMsg.size();
	auto routine = [&](size_t tid)
	{
		size_t start_idx = total_len * tid / num_threads;
		size_t end_idx = total_len * (tid + 1) / num_threads;
		end_idx = ((end_idx <= total_len) ? end_idx : total_len);
		size_t size = end_idx - start_idx;

		osuCrypto::PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));

		std::vector<osuCrypto::block> baseRecv(128);
		osuCrypto::DefaultBaseOT baseOTs;
		osuCrypto::BitVector baseChoice(128);
		baseChoice.randomize(prng1);
		osuCrypto::IknpOtExtSender sender;
		baseOTs.receive(baseChoice, baseRecv, prng1, chls[tid], 1);
		sender.setBaseOts(baseRecv, baseChoice);

		std::vector<std::array<osuCrypto::block, 2>> tmpMsg(size);
		sender.send(tmpMsg, prng1, chls[tid]);
		std::copy_n(tmpMsg.begin(), size, sendMsg.begin() + start_idx);
	};
	vector<thread> thrds(num_threads);
	for (size_t t = 0; t < num_threads; t++)
		thrds[t] = std::thread(routine, t);
	for (size_t t = 0; t < num_threads; t++)
		thrds[t].join();

	/*osuCrypto::PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));

	std::vector<osuCrypto::block> baseRecv(128);
	osuCrypto::DefaultBaseOT baseOTs;
	osuCrypto::BitVector baseChoice(128);
	baseChoice.randomize(prng1);
	osuCrypto::IknpOtExtSender sender;
	baseOTs.receive(baseChoice, baseRecv, prng1, chls[0], num_threads);
	sender.setBaseOts(baseRecv, baseChoice);
	sender.send(sendMsg, prng1, chls[0]);*/
}

void OSNReceiver::silent_ot_send(std::vector<std::array<osuCrypto::block, 2>> &sendMsg, std::vector<oc::Channel> &chls)
{
	// std::cout << "\n Silent OT sender!! \n";

	size_t num_threads = chls.size();
	size_t total_len = sendMsg.size();
	auto routine = [&](size_t tid)
	{
		size_t start_idx = total_len * tid / num_threads;
		size_t end_idx = total_len * (tid + 1) / num_threads;
		end_idx = ((end_idx <= total_len) ? end_idx : total_len);
		size_t size = end_idx - start_idx;

		osuCrypto::PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));
		osuCrypto::u64 numOTs = size;

		osuCrypto::SilentOtExtSender sender;
		sender.configure(numOTs);
		std::vector<std::array<osuCrypto::block, 2>> tmpMsg(size);
		sender.silentSend(tmpMsg, prng1, chls[tid]);
		std::copy_n(tmpMsg.begin(), size, sendMsg.begin() + start_idx);
	};

	vector<thread> thrds(num_threads);
	for (size_t t = 0; t < num_threads; t++)
		thrds[t] = std::thread(routine, t);
	for (size_t t = 0; t < num_threads; t++)
		thrds[t].join();

	/*osuCrypto::PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));
	osuCrypto::u64 numOTs = sendMsg.size();

	osuCrypto::SilentOtExtSender sender;
	sender.configure(numOTs, 2, num_threads);
	sender.silentSend(sendMsg, prng1, chls[0]);*/
}

std::vector<std::vector<block>> OSNReceiver::gen_benes_client_osn(int values, std::vector<oc::Channel> &chls)
{
	int N = int(ceil(log2(values)));

	int levels = 2 * N - 1;
	int switches = levels * (values / 2);
	block temp;
	std::vector<block> masks(values);
	std::vector<std::vector<block>> ret_masks(values);

	osuCrypto::PRNG prng(_mm_set_epi32(4253233465, 334565, 0, 235));

	for (int j = 0; j < values; j++)
	{ // we sample the input masks randomly
		temp = prng.get<block>();
		temp = temp;
		// temp = oc::block(0, (j + 3) << (16 * 1 + 4));
		masks[j] = temp;
		ret_masks[j].push_back(temp);
	}

	// //! test print
	// cout << IoStream::lock;
	// cout << "osnrecv get masks: " << endl;
	// for (int i = 0; i < values; ++i)
	// {
	// 	cout << i << " " << masks[i] << endl;
	// }
	// cout << IoStream::unlock;

	// //! test_print
	// cout << IoStream::lock;
	// cout << "osnrecv get ret_masks: " << endl;
	// for (int i = 0; i < values; ++i)
	// {
	// 	cout << i << " " << ret_masks[i][0] << endl;
	// }
	// cout << IoStream::unlock;

	std::vector<std::array<std::array<osuCrypto::block, 2>, 2>> ot_messages(switches);
	Channel &chl = chls[0];
	if (ot_type == 0)
	{
		std::vector<std::array<osuCrypto::block, 2>> tmp_messages(switches);
		osuCrypto::BitVector bit_correction(switches);
		silent_ot_send(tmp_messages, chls); // sample random ot blocks

		chl.recv(bit_correction);
		osuCrypto::block tmp;
		for (int k = 0; k < tmp_messages.size(); k++)
		{
			if (bit_correction[k] == 1)
			{
				tmp = tmp_messages[k][0];
				tmp_messages[k][0] = tmp_messages[k][1];
				tmp_messages[k][1] = tmp;
			}
		}

		AES aes(ZeroBlock);

		for (auto i = 0; i < ot_messages.size(); i++)
		{
			ot_messages[i][0] = {tmp_messages[i][0], aes.ecbEncBlock(tmp_messages[i][0])};
			ot_messages[i][1] = {tmp_messages[i][1], aes.ecbEncBlock(tmp_messages[i][1])};
		}
	}
	else
	{
		std::vector<std::array<osuCrypto::block, 2>> tmp_messages(switches);
		//! 1 out 2 ot
		//! recver have 2 random and sender chose 1
		rand_ot_send(tmp_messages, chls); // sample random ot blocks

		//! test tmp
		for (size_t i = 0; i < tmp_messages.size(); ++i)
		{
			tmp_messages[i][0] = tmp_messages[i][0];
			tmp_messages[i][1] = tmp_messages[i][1];
		}

		// //! test print
		// cout << IoStream::lock;
		// cout << "recver tmp_messages: " << endl;
		// for (size_t i = 0; i < tmp_messages.size(); ++i)
		// {
		// 	cout << i << " " << tmp_messages[i][0] << " " << tmp_messages[i][1] << endl;
		// }
		// cout << IoStream::unlock;

		AES aes(ZeroBlock);
		for (auto i = 0; i < ot_messages.size(); i++)
		{
			ot_messages[i][0] = {tmp_messages[i][0], aes.ecbEncBlock(tmp_messages[i][0])};
			ot_messages[i][1] = {tmp_messages[i][1], aes.ecbEncBlock(tmp_messages[i][1])};
		}
	}

	cpus.store(chls.size());
	std::vector<std::array<osuCrypto::block, 2>> correction_blocks(switches);
	prepare_correction(N, values, 0, 0, masks, ot_messages, correction_blocks);

	chl.send(correction_blocks);
	for (int i = 0; i < values; ++i)
	{
		ret_masks[i].push_back(masks[i]);
	}
	return ret_masks;
}

OSNReceiver::OSNReceiver(size_t size, int ot_type) : size(size), ot_type(ot_type)
{
}

void OSNReceiver::init(size_t size, std::vector<uint64_t> &p, int ot_type,string Sip, size_t num_threads)
{
	this->size = size;
	this->ot_type = ot_type;
	mpz_init(this->p);
	mpz_import(this->p, p.size(), -1, sizeof(uint64_t), 0, 0, p.data());
	
	this->session.start(this->ios, Sip, EpMode::Client);
	for (size_t i = 0; i < num_threads; i++)
	{
		this->chls.emplace_back(this->session.addChannel());
	}

}

std::pair<std::vector<std::vector<uint64_t>>, std::vector<std::vector<uint64_t>>> OSNReceiver::run_osn(/*std::vector<oc::block> inputs,*/)
{
	int values = size;
	std::vector<std::vector<block>> ret_masks = gen_benes_client_osn(values, chls);
	std::vector<block> output_masks, benes_input;
	std::vector<std::vector<uint64_t>> inputs_ul(values), output_masks_ul(values);

	mpz_t tmp[2];
	mpz_inits(tmp[0], tmp[1], NULL);

	osuCrypto::PRNG prng(_mm_set_epi32(4253233465, 334565, 0, 235)); // we need to modify this seed

	std::vector<oc::block> inputs(values); //! set a random input, could be a receiver data input
	for (int i = 0; i < inputs.size(); ++i)
	{
		
		inputs[i] = prng.get<oc::block>();
		block2mpz(inputs[i], tmp[0]);
		mpz_mod(tmp[0], tmp[0], p);
		inputs[i] = mpz2block(tmp[0]);
		// inputs[i] = oc::toBlock((i+1));
		// inputs[i] = oc::block(0,i<<4);
		// inputs[i] = oc::block(0, prng.get<uint64_t>());
	}

	
	

	for (int i = 0; i < values; ++i)
	{
		block2mpz(inputs[i], tmp[0]);
		block2mpz(ret_masks[i][0], tmp[1]);
		mpz_sub(tmp[0], tmp[0], tmp[1]);
		mpz_mod(tmp[0], tmp[0], p);
		ret_masks[i][0] = mpz2block(tmp[0]);
	}
	for (int i = 0; i < values; ++i)
		benes_input.push_back(ret_masks[i][0]);

	chls[0].send(benes_input); //! recver comumincate and send
	for (int i = 0; i < values; ++i)
		output_masks.push_back(ret_masks[i][1]);

	for (int i = 0; i < values; ++i)
	{
		for (uint64_t x : inputs[i].as<uint64_t>())
		{
			// printf("-------%lu\n", x);
			inputs_ul[i].push_back(x);
		}
		for (uint64_t x : output_masks[i].as<uint64_t>())
			output_masks_ul[i].push_back(x);
	}

	return std::make_pair(inputs_ul, output_masks_ul);
}

void OSNReceiver::setTimer(Timer &timer)
{
	this->timer = &timer;
}

void OSNReceiver::prepare_correction(int n, int Val, int lvl_p, int perm_idx, std::vector<oc::block> &src,
									 std::vector<std::array<std::array<osuCrypto::block, 2>, 2>> &ot_output,
									 std::vector<std::array<osuCrypto::block, 2>> &correction_blocks)
{
	//! in gen : prepare_correction(N, values, 0, 0, masks, ot_messages, correction_blocks);
	//! masks = src
	// ot message M0 = m0 ^ w0 || m1 ^ w1
	//  for each switch: top wire m0 w0 - bottom wires m1, w1
	//  M1 = m0 ^ w1 || m1 ^ w0
	int levels = 2 * n - 1, base_idx;
	int values = src.size();

	std::vector<block> bottom1;
	std::vector<block> top1;

	mpz_t m0, m1, w0, w1, M0[2], M1[2], corr_mesg[2], tmp;
	mpz_inits(m0, m1, w0, w1, M0[0], M0[1], M1[0], M1[1], corr_mesg[0], corr_mesg[1], tmp, NULL);
	std::array<oc::block, 2> corr_block, temp_block;

	if (values == 2)
	{
		if (n == 1)
		{
			base_idx = lvl_p * (Val / 2) + perm_idx;

			// m0 = src[0];
			block2mpz(src[0], m0);
			// m1 = src[1];
			block2mpz(src[1], m1);

			temp_block = ot_output[base_idx][0];
			// memcpy(M0, temp_block.data(), sizeof(M0));
			block2mpz(temp_block[0], M0[0]);
			block2mpz(temp_block[1], M0[1]);

			// w0 = M0[0] + m0
			mpz_add(tmp, M0[0], m0);
			mpz_mod(w0, tmp, p);

			// w1 = M0[1] + m1
			mpz_add(tmp, M0[1], m1);
			mpz_mod(w1, tmp, p);

			temp_block = ot_output[base_idx][1];
			// memcpy(M1, temp_block.data(), sizeof(M1));
			block2mpz(temp_block[0], M1[0]);
			block2mpz(temp_block[1], M1[1]);

			// corr_mesg[0] = M1[0] - m0 + w1;
			mpz_sub(tmp, M1[0], m0);
			mpz_add(tmp, tmp, w1);
			mpz_mod(corr_mesg[0], tmp, p);

			// corr_mesg[1] = M1[1] - m1 + w0;
			mpz_sub(tmp, M1[1], m1);
			mpz_add(tmp, tmp, w0);
			mpz_mod(corr_mesg[1], tmp, p);

			// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
			correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

			// M1[0] = m0 - w1;
			mpz_sub(tmp, m0, w1);
			mpz_mod(M1[0], tmp, p);

			// M1[1] = m1 - w0;
			mpz_sub(tmp, m1, w0);
			mpz_mod(M1[1], tmp, p);

			ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
			src[0] = mpz2block(w0);
			src[1] = mpz2block(w1);
		}
		else
		{
			base_idx = (lvl_p + 1) * (Val / 2) + perm_idx;
			// m0 = src[0];
			block2mpz(src[0], m0);
			// m1 = src[1];
			block2mpz(src[1], m1);

			temp_block = ot_output[base_idx][0];
			// memcpy(M0, temp_block.data(), sizeof(M0));
			block2mpz(temp_block[0], M0[0]);
			block2mpz(temp_block[1], M0[1]);

			// w0 = M0[0] + m0
			mpz_add(tmp, M0[0], m0);
			mpz_mod(w0, tmp, p);

			// w1 = M0[1] + m1
			mpz_add(tmp, M0[1], m1);
			mpz_mod(w1, tmp, p);

			temp_block = ot_output[base_idx][1];
			// memcpy(M1, temp_block.data(), sizeof(M1));
			block2mpz(temp_block[0], M1[0]);
			block2mpz(temp_block[1], M1[1]);

			// corr_mesg[0] = M1[0] - m0 + w1;
			mpz_sub(tmp, M1[0], m0);
			mpz_add(tmp, tmp, w1);
			mpz_mod(corr_mesg[0], tmp, p);

			// corr_mesg[1] = M1[1] - m1 + w0;
			mpz_sub(tmp, M1[1], m1);
			mpz_add(tmp, tmp, w0);
			mpz_mod(corr_mesg[1], tmp, p);

			// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
			correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

			// M1[0] = m0 - w1;
			mpz_sub(tmp, m0, w1);
			mpz_mod(M1[0], tmp, p);

			// M1[1] = m1 - w0;
			mpz_sub(tmp, m1, w0);
			mpz_mod(M1[1], tmp, p);

			ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
			src[0] = mpz2block(w0);
			src[1] = mpz2block(w1);
		}
		return;
	}

	if (values == 3)
	{
		base_idx = lvl_p * (Val / 2) + perm_idx;
		// m0 = src[0];
		block2mpz(src[0], m0);
		// m1 = src[1];
		block2mpz(src[1], m1);

		temp_block = ot_output[base_idx][0];
		// memcpy(M0, temp_block.data(), sizeof(M0));
		block2mpz(temp_block[0], M0[0]);
		block2mpz(temp_block[1], M0[1]);

		// w0 = M0[0] + m0
		mpz_add(tmp, M0[0], m0);
		mpz_mod(w0, tmp, p);

		// w1 = M0[1] + m1
		mpz_add(tmp, M0[1], m1);
		mpz_mod(w1, tmp, p);

		temp_block = ot_output[base_idx][1];
		// memcpy(M1, temp_block.data(), sizeof(M1));
		block2mpz(temp_block[0], M1[0]);
		block2mpz(temp_block[1], M1[1]);

		// corr_mesg[0] = M1[0] - m0 + w1;
		mpz_sub(tmp, M1[0], m0);
		mpz_add(tmp, tmp, w1);
		mpz_mod(corr_mesg[0], tmp, p);

		// corr_mesg[1] = M1[1] - m1 + w0;
		mpz_sub(tmp, M1[1], m1);
		mpz_add(tmp, tmp, w0);
		mpz_mod(corr_mesg[1], tmp, p);

		// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
		correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

		// M1[0] = m0 - w1;
		mpz_sub(tmp, m0, w1);
		mpz_mod(M1[0], tmp, p);

		// M1[1] = m1 - w0;
		mpz_sub(tmp, m1, w0);
		mpz_mod(M1[1], tmp, p);

		ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
		src[0] = mpz2block(w0);
		src[1] = mpz2block(w1);

		base_idx = (lvl_p + 1) * (Val / 2) + perm_idx;
		// m0 = src[1];
		block2mpz(src[1], m0);
		// m1 = src[2];
		block2mpz(src[2], m1);

		temp_block = ot_output[base_idx][0];
		// memcpy(M0, temp_block.data(), sizeof(M0));
		block2mpz(temp_block[0], M0[0]);
		block2mpz(temp_block[1], M0[1]);

		// w0 = M0[0] + m0
		mpz_add(tmp, M0[0], m0);
		mpz_mod(w0, tmp, p);

		// w1 = M0[1] + m1
		mpz_add(tmp, M0[1], m1);
		mpz_mod(w1, tmp, p);

		temp_block = ot_output[base_idx][1];
		// memcpy(M1, temp_block.data(), sizeof(M1));
		block2mpz(temp_block[0], M1[0]);
		block2mpz(temp_block[1], M1[1]);

		// corr_mesg[0] = M1[0] - m0 + w1;
		mpz_sub(tmp, M1[0], m0);
		mpz_add(tmp, tmp, w1);
		mpz_mod(corr_mesg[0], tmp, p);

		// corr_mesg[1] = M1[1] - m1 + w0;
		mpz_sub(tmp, M1[1], m1);
		mpz_add(tmp, tmp, w0);
		mpz_mod(corr_mesg[1], tmp, p);

		// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
		correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

		// M1[0] = m0 - w1;
		mpz_sub(tmp, m0, w1);
		mpz_mod(M1[0], tmp, p);

		// M1[1] = m1 - w0;
		mpz_sub(tmp, m1, w0);
		mpz_mod(M1[1], tmp, p);

		ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
		src[1] = mpz2block(w0);
		src[2] = mpz2block(w1);

		base_idx = (lvl_p + 2) * (Val / 2) + perm_idx;
		// m0 = src[0];
		block2mpz(src[0], m0);
		// m1 = src[1];
		block2mpz(src[1], m1);

		temp_block = ot_output[base_idx][0];
		// memcpy(M0, temp_block.data(), sizeof(M0));
		block2mpz(temp_block[0], M0[0]);
		block2mpz(temp_block[1], M0[1]);

		// w0 = M0[0] + m0
		mpz_add(tmp, M0[0], m0);
		mpz_mod(w0, tmp, p);

		// w1 = M0[1] + m1
		mpz_add(tmp, M0[1], m1);
		mpz_mod(w1, tmp, p);

		temp_block = ot_output[base_idx][1];
		// memcpy(M1, temp_block.data(), sizeof(M1));
		block2mpz(temp_block[0], M1[0]);
		block2mpz(temp_block[1], M1[1]);

		// corr_mesg[0] = M1[0] - m0 + w1;
		mpz_sub(tmp, M1[0], m0);
		mpz_add(tmp, tmp, w1);
		mpz_mod(corr_mesg[0], tmp, p);

		// corr_mesg[1] = M1[1] - m1 + w0;
		mpz_sub(tmp, M1[1], m1);
		mpz_add(tmp, tmp, w0);
		mpz_mod(corr_mesg[1], tmp, p);

		// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
		correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

		// M1[0] = m0 - w1;
		mpz_sub(tmp, m0, w1);
		mpz_mod(M1[0], tmp, p);

		// M1[1] = m1 - w0;
		mpz_sub(tmp, m1, w0);
		mpz_mod(M1[1], tmp, p);

		ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
		src[0] = mpz2block(w0);
		src[1] = mpz2block(w1);
		return;
	}

	// partea superioara
	for (int i = 0; i < values - 1; i += 2)
	{
		base_idx = (lvl_p) * (Val / 2) + perm_idx + i / 2;
		// m0 = src[i];
		// m1 = src[i ^ 1];
		block2mpz(src[i], m0);
		block2mpz(src[i ^ 1], m1);

		temp_block = ot_output[base_idx][0];
		// memcpy(M0, temp_block.data(), sizeof(M0));
		block2mpz(temp_block[0], M0[0]);
		block2mpz(temp_block[1], M0[1]);

		// w0 = M0[0] + m0
		mpz_add(tmp, M0[0], m0);
		mpz_mod(w0, tmp, p);

		// w1 = M0[1] + m1
		mpz_add(tmp, M0[1], m1);
		mpz_mod(w1, tmp, p);

		temp_block = ot_output[base_idx][1];
		// memcpy(M1, temp_block.data(), sizeof(M1));
		block2mpz(temp_block[0], M1[0]);
		block2mpz(temp_block[1], M1[1]);

		// corr_mesg[0] = M1[0] - m0 + w1;
		mpz_sub(tmp, M1[0], m0);
		mpz_add(tmp, tmp, w1);
		mpz_mod(corr_mesg[0], tmp, p);

		// corr_mesg[1] = M1[1] - m1 + w0;
		mpz_sub(tmp, M1[1], m1);
		mpz_add(tmp, tmp, w0);
		mpz_mod(corr_mesg[1], tmp, p);

		// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
		correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

		// M1[0] = m0 - w1;
		mpz_sub(tmp, m0, w1);
		mpz_mod(M1[0], tmp, p);

		// M1[1] = m1 - w0;
		mpz_sub(tmp, m1, w0);
		mpz_mod(M1[1], tmp, p);

		ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
		src[i] = mpz2block(w0);
		src[i ^ 1] = mpz2block(w1);

		bottom1.push_back(src[i]);
		top1.push_back(src[i ^ 1]);
	}

	if (values % 2 == 1)
	{
		top1.push_back(src[values - 1]);
	}

	cpus--;
	thread top_thrd, btm_thrd;
	if (cpus > 0)
	{
		top_thrd = thread(&OSNReceiver::prepare_correction, this, n - 1, Val, lvl_p + 1, perm_idx + values / 4, std::ref(top1), std::ref(ot_output), std::ref(correction_blocks));
	}
	else
	{
		prepare_correction(n - 1, Val, lvl_p + 1, perm_idx + values / 4, top1, ot_output, correction_blocks);
	}
	if (cpus > 0)
	{
		btm_thrd = thread(&OSNReceiver::prepare_correction, this, n - 1, Val, lvl_p + 1, perm_idx, std::ref(bottom1), std::ref(ot_output), std::ref(correction_blocks));
	}
	else
	{
		prepare_correction(n - 1, Val, lvl_p + 1, perm_idx, bottom1, ot_output, correction_blocks);
	}
	if (top_thrd.joinable())
		top_thrd.join();
	if (btm_thrd.joinable())
		btm_thrd.join();
	cpus++;

	// partea inferioara
	for (int i = 0; i < values - 1; i += 2)
	{
		base_idx = (lvl_p + levels - 1) * (Val / 2) + perm_idx + i / 2;
		// m1 = top1[i / 2];
		// m0 = bottom1[i / 2];
		block2mpz(top1[i / 2], m1);
		block2mpz(bottom1[i / 2], m0);

		temp_block = ot_output[base_idx][0];
		// memcpy(M0, temp_block.data(), sizeof(M0));
		block2mpz(temp_block[0], M0[0]);
		block2mpz(temp_block[1], M0[1]);

		// w0 = M0[0] + m0
		mpz_add(tmp, M0[0], m0);
		mpz_mod(w0, tmp, p);

		// w1 = M0[1] + m1
		mpz_add(tmp, M0[1], m1);
		mpz_mod(w1, tmp, p);

		temp_block = ot_output[base_idx][1];
		// memcpy(M1, temp_block.data(), sizeof(M1));
		block2mpz(temp_block[0], M1[0]);
		block2mpz(temp_block[1], M1[1]);

		// corr_mesg[0] = M1[0] - m0 + w1;
		mpz_sub(tmp, M1[0], m0);
		mpz_add(tmp, tmp, w1);
		mpz_mod(corr_mesg[0], tmp, p);

		// corr_mesg[1] = M1[1] - m1 + w0;
		mpz_sub(tmp, M1[1], m1);
		mpz_add(tmp, tmp, w0);
		mpz_mod(corr_mesg[1], tmp, p);

		// correction_blocks[base_idx] = {corr_mesg[0], corr_mesg[1]};
		correction_blocks[base_idx] = {mpz2block(corr_mesg[0]), mpz2block(corr_mesg[1])};

		// M1[0] = m0 - w1;
		mpz_sub(tmp, m0, w1);
		mpz_mod(M1[0], tmp, p);

		// M1[1] = m1 - w0;
		mpz_sub(tmp, m1, w0);
		mpz_mod(M1[1], tmp, p);

		ot_output[base_idx][1] = {mpz2block(M1[0]), mpz2block(M1[1])};
		src[i] = mpz2block(w0);
		src[i ^ 1] = mpz2block(w1);
	}

	int idx = int(ceil(values * 0.5));
	if (values % 2 == 1)
	{
		src[values - 1] = top1[idx - 1];
	}
}
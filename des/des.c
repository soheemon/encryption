#include <stdint.h>

#include "des.h"

typedef struct _split_56_28 {
	uint64_t : 8,
				  left : 28,
				  right: 28;
} split_56_28;

typedef struct _split_64_32 {
	uint64_t left : 32,
				  right: 32;
} split_64_32;

typedef struct _uint48_t {
	uint64_t : 16,
				  bit48 : 48;
} uint48_t;

typedef struct _parity_chk {
	uint8_t bit1:1,
				  bit2:1,
				  bit3:1,
				  bit4:1,
				  bit5:1,
				  bit6:1,
				  bit7:1,
				  parity_bit:1;
} parity_chk;

static uint48_t subkey[16];

static uint64_t parity_chk_64(uint56_t input);
static uint8_t parity_chk_8(uint8_t input);
static split_56_28 permutation_c_1(uint64_t input);
static void keygen_round(split_56_28 input);
static void left_shift_n(split_56_28* input, uint8_t n);
static uint56_t combine_left_with_right_28(split_56_28 input);
static uint48_t permutation_c_2(uint56_t input);
static uint64_t switch_64(split_64_32 input);
static split_64_32 initial_permutation(uint64_t input);
static split_64_32 seperate_64(uint64_t input);
static split_64_32 festal(split_64_32 input, uint48_t key);
static uint48_t expantion_permutation(split_64_32 input);
static uint32_t substitution(uint48_t input);
static uint32_t permutation_4(uint32_t input);
static uint64_t initial_permutation_inverse(uint64_t input);

void subkey_generate(uint56_t input) {
	uint64_t parity_res = 0;

	parity_res = parity_chk_64(input);

	split_56_28 left_shift_tmp;
	left_shift_tmp.left = 0;
	left_shift_tmp.right = 0;

	left_shift_tmp = permutation_c_1(parity_res);
	keygen_round(left_shift_tmp);
}

static uint64_t parity_chk_64(uint56_t input) {
	uint56_t tmp_56;
	tmp_56.bit56 = 0;
	uint8_t tmp_8[8] = {0}; 

	for(int i = 0; i < 8; i++) {
		tmp_56.bit56 = input.bit56 & ((uint64_t )0x7F <<(7 * i));
		tmp_8[i] = (tmp_56.bit56 >> (7 * i));
	}

	uint8_t parity_ret[8] = {0};
	for(int i = 0; i < 8; i++) {
		parity_ret[i] = parity_chk_8(tmp_8[i]);
	}
	uint64_t output = 0;
	for(int i = 7; i >= 0; i--) {

		output <<= 8;
		output |= (uint64_t)parity_ret[i];
	}

	return output;
}

static uint8_t parity_chk_8(uint8_t input) {

	parity_chk parity;
	uint8_t tmp = input;
	tmp <<=1;

	parity.bit1 = (tmp & 0x80) ? 1 : 0;
	parity.bit2 = (tmp & 0x40) ? 1 : 0;
	parity.bit3 = (tmp & 0x20) ? 1 : 0;
	parity.bit4 = (tmp & 0x10) ? 1 : 0;
	parity.bit5 = (tmp & 0x08) ? 1 : 0;
	parity.bit6 = (tmp & 0x04) ? 1 : 0;
	parity.bit7 = (tmp & 0x02) ? 1 : 0;

	parity.parity_bit = ~(((parity.bit1 ^ parity.bit2) ^ (parity.bit3 ^ parity.bit4)) ^ ((parity.bit5 ^ parity.bit6) ^ parity.bit7)) ? 1 : 0;
	uint8_t output = 0;

	output |= parity.bit1 ? 0x80 : 0;
	output |= parity.bit2 ? 0x40 : 0;
	output |= parity.bit3 ? 0x20 : 0;
	output |= parity.bit4 ? 0x10 : 0;
	output |= parity.bit5 ? 0x08 : 0;
	output |= parity.bit6 ? 0x04 : 0;
	output |= parity.bit7 ? 0x02 : 0;
	output |= parity.parity_bit ? 0x01 : 0;

	return output;
}

static split_56_28 permutation_c_1(uint64_t input) {
	
	split_56_28 output;

	uint64_t temp = 0;
	uint64_t out_temp = 0;
	output.left = 0;
	output.right = 0;

	uint8_t left_table[28] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36 };
	uint8_t right_table[28] = {63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

	for(int i = 0; i < 28; i++) {

		temp = input & ((uint64_t)0x8000000000000000 >> (left_table[i] - 1));
		temp = temp << (left_table[i] - 1);
		temp = temp >> i;

		out_temp |= temp;
	}   
	output.left = out_temp >> 36;

	out_temp = 0;
	temp = 0;

	for(int i = 0; i < 28; i++) {

		temp = input & ((uint64_t)0x8000000000000000 >> (right_table[i] - 1)); 
		temp = temp << (right_table[i] - 1);
		temp = temp >> i;

		out_temp |= temp;
	}   

	output.right = out_temp >> 36;

	return output;
}

static void keygen_round(split_56_28 input){

	uint8_t round_table[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; 

	uint56_t left_shift_res;
	left_shift_res.bit56 = 0;
	uint48_t pc_2_res;
	pc_2_res.bit48 = 0;

	split_56_28 left_shift_temp;
	left_shift_temp.left = 0;
	left_shift_temp.right = 0;

	left_shift_temp = input;

	for(int i = 0; i < 16; i++) {
	
		left_shift_n(&left_shift_temp, round_table[i]);

		left_shift_res.bit56 = 0;
		left_shift_res = combine_left_with_right_28(left_shift_temp);

		pc_2_res.bit48 = 0;
		pc_2_res = permutation_c_2(left_shift_res);

		subkey[i].bit48 = pc_2_res.bit48;
	}
}

static void left_shift_n(split_56_28* input, uint8_t n) {

	input->left = (input->left << n) | (input->left >> 28-n);
	input->right = (input->right << n) | (input->right >> 28-n);

}

static uint56_t combine_left_with_right_28(split_56_28 input) {
	uint56_t output;
	output.bit56 = 0;

	output.bit56 = input.left;
	output.bit56 <<= 28;
	output.bit56 |= input.right;

	return output;
}

static uint48_t permutation_c_2(uint56_t input) {
	uint8_t pc2_table[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
	uint56_t out_temp, temp;
	out_temp.bit56 = 0;
	temp.bit56 = 0;

	uint48_t output;
	output.bit48 = 0;

	for(int i = 0; i < 48; i++) {

		temp.bit56 = input.bit56 & (0x80000000000000 >> (pc2_table[i] - 1)); 
		temp.bit56 = temp.bit56 << (pc2_table[i] - 1);
		temp.bit56 = temp.bit56 >> i;

		out_temp.bit56 |= temp.bit56;

	}
	output.bit48 = (out_temp.bit56 >> 8);
	return output;
}

uint64_t des_encrypt(uint64_t input) {

	split_64_32 ip_res;
	ip_res.left = 0;
	ip_res.right = 0;

	ip_res = initial_permutation(input);

	split_64_32 festal_res;
	festal_res = ip_res;

	for(int i = 0; i < 16; i++) {
		festal_res = festal(festal_res, subkey[i]);
	}

	uint64_t sw_res;
	sw_res = switch_64(festal_res);

	uint64_t output = initial_permutation_inverse(sw_res);
	return output;
}

static uint64_t switch_64(split_64_32 input) {
	split_64_32 out_tmp;

	out_tmp.left = input.right;
	out_tmp.right = input.left;

	uint64_t output;
	output = out_tmp.left;
	output <<= 32;
	output |= out_tmp.right;

	return output; 
}
static split_64_32 initial_permutation(uint64_t input) {

	uint8_t ip_table[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
	
	uint64_t out_tmp = 0;
	uint64_t temp = 0;

	for(int i = 0; i < 64; i++) {

		temp = input & ((uint64_t)0x8000000000000000 >> (ip_table[i] - 1)); 
		temp = temp << (ip_table[i] - 1);
		temp = temp >> i;

		out_tmp |= temp;
	}   

	split_64_32 output;
	output.left = 0;
	output.right = 0;

	output = seperate_64(out_tmp);
	return output;
}

static split_64_32 seperate_64(uint64_t input) {

	split_64_32 output;
	output.left = 0;
	output.right = 0;

	output.left = input >> 32;
	output.right = (uint32_t)input;

	return output;
}

static split_64_32 festal(split_64_32 input, uint48_t key) {

	uint48_t res_ep;
	res_ep.bit48 = 0;

	res_ep = expantion_permutation(input);

	uint48_t res_xor;
	res_xor.bit48 = 0;

	res_xor.bit48 = (key.bit48 ^ res_ep.bit48);

	uint32_t res_sub = 0;
	res_sub = substitution(res_xor);

	uint32_t res_p4 = 0;
	res_p4 = permutation_4(res_sub);

	split_64_32 output;
	output.right = 0;
	output.right = (input.left ^ res_p4);

	output.left = input.right;
	return output;
}

static uint48_t expantion_permutation(split_64_32 input) {

	uint8_t ep_table[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
	uint48_t output;
	output.bit48 = 0;
	uint48_t temp;
	temp.bit48 = 0;

	for(int i = 0; i < 48; i++) {

		temp.bit48 = input.right & ((unsigned int)0x80000000 >> (ep_table[i] - 1));
		temp.bit48 <<= 16;
		temp.bit48 = temp.bit48 << (ep_table[i] - 1);
		temp.bit48 = temp.bit48 >> i;
		output.bit48 |= temp.bit48;
	}
	return output;
}

static uint32_t substitution(uint48_t input) {

	uint8_t sbox[8][4][16]={{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 2, 14, 10, 0, 6, 13}},

		{{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
			{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
			{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
			{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

		{{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
			{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
			{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
			{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

		{{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
			{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
			{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
			{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

		{{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
			{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
			{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
			{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

		{{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
			{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
			{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
			{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

		{{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
			{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
			{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
			{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

		{{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
			{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
			{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
			{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

	uint8_t tmp = 0;
	uint8_t sub_val[8];

	for(int i = 0; i < 8; i++, tmp = 0) {

		tmp = (input.bit48 & (0xFC0000000000 >> (6*i))) >> (48 - (6 * (i + 1)));

		sub_val[i] = tmp;
	}
	uint8_t row = 0;
	uint8_t col = 0;

	uint32_t output = 0;

	for(int i = 0; i < 8; i++, row = 0, col = 0) {

	row = ((sub_val[i] >> 4) & 2) | (sub_val[i]  & 1);
	col = (sub_val[i] & 0x1E) >> 1;
	sub_val[i] = sbox[i][row][col];
	}

	for(int i = 0; i < 8; i++) {
		output |= ((uint32_t)sub_val[i] & 0xF) << (28 - (i * 4));
	}
	return output; 
}

static uint32_t permutation_4(uint32_t input) {

	uint8_t p4_table[32] = {
		16, 7, 20, 21, 29, 12, 28, 17,
		1, 15, 23, 26,  5, 18, 31, 10,
		2, 8, 24, 14, 32, 27, 3, 9,
		19, 13, 30,  6, 22, 11, 4, 25
	};

	uint32_t output,temp;

	output = 0;
	temp = 0;
	for(int i = 0; i < 32; i++) {

		temp = input & (0x80000000 >> (p4_table[i] - 1)); 
		temp = temp << (p4_table[i] - 1);
		temp = temp >> i;

		output |= temp;
	}   

	return output;
}



static uint64_t initial_permutation_inverse(uint64_t input) {
	uint8_t ipi_table[64] = {
		40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41,  9, 49, 17, 57, 25
	};
	
	uint64_t output = 0;
	uint64_t temp = 0;

	for(int i = 0; i < 64; i++) {

		temp = input & (0x8000000000000000 >> (ipi_table[i] - 1)); 
		temp = temp << (ipi_table[i] - 1);
		temp = temp >> i;

		output |= temp;
	}   

	return output;
}
uint64_t des_decrypt(uint64_t input) {

	split_64_32 ip_res;
	ip_res.left = 0;
	ip_res.right = 0;

	ip_res = initial_permutation(input);

	split_64_32 festal_res;
	festal_res = ip_res;

	for(int i = 15; i >= 0; i--) {
		festal_res = festal(festal_res, subkey[i]);
	}

	uint64_t sw_res;
	sw_res = switch_64(festal_res);

	uint64_t output = initial_permutation_inverse(sw_res);
	return output;
}

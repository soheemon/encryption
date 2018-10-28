#include "sdes.h"

#define EXTRACT_BIT_10(src, num) ((src) & (0x200 >> (num - 1)))
#define EXTRACT_BIT_8(src, num) ((src) & (0x80 >> (num - 1)))
#define EXTRACT_BIT_4(src, num) ((src) & (0x8 >> (num - 1)))

#define SHIFT_BIT(src, from, to) (((src) << ((from) - 1)) >> ((to) - 1))

#define SPLIT_BIT_10_LEFT(src) (((src) & 0x3E0) >> 5)
#define SPLIT_BIT_10_RIGHT(src) ((src) & 0x1F)
#define SPLIT_BIT_8_LEFT(src) (((src) & 0xF0) >> 4)
#define SPLIT_BIT_8_RIGHT(src) ((src) & 0x0F)
#define ATTACH_BIT_10(input, left, right) input = left; input = left << 5; input |= right
#define ATTACH_BIT_8(input, left, right) input = left; input = left << 4; input |= right
#define ATTACH_BIT_4(input, left, right) input = left; input = left << 2; input |= right


#define BIT_ON_5(input, num) ((input) |= (0x10 >> (num-1)))
#define BIT_OFF_5(input, num) ((input) &= (~(0x10 >> (num-1))))
#define CHECK_BIT_5(input, num) (((input) & (0x10 >> (num-1))) ? 1 : 0)

#define LEFT_SHIFT(input) do{\
	bit8_t flag = 0;\
	flag = CHECK_BIT_5(input, 1);\
	BIT_OFF_5(input, 1);\
	input <<=1;\
	if(flag){\
		BIT_ON_5(input, 5);\
	} else {\
		BIT_OFF_5(input, 5);\
	}\
}while(0)

#define SPLIT_COL_ROW(input, col, row)\
	col = (input & 0x6) >>1;\
	row = (input & 0x1);\
	row |= ((input & 0x8) >>2)

#define SWITCH(input) (((input) << 4) | ((input) >> 4));


typedef struct splited_10_5_tag {
	unsigned short : 6,
				   left : 5,
				   right : 5;
} splited_10_5;

typedef struct splited_8_4_tag {
	unsigned char left : 4,
				  right : 4;
} splited_8_4;

typedef struct splited_4_2_tag {
	unsigned char : 4,
				  row : 2,
				  col : 2;
} splited_4_2;

typedef struct sub_key_tag {
	unsigned short key_1 : 8,
				   key_2 : 8;
} sub_key;

static sub_key subkey; 

typedef struct bit4_t_tag {
	unsigned char : 4,
				   bit4 : 4;
} bit4_t;

typedef struct bit5_t_tag {
	unsigned char : 3,
				   bit5 : 5;
} bit5_t;

static bit10_t permutation_10(bit10_t input) {
	bit8_t p10_table[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};

	bit10_t output, temp;
	output.bit10 = 0;
	temp.bit10 = 0;

	for(int i = 1; i <= 10; i++) {
		temp.bit10 = EXTRACT_BIT_10(input.bit10, p10_table[i-1]);
		temp.bit10 = SHIFT_BIT(temp.bit10, p10_table[i-1], i);
		output.bit10 |= temp.bit10;
	}   
	return output;
}

static splited_10_5 left_shift(splited_10_5 input) {
	splited_10_5 output;
	output.left = 0;
	output.right = 0;
	LEFT_SHIFT(input.left);
	LEFT_SHIFT(input.right);
	output.left = input.left;
	output.right = input.right;
	
	return output;
}

static bit10_t left_shift_1(bit10_t input) {

	splited_10_5 temp;
	temp.left = 0;
	temp.right = 0;
	bit10_t output;
	output.bit10 = 0;

	temp.left = SPLIT_BIT_10_LEFT(input.bit10);
	temp.right = SPLIT_BIT_10_RIGHT(input.bit10);
	temp = left_shift(temp);
	ATTACH_BIT_10(output.bit10, temp.left, temp.right);
	
	return output;
}

static bit10_t left_shift_2(bit10_t input) {

	splited_10_5 temp;
	temp.left = 0;
	temp.right = 0;

	bit10_t output;
	output.bit10 = 0;

	temp.left = SPLIT_BIT_10_LEFT(input.bit10);
	temp.right = SPLIT_BIT_10_RIGHT(input.bit10);

	temp = left_shift(temp);
	temp = left_shift(temp);

	ATTACH_BIT_10(output.bit10, temp.left, temp.right);
	
	return output;
}

static bit8_t permutation_8(bit10_t input) {
	bit8_t p8_table[8] = {6, 3, 7, 4, 8, 5, 10, 9};
	bit10_t out_temp, temp;
	out_temp.bit10 = 0;
	bit8_t output = 0;

	for(int i = 1; i <= 8; i++) {

		temp.bit10 = EXTRACT_BIT_10(input.bit10, p8_table[i-1]);
		temp.bit10 = SHIFT_BIT(temp.bit10, p8_table[i-1], i);
		out_temp.bit10 |= temp.bit10;
	}
	output = (out_temp.bit10 >> 2);
	return output;
}

void key_generate(bit10_t key) {

	bit10_t temp;
	temp.bit10 = 0;

	bit8_t temp2 = 0;
	temp = permutation_10(key);
	temp = left_shift_1(temp);

	temp2 = permutation_8(temp);
	subkey.key_1 = temp2;

	temp = left_shift_2(temp);
	temp2 = permutation_8(temp);
	subkey.key_2 = temp2;

}
static splited_8_4 initial_permutation(bit8_t input) {

	bit8_t ip_table[8] = {2, 6, 3, 1, 4, 8, 5, 7};
	
	bit8_t out_tmp = 0;
	bit8_t temp = 0;

	for(int i = 1; i <= 8; i++) {

	temp = EXTRACT_BIT_8(input, ip_table[i-1]);
	temp = SHIFT_BIT(temp, ip_table[i-1], i);
		out_tmp |= temp;
	}
	splited_8_4 output;
	output.left = 0;
	output.right = 0;

	output.left = SPLIT_BIT_8_LEFT(out_tmp);
	output.right = SPLIT_BIT_8_RIGHT(out_tmp);

	return output;
}
static bit8_t expantion_permutation(splited_8_4 input) {

	bit8_t ep_table[8] = {4, 1, 2, 3, 2, 3, 4, 1};
	bit8_t output = 0;
	bit8_t temp = 0;

	for(int i = 1; i <= 8; i++) {
		temp = EXTRACT_BIT_4(input.right, ep_table[i-1]);
		temp = temp << 4;
		temp = SHIFT_BIT(temp, ep_table[i-1], i);
		output |= temp;
	}
	return output;
}

static bit4_t substitution(splited_8_4 input) {

	splited_4_2 left;
	left.row = 0;
	left.col = 0;

	splited_4_2 right;
	right.row = 0;
	right.col = 0;

	bit4_t output;
	output.bit4 = 0;

	SPLIT_COL_ROW(input.left, left.col, left.row);
	SPLIT_COL_ROW(input.right, right.col, right.row); 

	bit8_t s0_table[4][4] = {{1, 0, 3, 2},{3, 2, 1, 0},{0, 2, 1, 3},{3, 1, 3, 2}};
	bit8_t s1_table[4][4] = {{0, 1, 2, 3},{2, 0, 1, 3},{3, 0, 1, 0},{2, 1, 0, 3}};

	ATTACH_BIT_4(output.bit4, s0_table[left.row][left.col], s1_table[right.row][right.col]);

	return output; 
}
static bit4_t permutation_4(bit4_t input) {

	bit8_t p4_table[4] = {2, 4, 3, 1};
	bit4_t output;
	output.bit4 = 0;
	bit4_t temp;
	temp.bit4 = 0;

	for(int i = 1; i <= 4; i++) {
		temp.bit4 = EXTRACT_BIT_4(input.bit4, p4_table[i-1]);
		temp.bit4 = SHIFT_BIT(temp.bit4, p4_table[i-1], i);
		output.bit4 |= temp.bit4;
	}
	return output;
}
static bit4_t function_key(splited_8_4 input, bit8_t key) {

	bit8_t res_ep = 0;
	bit8_t res_xor = 0;

	res_ep = expantion_permutation(input);

	res_xor = (key ^ res_ep);

	splited_8_4 temp_sub;
	temp_sub.left = 0;
	temp_sub.right = 0;

	temp_sub.left = SPLIT_BIT_8_LEFT(res_xor);
	temp_sub.right = SPLIT_BIT_8_RIGHT(res_xor);

	bit4_t res_sub;
	res_sub.bit4 = 0;
	res_sub = substitution(temp_sub);

	bit4_t res_p4;
	res_p4.bit4 = 0;
	res_p4 = permutation_4(res_sub);
	return res_p4;

}

static bit8_t initial_permutation_inverse(bit8_t input) {
	bit8_t ip_i_table[8] = {4, 1, 3, 5, 7, 2, 8, 6};
	bit8_t output = 0;
	bit8_t temp = 0;

	for(int i = 1; i <= 8; i++) {
		temp = EXTRACT_BIT_8(input, ip_i_table[i-1]);
		temp = SHIFT_BIT(temp, ip_i_table[i-1], i);
		output |= temp;
	}
	return output;
}

static bit8_t fk_key(splited_8_4 input, bit8_t key) {

	bit4_t fk_res;
	fk_res.bit4 = 0;
	fk_res = function_key(input, key);

	bit4_t temp;
	temp.bit4 = 0;
	temp.bit4 = (input.left ^ fk_res.bit4);

	bit8_t temp2 = 0;
	ATTACH_BIT_8(temp2, temp.bit4, input.right);

	return temp2;
}

bit8_t sdes_encrypt(bit8_t input) {
	splited_8_4 ip_res;
	ip_res.left = 0;
	ip_res.right = 0;

	ip_res = initial_permutation(input);

	bit8_t k1_tmp = 0;

	k1_tmp = fk_key(ip_res, (bit8_t)subkey.key_1);

	bit8_t k1_out_tmp = 0;
	k1_out_tmp = SWITCH(k1_tmp);

	splited_8_4 k1_res;
	k1_res.left = 0;
	k1_res.right = 0;

	k1_res.left = SPLIT_BIT_8_LEFT(k1_out_tmp);
	k1_res.right = SPLIT_BIT_8_RIGHT(k1_out_tmp);

	bit8_t output = 0;
	output = fk_key(k1_res, (bit8_t)subkey.key_2);
	bit8_t output2 = initial_permutation_inverse(output);
	return output2;
}
bit8_t sdes_decrypt(bit8_t input) {
	splited_8_4 ip_res;
	ip_res.left = 0;
	ip_res.right = 0;

	ip_res = initial_permutation(input);

	bit8_t k1_tmp = 0;

	k1_tmp = fk_key(ip_res, (bit8_t)subkey.key_2);

	bit8_t k1_out_tmp = 0;
	k1_out_tmp = SWITCH(k1_tmp);

	splited_8_4 k1_res;
	k1_res.left = 0;
	k1_res.right = 0;

	k1_res.left = SPLIT_BIT_8_LEFT(k1_out_tmp);
	k1_res.right = SPLIT_BIT_8_RIGHT(k1_out_tmp);

	bit8_t output = 0;
	output = fk_key(k1_res, (bit8_t)subkey.key_1);
	bit8_t output2 = initial_permutation_inverse(output);
	return output2;
}


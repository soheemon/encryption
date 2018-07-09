#include <stdio.h>
typedef unsigned long long bit_64;
typedef unsigned int bit_32;
typedef struct _splited_56_28 {
	unsigned long long : 8,
				  left : 28,
				  right: 28;
} splited_56_28;

typedef struct _splited_64_32 {
	unsigned long long left : 32,
				  right: 32;
} splited_64_32;

typedef struct _bit_56 {
	unsigned long long : 8,
				  bit56 : 56;
} bit_56;

typedef struct _bit_48 {
	unsigned long long : 16,
				  bit48 : 48;
} bit_48;

typedef struct _sub_key {

	bit_48 sub_key1;
	bit_48 sub_key2;
	bit_48 sub_key3;
	bit_48 sub_key4;
	bit_48 sub_key5;
	bit_48 sub_key6;
	bit_48 sub_key7;
	bit_48 sub_key8;
	bit_48 sub_key9;
	bit_48 sub_key10;
	bit_48 sub_key11;
	bit_48 sub_key12;
	bit_48 sub_key13;
	bit_48 sub_key14;
	bit_48 sub_key15;
	bit_48 sub_key16;

} sub_key;

sub_key subkey;
typedef struct _sp_val {

	char sp_1;
	char sp_2;
	char sp_3;
	char sp_4;
	char sp_5;
	char sp_6;
	char sp_7;
	char sp_8;

} sp_val;
splited_56_28 permutation_c_1(bit_64 input) {
	
	splited_56_28 output;

	bit_64 temp = 0;
	bit_64 out_temp = 0;
	output.left = 0;
	output.right = 0;

	char left_table[28] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36 };
	char right_table[28] = {63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

	for(int i = 0; i < 28; i++) {

		temp = input & (0x8000000000000000 >> (left_table[i] - 1)); 
		temp = temp << (left_table[i] - 1);
		temp = temp >> i;

		out_temp |= temp;
	}   
	output.left = out_temp >> 36;

	out_temp = 0;
	temp = 0;

	for(int i = 0; i < 28; i++) {

		temp = input & (0x8000000000000000 >> (right_table[i] - 1)); 
		temp = temp << (right_table[i] - 1);
		temp = temp >> i;

		out_temp |= temp;
	}   

	output.right = out_temp >> 36;

	return output;
}

void left_shift_n(splited_56_28* input, int n) {

	input->left = (input->left << n) | (input->left >> 28-1);
	input->right = (input->right << n) | (input->right >> 28-1);

}
bit_56 combine_left_with_right_28(splited_56_28 input) {

	bit_56 output;
	output.bit56 = 0;

	output.bit56 = input.left;
	output.bit56 <<= 28;
	output.bit56 |= input.right;

	return output;
}

splited_64_32 seperate_64(bit_64 input) {

	splited_64_32 output;
	output.left = 0;
	output.right = 0;

	output.left = input >> 32;
	output.right = input << 32;

	return output;
}

bit_48 permutation_c_2(bit_56 input) {
	char pc2_table[48] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
	bit_56 out_temp, temp;
	out_temp.bit56 = 0;
	temp.bit56 = 0;

	bit_48 output;
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
void keygen_round(splited_56_28 input){

	char round_table[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; 

	bit_56 left_shift_res;
	bit_48 pc_2_res;

	splited_56_28 left_shift_temp;
	left_shift_temp.left = 0;
	left_shift_temp.right = 0;

	left_shift_temp = input;

	for(int i = 0; i < 16; i++) {
	
		left_shift_n(&left_shift_temp, round_table[i]);

		left_shift_res.bit56 = 0;
		left_shift_res = combine_left_with_right_28(left_shift_temp);


		pc_2_res.bit48 = 0;
		pc_2_res = permutation_c_2(left_shift_res);

		switch (i+1) {
			case 1 : subkey.sub_key1.bit48 = pc_2_res.bit48;
					 break;
			case 2 : subkey.sub_key2.bit48 = pc_2_res.bit48;
					 break;
			case 3 : subkey.sub_key3.bit48 = pc_2_res.bit48;
					 break;
			case 4 : subkey.sub_key4.bit48 = pc_2_res.bit48;
					 break;
			case 5 : subkey.sub_key5.bit48 = pc_2_res.bit48;
					 break;
			case 6 : subkey.sub_key6.bit48 = pc_2_res.bit48;
					 break;
			case 7 : subkey.sub_key7.bit48 = pc_2_res.bit48;
					 break;
			case 8 : subkey.sub_key8.bit48 = pc_2_res.bit48;
					 break;
			case 9 : subkey.sub_key9.bit48 = pc_2_res.bit48;
					 break;
			case 10 : subkey.sub_key10.bit48 = pc_2_res.bit48;
					 break;
			case 11 : subkey.sub_key11.bit48 = pc_2_res.bit48;
					 break;
			case 12 : subkey.sub_key12.bit48 = pc_2_res.bit48;
					 break;
			case 13 : subkey.sub_key13.bit48 = pc_2_res.bit48;
					 break;
			case 14 : subkey.sub_key14.bit48 = pc_2_res.bit48;
					 break;
			case 15 : subkey.sub_key15.bit48 = pc_2_res.bit48;
					 break;
			case 16 : subkey.sub_key16.bit48 = pc_2_res.bit48;
					 break;
		}
	}
}
void subkey_generate(bit_64 input) {

	splited_56_28 left_shift_tmp;
	left_shift_tmp.left = 0;
	left_shift_tmp.right = 0;

	left_shift_tmp = permutation_c_1(input);

	keygen_round(left_shift_tmp);
}

bit_48 expantion_permutation(splited_64_32 input) {

	char ep_table[48] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
	bit_48 output;
	output.bit48 = 0;
	bit_48 temp;
	temp.bit48 = 0;

	for(int i = 0; i < 48; i++) {

		temp.bit48 = input.right & (0x80000000 >> (ep_table[i] - 1));
		temp.bit48 <<= 16;
		temp.bit48 = temp.bit48 << (ep_table[i] - 1);
		temp.bit48 = temp.bit48 >> i;

		output.bit48 |= temp.bit48;
	return output;
	}
}

bit_32 substitution(bit_48 input) {

	bit_32 output = 0;
	sp_val sp_value;

//	char s1_table[4][16] = ;
//	char s2_table[4][16] = ;
//	char s3_table[4][16] = ;
//	char s4_table[4][16] = ;
//	char s5_table[4][16] = ;
//	char s6_table[4][16] = ;
//	char s7_table[4][16] = ;
	char tmp = 0;
	for(int i = 0; i < 8; i++, tmp = 0) {

		tmp = (input.bit48 & (0xFC0000000000 >> (6*i))) >> (48 - (6 * (i + 1)));

		printf("%d: %x\n", i+1, tmp<<2);
		switch (i+1) {
			case 1 : sp_value.sp_1 = tmp;
					 break;
			case 2 : sp_value.sp_2 = tmp;
					 break;
			case 3 : sp_value.sp_3 = tmp;
					 break;
			case 4 : sp_value.sp_4 = tmp;
					 break;
			case 5 : sp_value.sp_5 = tmp;
					 break;
			case 6 : sp_value.sp_6 = tmp;
					 break;
			case 7 : sp_value.sp_7 = tmp;
					 break;
			case 8 : sp_value.sp_8 = tmp;
					 break;
		}
	}
//	ATTACH_BIT_4(output.bit4, s0_table[left.row][left.col], s1_table[right.row][right.col]);

	return output; 
}
//bit_32 function_key(splited_8_4 input, bit_48 key) {
//
//	bit_48 res_ep;
//	res_ep.bit48 = 0;
//	bit_48 res_xor;
//	res_xor.bit48 = 0;
//
//	res_ep = expantion_permutation(input);
//
//	res_xor.bit48 = (key.bit48 ^ res_ep.bit48);
//
//	bit_32 res_sub = 0;
//	res_sub = substitution(res_xor);
////
//	bit_32 res_p = 0;
////	res_p4 = permutation_4(res_sub);
//	return res_p;
////
//}

//bit8_t fk_key(splited_8_4 input, bit8_t key) {
//
//	bit4_t fk_res;
//	fk_res.bit4 = 0;
//	fk_res = function_key(input, key);
//
//	bit4_t temp;
//	temp.bit4 = 0;
//	temp.bit4 = (input.left ^ fk_res.bit4);
//
//	bit8_t temp2 = 0;
//	ATTACH_BIT_8(temp2, temp.bit4, input.right);
//
//	return temp2;
//}

splited_64_32 initial_permutation(bit_64 input) {

	char ip_table[64] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
	
	bit_64 out_tmp = 0;
	bit_64 temp = 0;

	for(int i = 0; i < 64; i++) {

		temp = input & (0x8000000000000000 >> (ip_table[i] - 1)); 
		temp = temp << (ip_table[i] - 1);
		temp = temp >> i;

		out_tmp |= temp;
	}   

	splited_64_32 output;
	output.left = 0;
	output.right = 0;

	output = seperate_64(out_tmp);
	return output;
}

bit_64 des_encrypt(bit_64 input) {

	splited_64_32 ip_res;
	ip_res.left = 0;
	ip_res.right = 0;

	ip_res = initial_permutation(input);
//	bit8_t k1_tmp = 0;
//
//	k1_tmp = fk_key(ip_res, (bit8_t)subkey.key_1);
//
//	bit8_t k1_out_tmp = 0;
//	k1_out_tmp = SWITCH(k1_tmp);
//
//	splited_8_4 k1_res;
//	k1_res.left = 0;
//	k1_res.right = 0;
//
//	k1_res.left = SPLIT_BIT_8_LEFT(k1_out_tmp);
//	k1_res.right = SPLIT_BIT_8_RIGHT(k1_out_tmp);
//
//	bit8_t output = 0;
//	output = fk_key(k1_res, (bit8_t)subkey.key_2);
//	bit8_t output2 = initial_permutation_inverse(output);
//	return output2;
}

void main(void) {
//	bit_64 temp = 0x4E36DD8D568284C9;
//
//	bit_64 plain_t = (bit_64)"SOHEEMON";
//	subkey_generate(temp);
//	des_encrypt(plain_t);
	bit_48 temp;
	temp.bit48 = 0xFCFCFCFCFCFC;
	substitution(temp);
}

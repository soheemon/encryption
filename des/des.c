#include <stdio.h>

typedef unsigned long long bit_64;

typedef struct _splited_56_28 {
	unsigned long long : 8,
				  left : 28,
				  right: 28;
} splited_56_28;

typedef struct _bit_56 {
	unsigned long long : 8,
				  bit56 : 56;
} bit_56;

splited_56_28 permutation_c(bit_64 input) {
	
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

	printf("left: %x \n right: %x", input->left, input->right);
}


void main(void) {
	bit_64 temp = 0x4E36DD8D568284C9;
	splited_56_28 temp_t;

	temp_t = permutation_c(temp);
	left_shift_n(&temp_t, 1);

}

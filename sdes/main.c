#include <stdio.h>
#include <stdlib.h>

#include "sdes.h"

int main(int argc, char* argv[]) {

	if(argc != 3) {

		printf("input 10bit key, 8bit character\n");
		printf("example: %s 300 A\n", argv[0]);

		return 1;
	}

	int temp = atoi(argv[1]); 
	if(temp > 1023){

		printf("key size error!\n");
		return 1;
	}

	bit10_t secret_key;
	secret_key.bit10 = temp;

	temp = atoi(argv[2]);
	if(temp > 254) {

		printf("text size error! \n");
		return 1;
	}

	bit8_t plain_text = *argv[2];

	key_generate(secret_key);

	bit8_t encrypt_text = sdes_encrypt(plain_text);
	bit8_t decrypt_text = sdes_decrypt(encrypt_text);

	printf(" secret key: %x\n plain text: %c\n encrypt_text: %c\n decrypt_text: %c\n", secret_key.bit10, plain_text, encrypt_text, decrypt_text);
}

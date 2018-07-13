#include <stdint.h>

#ifndef __DES_H__
#define __DES_H__

typedef struct _uint56_t {
	uint64_t : 8,
				  bit56 : 56;
} uint56_t;

void subkey_generate(uint56_t input);
uint64_t des_encrypt(uint64_t input);
uint64_t des_decrypt(uint64_t input);

#endif

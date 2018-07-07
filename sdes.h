typedef unsigned char bit8_t;

typedef struct bit10_t_tag {
	unsigned short : 6,
				   bit10 : 10;
} bit10_t;

void key_generate(bit10_t secret_key);
bit8_t sdes_encrypt(bit8_t plain_text);
bit8_t sdes_decrypt(bit8_t encrypted_text);

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "des.h"

void file_encrypt(char* input, const char* key, char* mode) {

	char* src_file_name = input;
	char dst_file_name[30];

	for(int i = 0, j = 0; j < 30 ; i++, j++) {

		if(src_file_name[i] == '.') {
			dst_file_name[j] = '_';
			dst_file_name[++j] = 'e';
			dst_file_name[++j] = 'n';
			dst_file_name[++j] = 'c';
			dst_file_name[++j] = src_file_name[i]; //add '.'
			continue;
		}

		dst_file_name[j] = src_file_name[i];
	}

	FILE* src_file = fopen(src_file_name, "rb");

	if(src_file == NULL) {
		return;
	}

	fseek(src_file, 0, SEEK_END);
	uint32_t file_size = ftell(src_file);
	rewind(src_file);
if(file_size < sizeof(uint64_t)) {
		printf("file size error!\n");
	}
	uint64_t* buffer = (uint64_t*)malloc(sizeof(uint64_t));
	if(buffer == NULL) {
		return;
	}

	memset(buffer, 0, sizeof(uint64_t));

	FILE* dst_file = fopen(dst_file_name, "a");
	if(dst_file == NULL) {
		return;
	}
	
	size_t read_size = 0;
	uint64_t enc_text = 0;

	uint56_t user_key;
	user_key.bit56 = atoi(key);

	subkey_generate(user_key);

	while(1) {

		read_size = fread(buffer,1 ,sizeof(uint64_t), src_file);

		if (read_size < sizeof(uint64_t)) {

			enc_text = des_encrypt(*buffer);
			fwrite(&enc_text,1 , sizeof(uint64_t), dst_file);

			break;
		}

		enc_text = des_encrypt(*buffer);
		fwrite(&enc_text,1 , sizeof(uint64_t), dst_file);
	}

	fclose(dst_file);
	fclose(src_file);
	free(buffer);
}

void file_decrypt(char* input, const char* key, char* mode) {

	char* src_file_name = input;
	char dst_file_name[30];

//	strcpy(des_file_name, src_file_name);

	for(int i = 0, j = 0; j < 30 ; i++, j++) {

		if(src_file_name[i] == '.') {
			dst_file_name[j] = '_';
			dst_file_name[++j] = 'e';
			dst_file_name[++j] = 'n';
			dst_file_name[++j] = 'c';
			dst_file_name[++j] = src_file_name[i]; //add '.'
			continue;
		}

		dst_file_name[j] = src_file_name[i];
	}

	FILE* src_file = fopen(src_file_name, "rb");

	if(src_file == NULL) {
		return;
	}

	fseek(src_file, 0, SEEK_END);
	uint32_t file_size = ftell(src_file);
	rewind(src_file);
if(file_size < sizeof(uint64_t)) {
		printf("file size error!\n");
	}
	uint64_t* buffer = (uint64_t*)malloc(sizeof(uint64_t));
	if(buffer == NULL) {
		return;
	}

	memset(buffer, 0, sizeof(uint64_t));

	FILE* dst_file = fopen(dst_file_name, "a");
	if(dst_file == NULL) {
		return;
	}
	
	size_t read_size = 0;
	uint64_t enc_text = 0;

	uint56_t user_key;
	user_key.bit56 = atoi(key);

	subkey_generate(user_key);

	while(1) {

		read_size = fread(buffer,1 ,sizeof(uint64_t), src_file);

		if (read_size < sizeof(uint64_t)) {

			enc_text = des_decrypt(*buffer);
			fwrite(&enc_text,1 , sizeof(uint64_t), dst_file);

			break;
		}

		enc_text = des_encrypt(*buffer);
		fwrite(&enc_text,1 , sizeof(uint64_t), dst_file);
	}

	fclose(dst_file);
	fclose(src_file);
	free(buffer);
}
int main(int argc, char* argv[]) {

	if (argc != 4) {
		printf("input error!\n");
		return 1;
	}

	file_decrypt(argv[1], argv[2], argv[3]);


}

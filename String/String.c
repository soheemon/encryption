#include <stdio.h>
#include <stdlib.h>

typedef struct _string {
	char* string;
	int strLength;

	void (*strInit) (const char*, struct _string*);
	void (*freeStr) (struct _string*);
	void (*pri) (struct _string*);
}String;

void _freeStr (String* this) {
	free(this->string);
	free(this);
}
void _strInit (const char* string, String* this) {

	char* tmpChar = (char*)string;
	int _strLength = 0;

	while(*tmpChar != '\0') {

		tmpChar++;
		_strLength++;
	}

	this->strLength = _strLength;
	this->string = (char*)malloc(_strLength);

	tmpChar = (char*)string;

	int idxOfStr = 0;
	while(*tmpChar != '\0') {

		this->string[idxOfStr] = string[idxOfStr];
		tmpChar++;
		idxOfStr++;
	}
	this->string[idxOfStr] = '\0';
}

void _pri(String* this) {
	printf("%s %d\n", this->string, this->strLength);
}

void insInit (String* this) {
	this->strInit = _strInit;
	this->pri = _pri;
	this->freeStr = _freeStr;
}

void main(void) {
	String* test;
	test = (String*) malloc(sizeof(String));

	insInit(test);

	test->strInit("aaaaaaaaaassssssssssddddddddddffffffffff", test);
	test->pri(test);
	test->freeStr(test);

}

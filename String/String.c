#include <stdio.h>
#include <stdlib.h>

typedef enum {
	true = 0,
	false = 1
} bool;

typedef struct _string {
	char* string;
	int strLength;

	void (*strInit) (const char*, struct _string*);
	void (*strFree) (struct _string*);
	void (*pri) (struct _string*);

	bool (*startWith) (char, struct _string*);
	bool (*endWith) (char, struct _string*);
	bool (*equals) (struct _string*, struct _string*);
}String;

void _strFree (String* this) {
	free(this->string);
	free(this);
}

bool _startWith (char chr, String* this) {

	if(chr == this->string[0]) {
		return true;
	}else {
		return false;
	}

}

bool _endWith (char chr, String* this) {

	if(chr == this->string[this->strLength-1]) {
		return true;
	}else {
		return false;
	}

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

String* insInit (String* this) {

	this = (String*) malloc(sizeof(String));

	this->strInit = _strInit;
	this->pri = _pri;
	this->strFree = _strFree;

	this->startWith = _startWith;
	this->endWith = _endWith;

	return this;
}

void main(void) {

	String* test;
	test = insInit(test);

	test->strInit("aaaaaaaaaassssssssssddddddddddffffffffff!", test);
	test->pri(test);
	printf("%d\n", (int)test->startWith('a', test));
	printf("%d\n", (int)test->endWith('!', test));
	test->strFree(test);

}

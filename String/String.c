#include "String.h"

static void _replace (String* this, char srcChar, char repChar) {

	for(int idxOfStr = 0; idxOfStr < this->strLength - 1; idxOfStr++){
		if (this->string[idxOfStr] == srcChar) {
			this->string[idxOfStr] = repChar;
		}
	}
}

static int _indexOf (String* this, char comChar) {

	int idxOfStr = 0;

	while(idxOfStr < this->strLength) {

		if(this->string[idxOfStr] == comChar) {
			return idxOfStr;
		}

		idxOfStr++;
	}
	return -1;
}

static int _lastIndexOf (String* this, char comChr) {

	int idxOfStr = this->strLength - 1;

	while(idxOfStr > 0) {

		if(this->string[idxOfStr] == comChr) {
			return idxOfStr;
		}

		idxOfStr--;
	}
	return -1;
}


static int _getLength (String* this) {
	return this->strLength;
}

static bool _equals (String* str1, String* str2) {

	if(str1->strLength != str2->strLength) {
		return false;
	}
	
	int strIndex = 0;

	while(strIndex < str1->strLength) {

		if(str1->string[strIndex] != str2->string[strIndex]) {
			return false;
		}

		strIndex++;
	}
	return true;
}

static bool _startWith (char chr, String* this) {

	if(chr == this->string[0]) {
		return true;
	}else {
		return false;
	}

}

static bool _endWith (char chr, String* this) {

	if(chr == this->string[this->strLength-1]) {
		return true;
	}else {
		return false;
	}

}

static void _strInit (const char* string, String* this) {


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
	while(string[idxOfStr] != '\0') {

		this->string[idxOfStr] = string[idxOfStr];
		idxOfStr++;
	}
	this->string[idxOfStr] = '\0';
}

static void _pri(String* this) {
	printf("%s\n", this->string);
}

void _strFree(String* this) {
	free(this->string);
	free(this);
}

String* insInit (String* this) {

	this = (String*) malloc(sizeof(String));

	this->strInit = _strInit;
	this->strFree = _strFree;
	this->pri = _pri;

	this->startWith = _startWith;
	this->endWith = _endWith;

	this->equals = _equals;
	this->getLength = _getLength;

	this->indexOf = _indexOf;
	this->lastIndexOf = _lastIndexOf;
	this->replace = _replace;

	return this;
}


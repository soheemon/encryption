#include <stdio.h>
#include <stdlib.h>

#ifndef __DES_H__
#define __DES_H__

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

	char* (*toChar) (struct _string*);
	int (*getLength) (struct _string*);

	int (*indexOf) (struct _string*, char comChar);
	int (*lastIndexOf) (struct _string*, char cmpChar);


} String;

String* insInit(String* this);

#endif

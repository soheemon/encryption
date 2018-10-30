#include "String.h"                                                        

int main(int argc, char* args[]) {                                                          

	String* testString1;
	String* testString2;

	testString1	= insInit(testString1);
	testString2	= insInit(testString2);

	testString1->strInit("hello from the other side", testString1); // String 객체에 문자열 할당
	testString2->strInit("I must've called a thousand times", testString2);

	testString1->pri(testString1);
	testString2->pri(testString2);

	printf("testString1 length :%d\n", testString1->getLength(testString1)); // 문자열 길이 getter
	printf("testString2 length :%d\n", testString2->getLength(testString2));

	if(testString1->equals(testString1, testString2) == true) { // 문자열 비교 함수
		printf("testString1 and testString2 are same\n");
	} else {
		printf("testString1 and testString2 are not same\n");
	}

	char chr = 'X';
	int index = testString1->indexOf(testString1, chr); // testString1 객체에서 문자 X가 존재여부/몇번째에 존재하는지 확인.

	if(index < 0) {
		printf("%c is not in the string\n", chr);
	}else {
		printf("%c is at %d in the string\n", chr, index);
	}

	testString1->strFree(testString1);
	testString2->strFree(testString2);

}  

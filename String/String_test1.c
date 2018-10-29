#include "String.h"                                                        

void main(void) {                                                          

	String* test;                                                          
	String* test1;                                                         

	test = insInit(test);                                                  
	test1 = insInit(test1);                                                

	test->strInit("aaaahhhaaaassssssssssddddddddddffffffffff!", test);     
	test1->strInit("aaaaaaaaaassssssssssddddddddddffffffffff!", test1);    

	printf("%d\n", test->equals(test, test1));                             
	printf("%d\n", test->getLength(test));                                 
	char* test3 = (char*) malloc(test->getLength(test));                   
	test3 = test->toChar(test);                                            
	printf("%s\n", test3);                                                 
	printf("%d\n", test1->indexOf(test1, 'h'));                            
	test->strFree(test);                                                   
	test1->strFree(test1);                                                 
	free(test3);                                                           

}  

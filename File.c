#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef struct _file {
	FILE* fileStream;
	void (*fileOpen) (char*, struct _file*);
	void (*readOneLine) (char*, struct _file*);
	void (*fileReopen) (char*, struct _file*);

	void (*removeComment) (struct _file*);
}file;

void _fileOpen(char* filePath, file* this) {
	if ((this->fileStream = fopen(filePath, "r+")) == NULL) { //a면 커서가 파일의 끝으로 이동한 상태로 open한다. //r, w 사용시 예외처리 필요
		return;
	} } void _fileClose(file* this) {
	fclose(this->fileStream);
}
void _readOneLine(char* dst, file* this) {
	if (fgets(dst, BUFFER_SIZE, this->fileStream) == NULL){
		printf("NULL!!");
	}
}
void ckOneLine(char* oneLine) {
	int commentIsReal = 0;
	int commentRange = 0;
	for(; oneLine[commentRange] != '\0'; commentRange++) {
		if((oneLine[commentRange] == '/') && (oneLine[commentRange + 1] == '/')){//주석을 만나면 빠져나감. 
			commentIsReal = 1;
				break;
		}	
	}
	if(commentIsReal == 1) {
	for(; oneLine[commentRange] != '\0'; commentRange++) {
		oneLine[commentRange] = ' ';//oneLine이 /라고 생각해서 \0전까지 전부 공백을 넣는다.
	}
}
}//한개의 문장이 끝났음.

void _removeComment(file* this) {
	char buffer[BUFFER_SIZE];
	while(!feof(this->fileStream)) {
		this->readOneLine(buffer, this);
		ckOneLine(buffer);
		printf("%s", buffer);
	}
}
file* fileInit(file* this) { //void로 하면 segfault가 발생하는데 이유를 모르겠음.
	this = (file*)malloc(sizeof(file));
	this->fileOpen = _fileOpen;
	this->readOneLine = _readOneLine;
	this->removeComment = _removeComment;

	return this;
}

void main(void) {
	file* test1 = fileInit(test1);
	test1->fileOpen("testtest.c", test1);
	char buffer[BUFFER_SIZE] ="";
	test1->removeComment(test1);
}

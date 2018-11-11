#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef struct _file {
	FILE* fileStream;
	void (*fileOpen) (char*, struct _file*, const char*);
	void (*readOneLine) (char*, struct _file*);
	void (*writeOneLine) (char*, struct _file*);
	void (*fileReopen) (char*, struct _file*);

	void (*removeComment) (struct _file*, struct _file*);
}file;

void _fileOpen(char* filePath, file* this, const char* mode) {
	if ((this->fileStream = fopen(filePath, mode)) == NULL) { //a면 커서가 파일의 끝으로 이동한 상태로 open한다.
		return;
	}
}

void _fileClose(file* this) {
	fclose(this->fileStream);
}

void _readOneLine(char* dst, file* this) {
	if (fgets(dst, BUFFER_SIZE, this->fileStream) == NULL){
		return;
	}
}

void _writeOneLine(char* dst, file* this) {
	if (fputs(dst, this->fileStream) < 0) {
		return;
	}
}
void ckOneLine(char* oneLine) {
	int commentIsReal = 0;
	int commentRange = 0;

	for(; oneLine[commentRange] != '\n'; commentRange++) {
		if((oneLine[commentRange] == '/') && (oneLine[commentRange + 1] == '/')){//주석을 만나면 빠져나감. 
			commentIsReal = 1;
				break;
		}	
	}

	if(commentIsReal == 1) {
		for(; oneLine[commentRange] != '\n'; commentRange++) {
		oneLine[commentRange] = ' ';//oneLine이 /라고 생각해서 \0전까지 전부 공백을 넣는다.
		}
	}
}//한개의 문장이 끝났음.

void _removeComment(file* src, file* dst) {
	char buffer[BUFFER_SIZE];

	do {
		src->readOneLine(buffer, src);
		ckOneLine(buffer);
		dst->writeOneLine(buffer, dst);
		printf("%s", buffer);

	}while(!feof(src->fileStream));
}

file* fileInit(file* this) { //void로 하면 segfault가 발생하는데 이유를 모르겠음.
	this = (file*)malloc(sizeof(file));
	this->fileOpen = _fileOpen;
	this->readOneLine = _readOneLine;
	this->writeOneLine = _writeOneLine;
	this->removeComment = _removeComment;

	return this;
}

void main(void) {
	file* test1 = fileInit(test1);
	test1->fileOpen("toto.c", test1, "r");

	file* test2 = fileInit(test2);
	test2->fileOpen("nocommentver.c", test2, "w");

	test1->removeComment(test1, test2);
}

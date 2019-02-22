#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Init.h"

int checkHex(char * data, char * error){
	int len = strlen(data);
	char a, f;
	if(len %2 !=0)
	return -1;
	if(strcmp(getCharsize(), "small") == 0){
		a = 'a';
		f = 'f';
	}
	if(strcmp(getCharsize(), "big") == 0){
		a = 'A';
		f = 'F';
	}
	for(int i=0; i<len; i++){
		if(data[i] < '0' || (data[i] > '9' && data[i] < a) || data[i]>f){
			printf("Wrong %s character on position %d, maybe try --charset option\n", error, i);
			return -1;
		}
	}
	return 0;
}

int checkHash(char *data){
	if(strlen(data) != 32){
		printf("Wrong hash length!\n");
		return -1;
	}
	return 0;
}

int optionHelp(char ** argv, int argc, int i){
	puts("Welcome to hash breaker!\n");
	puts("Usage:");
	puts("./HC [OPTIONS] hex_hash");
	puts("Valid options for aplication are:");
	puts("		--prefix prefix of word that md5 hash is specified (in hex)");
	puts("		--suffix suffix of word that md5 hash is specified (in hex)");
	puts("		--charsize small|big size of hex digits (a-f or A-F)");
	puts("		--charset possible chars in result string (ex. abcdefghijklmnopqrstuvwxyz)");
	return -1;
}

int optionPrefix(char ** argv, int argc, int i){
	if(i+1<argc){
		HC_prefix = argv[i+1];
		return 1;
	}
	else{
		puts("Wrong command line parameter --prefix, no string sprecified");
		return -1;
	}
}

int optionSuffix(char ** argv, int argc, int i){
	if(i+1<argc){
		HC_suffix = argv[i+1];
		return 1;
	}
	else{
		puts("Wrong command line parameter --suffix, no string sprecified");
		return -1;
	}
}

int optionCharsize(char ** argv, int argc, int i){
	if(i+1<argc){
		if((strcmp(argv[i+1], "big") == 0) || (strcmp(argv[i+1], "small") == 0)){
			HC_charsize = argv[i+1];
			return 1;
		}
		else{
			puts("Wrong command line parameter --charsize, only big or small allowed");
			return -1;
		}
	}
	else{
		puts("Wrong command line parameter --charsize, no string sprecified");
		return -1;
	}
}

int optionCharset(char ** argv, int argc, int i){
	if(i+1<argc){
		HC_charset = argv[i+1];
		return 1;
	}
	else{
		puts("Wrong command line parameter --charset, no string sprecified");
		return -1;
	}
}

void convertAF(char* data){
	int len = strlen(data);
	for(int i=0; i<len; i++){
		if(data[i]>='A' && data[i]<='F')
		data[i] += 'a' - 'A';
	}
	return;
}

void convertFromHex(char **data){
	int len = strlen(*data);
	char *res = (char*)malloc(len/2+1);
	for(int i=0; i<len; i+=2){
		if((*data)[i]>'9' && (*data)[i+1]>'9'){
			res[i/2] = 16*((*data)[i]-'a'+10)+(*data)[i+1]-'a'+10;
		}
		if((*data)[i]<='9' && (*data)[i+1]>'9'){
			res[i/2] = 16*((*data)[i]-'0')+(*data)[i+1]-'a'+10;
		}
		if((*data)[i]>'9' && (*data)[i+1]<='9'){
			res[i/2] = 16*((*data)[i]-'a'+10)+(*data)[i+1]-'0';
		}
		if((*data)[i]<='9' && (*data)[i+1]<='9'){
			res[i/2] = 16*((*data)[i]-'0')+(*data)[i+1]-'0';
		}
	}
	res[len/2] = '\x00';
	*data = res;
	return;
}

typedef int (*optFuncDef)(char**, int, int);
int HC_Init(int argc, char* argv[]){
	int a = -2, b = 0;
	char * options[] = {(char*)"--help", (char*)"--prefix", (char*)"--suffix", (char*)"--charsize", (char*)"--charset"};
	optFuncDef optionsFunctions[] = {optionHelp, optionPrefix, optionSuffix, optionCharsize, optionCharset};
	int checked[] = {0, 0, 0, 0, 0};
	for(int i=1; i<argc; i++){
		a = -2;
		for(int j=0; j<5; j++){
			if(strcmp(argv[i], options[j]) == 0){
				if(checked[j] == 1){
					printf("Double option declaration %s\n", argv[i]);
					return -1;
				}
				a = (*(optionsFunctions[j]))(argv, argc, i);
				if(a == -1)
				return 1;
				i+=a;
				checked[j] = 1;
				break;
			}
		}
		if(a == -2 && i<argc){
			if(b == 0){
				if(checkHash(argv[i]) == 0){
					HC_hash = argv[i];
					b = 1;
				}
				else{
					return 1;
				}
			}
			else{
				printf("Not recognized argument %s\n", argv[i]);
				return 1;
			}
		}
	}
	if(b == 0){
		puts("No hash sprecified!");
		return 1;
	}
	if(checkHex(HC_hash, (char*)"hash") == -1)
		return 1;
	if(checkHex(HC_prefix, (char*)"prefix") == -1)
		return 1;
	if(checkHex(HC_suffix, (char*)"suffix") == -1)
		return 1;
	convertAF(HC_hash);
	convertAF(HC_prefix);
	convertAF(HC_suffix);
	HC_preflen = strlen(HC_prefix)/2;
	HC_sufflen = strlen(HC_suffix)/2;
	//printf("%d\n", HC_preflen);
	convertFromHex(&HC_prefix);
	convertFromHex(&HC_suffix);
	if(strcmp(HC_charset, "all") == 0){
		HC_charset = (char*)malloc(256);
		for(int i=0; i<256; i++){
			HC_charset[i] = i;
		}
	}
	else{
		HC_charsetlen = strlen(HC_charset);
	}
	return 0;
}

char* getPrefix(){
	return HC_prefix;
}

int getPrefLen(){
	return HC_preflen;
}

char* getSuffix(){
	return HC_suffix;
}

int getSuffLen(){
	return HC_sufflen;
}

char* getHash(){
	return HC_hash;
}

char* getCharsize(){
	return HC_charsize;
}

char* getCharset(){
	return HC_charset;
}

int getCharsetLen(){
	return HC_charsetlen;
}

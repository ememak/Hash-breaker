#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include"md5.c"
#include"gpu.h"
#define MAX_PREF 10
#define MAX_SUFF 10
#define MAX_LEN 32
#define uint unsigned int

int increment(unsigned char *word, int len, int charsetlen, int inc){
	int i=len-1;
	while(inc > 0 && i >= 0){
		int add = inc + word[i];
		word[i] = add%charsetlen;
		inc = add/charsetlen;
		i--;
	}
	return inc > 0;
}

int HC_calc(char *hash, char *pref, int lp, char *suff, int ls, char *charset, int lc, int len){
	HC_res = (char*)malloc(len + 1);
	HC_res[len] = '\x00';
	int ok = 0;
	#pragma omp parallel
	{
		int fin = 0, size = omp_get_num_threads(), rank = omp_get_thread_num(), c = 0;
		char *word = (char*)malloc(lp + len + ls), *res = (char*)malloc(32), *brut = (char*)malloc(len);
		for(int i=0; i<32; i++)
		res[i] = 0;
		for(int i=0; i<lp; i++)
		word[i] = pref[i];
		for(int i=0; i<ls; i++)
		word[i + lp + len] = suff[i];
		for(int i=0; i<len; i++)
		brut[i] = 0;
		increment((unsigned char*)brut, len, lc, rank);
		while(fin == 0 && ok == 0){
			for(int i=0; i<len; i++)
			word[i + lp] = charset[brut[i]];
			res =  md5((unsigned char*)word, lp + len + ls);
			for(int i=0; i<32; i++){
				if(res[i] != hash[i]){
					break;
				}
				if(i == 31){
					ok = 1;
					fin = 1;
					for(int j=0; j<len; j++)
					HC_res[j] = word[j+lp];
				}
			}
			//if((c%(128*256*512*size)) == 0){
			if((c%(128*256*4*size)) == 0){
				printf("CALCULATED: ");
				for(int i=0; i<len; i++)
				printf("%d ", brut[i]);
				printf("\n");
			}
			if(ok == 1 && fin == 1){
				for(int i=0; i<len; i++)
				HC_res[i] = word[lp + i];
				break;
			}
			fin = increment((unsigned char*)brut, len, lc, size);
			c++;
		}
		free(brut);
		free(word);
		free(res);
	}
	if(ok == 0)
	free(HC_res);
	return ok;
}

char *getRes(){
	return HC_res;
}

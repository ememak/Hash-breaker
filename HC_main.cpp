#include<stdio.h>
#include<string.h>
#include<time.h>

#include "Init.h"
#ifdef CPU_CALC
#include "cpu.h"
#endif
#ifdef GPU_CALC
#include "gpu.h"
#endif
#define MAX_N 12

struct timespec start, finish;
double elapsed;
int main(int argc, char *argv[]){
	if(HC_Init(argc, argv) == 1){
		return 0;
	}
	int lp = getPrefLen(), ls = getSuffLen();
	printf("Init end!, prefix = %s, suffix = %s, charset = %s\n", getPrefix(), getSuffix(), getCharset());
	printf("Init end!, prefixlen = %d, suffixlen = %d, charsetlen = %d\n", getPrefLen(), getSuffLen(), getCharsetLen());
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int len = 1; len<MAX_N; len++){
		int r = HC_calc(getHash(), getPrefix(), lp, getSuffix(), ls, getCharset(), getCharsetLen(), len);
		if(r == 1){
			clock_gettime(CLOCK_MONOTONIC, &finish);
			elapsed = (finish.tv_sec - start.tv_sec);
			elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
			printf("Calculation time = %f\n", elapsed);
			char *res = getRes();
			printf("Example string with your hash, prefix and suffix is: ");
			for(int i=0; i<lp; i++)
			printf("%c", getPrefix()[i]);
			for(int i=0; i<len; i++)
			printf("%c", res[i]);
			for(int i=0; i<ls; i++)
			printf("%c", getSuffix()[i]);
			printf(" (");
			for(int i=0; i<lp; i++)
			printf("%d ", getPrefix()[i]);
			for(int i=0; i<len; i++)
			printf("%d ", res[i]);
			for(int i=0; i<ls; i++)
			printf("%d ", getSuffix()[i]);
			printf(")\n");
			return 0;
		}
	}
	printf("No string found\n");
	return 0;
}


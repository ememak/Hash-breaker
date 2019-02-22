#include<stdlib.h>
#include<stdio.h>
#include"md5.cu"
#include"gpu.h"
#define NUM_BLOCKS 128
#define THREADS_PER_BLOCK 256
#define HASH_PER_THREAD 512
#define MAX_N 12
#define MAX_PREF 10
#define MAX_SUFF 10
#define MAX_LEN 32
#define uint unsigned int

__device__ __constant__ char prefix[MAX_PREF];
__device__ __constant__ int lenPref;
__device__ __constant__ char suffix[MAX_SUFF];
__device__ __constant__ int lenSuf;
__device__ __constant__ char start[MAX_N];

__device__ void dincrement(unsigned char *word, int len, int charsetlen, int inc){
	int i=len-1;
	while(inc > 0 && i >= 0){
		int add = inc + word[i];
		word[i] = add%charsetlen;
		inc = add/charsetlen;
		i--;
	}
}

__global__ void kernel(int len, int numThreads, int md5PerThread, char *res){
	int tid = threadIdx.x + blockDim.x * blockIdx.x;
	/*for(int i=0; i<md5PerThread; i++){
		if(tid == 0 && i%1 ==0)
		printf("%d %d\n", i, md5PerThread);
	}*/
	printf("GPU: %d %s %s %d %d %s\n", tid, prefix, suffix, lenPref, lenSuf, start);
	return;
}

__global__ void calcKernel(uint h0, uint h1, uint h2, uint h3, int len,
														 int numThreads, int md5PerThread, char *res,
 														 char *charset, int charsetlen){
	int tid = threadIdx.x + blockDim.x * blockIdx.x, lp = lenPref, ls = lenSuf;
	unsigned char myBrut[MAX_LEN], word[MAX_LEN];
	for(int i=0; i<lp; i++){
		word[i] = prefix[i];
	}
	for(int i=0; i<ls; i++){
		word[i + lp + len] = suffix[i];
	}
	for(int i=0; i<len; i++){
		myBrut[i] = start[i];
	}
	dincrement(myBrut, len, charsetlen, tid);
	uint mh0, mh1, mh2, mh3;
	int i;
	for(i=0; i<md5PerThread; i++){
		for(int j=0; j<len; j++){
			word[j+lp] = charset[myBrut[j]];
		}
		mh0 = 0, mh1 = 0, mh2 = 0, mh3 = 0;
		md5_vfy(word, lp+ls+len, &mh0, &mh1, &mh2, &mh3);
		if(h0 == mh0 && h1 == mh1 && h2 == mh2 && h3 == mh3){
			printf("%s\n", word);
			for(int j=0; j<len; j++){
				res[j] = charset[myBrut[j]];
			}
		}
		dincrement(myBrut, len, charsetlen, numThreads);
	}
	return;
}

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
	int fin = 0;
	char *d_res, *d_charset, *st = (char*)malloc(len + 1);
	cudaMemcpyToSymbol(prefix, pref, lp, 0, cudaMemcpyHostToDevice);
	cudaMemcpyToSymbol(suffix, suff, ls, 0, cudaMemcpyHostToDevice);
	
	cudaMemcpyToSymbol(lenPref, &lp, 4, 0, cudaMemcpyHostToDevice);
	cudaMemcpyToSymbol(lenSuf, &ls, 4, 0, cudaMemcpyHostToDevice);

	cudaMalloc((void**)&d_res, len);
	cudaMalloc((void**)&d_charset, lc);
	HC_res = (char*)malloc(len);
	for(int i=0; i<len; i++)
	HC_res[i] = 0;
	for(int i=0; i<len; i++)
	st[i] = 0;
	st[len] = '\x00';
	cudaMemcpy(d_res, HC_res, len, cudaMemcpyHostToDevice);
	cudaMemcpy(d_charset, charset, lc, cudaMemcpyHostToDevice);
	uint iH0, iH1, iH2, iH3;
	md5_to_ints((unsigned char*)hash, &iH0, &iH1, &iH2, &iH3);
	while(fin == 0){
		cudaMemcpyToSymbol(start, st, len, 0, cudaMemcpyHostToDevice);
		calcKernel<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(iH0, iH1, iH2, iH3, len, NUM_BLOCKS*THREADS_PER_BLOCK, HASH_PER_THREAD, d_res, d_charset, lc);
		fin = increment((unsigned char*)st, len, lc, NUM_BLOCKS*THREADS_PER_BLOCK*HASH_PER_THREAD);
		//kernel<<<NUM_BLOCKS, THREADS_PER_BLOCK>>>(len, NUM_BLOCKS*THREADS_PER_BLOCK, HASH_PER_THREAD, d_res);
		cudaMemcpy(HC_res, d_res, len, cudaMemcpyDeviceToHost);
		printf("CALCULATED: ");
		for(int i=0; i<len; i++)
		printf("%d ", st[i]);
		printf("\n");
		for(int i=0; i<len; i++){
			if(HC_res[i] != 0){
				return 1;
			}
		}
	}
	cudaFree(d_res);
	cudaFree(d_charset);
	free(HC_res);
	return 0;
}

char *getRes(){
	return HC_res;
}

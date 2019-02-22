#ifndef HC_GPU_H
#define HC_GPU_H
int HC_calc(char *hash, char *prefix, int lp, char *suffix, int ls, char *charset, int lc, int len);
char *getRes();
static char *HC_res;
#endif

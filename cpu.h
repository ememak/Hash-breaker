#ifndef HC_CPU_H
#define HC_CPU_H
int HC_calc(char *hash, char *prefix, int lp, char *suffix, int ls, char *charset, int lc, int len);
char *getRes();
static char *HC_res;
#endif

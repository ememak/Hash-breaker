#ifndef HC_INIT_H
#define HC_INIT_H
int HC_Init(int argc, char* argv[]);
char* getPrefix();
int getPrefLen();
char* getSuffix();
int getSuffLen();
char* getHash();
char* getCharsize();
char* getCharset();
int getCharsetLen();

static char *HC_prefix = (char*)"";
static int HC_preflen = 0;
static char *HC_suffix = (char*)"";
static int HC_sufflen = 0;
static char *HC_hash = (char*)"";
static char *HC_charsize = (char*)"small";
static char *HC_charset = (char*)"all";
static int HC_charsetlen = 256;

#endif

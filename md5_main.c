/*
 * Simple MD5 implementation
 *
 * Code from https://gist.github.com/creationix/4710780
 * 
 * Compile with: gcc -o md5 -O3 -lm md5.c
 */
#include <stdio.h>
#include "md5.c"
 
int main(int argc, char **argv) {
 
    if (argc < 2) {
        printf("usage: %s 'string'\n", argv[0]);
        return 1;
    }
 
    char *msg = argv[1];
    size_t len = strlen(msg);
 
		printf("%s\n", md5(msg, len));
    return 0;
}

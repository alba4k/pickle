#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pickle.h"

int main(int argc, char **argv) {
    char *string = NULL;

    if(argc-1) {
        if(!access(argv[1], F_OK)) {    // is that a file?
            FILE *fp = fopen(argv[1], "r");

            fseek(fp, 0, SEEK_END);
            size_t len = ftell(fp);
            rewind(fp);

            string = malloc(len+1);
            string[fread(string, 1, len, fp)] = 0;
            fclose(fp);

            goto done;
            } else {
            string = malloc(0x4000);
            string[0] = 0;

            for(int i = 1; i <= argc-1; ++i) {
                strncat(string, argv[i], 0x4000-1-strlen(string));
                if(i < argc-1)
                    strncat(string, " ", 0x4000-1-strlen(string));
            }

            goto done;
        }
    }

    string = malloc(0x4000);
    fgets(string, 0x4000-1, stdin);

    done: ;

    char buf[7];
    char c = string[0];
    encode(c, buf);
    printf(" encoded '%c' into \"%s\"\n", c, buf);

    return 0;
}

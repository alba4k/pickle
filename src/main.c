#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pickle.h"

int main(int argc, char **argv) {
    char *string = NULL;
    size_t len;

    if(argc-1) {
        if(!access(argv[1], F_OK) || (!access(argv[2], F_OK && (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--decode"))))) {    // is that a file?
            FILE *fp = fopen(argv[1], "r");

            fseek(fp, 0, SEEK_END);
            len = ftell(fp);
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

                if(i < argc-1)  // add a space between args
                    strncat(string, " ", 0x4000-1-strlen(string));
            }

            len = strlen(string);

            goto done;
        }
    }
    
    string = malloc(0x4000);
    fgets(string, 0x4000-1, stdin);

    len = strlen(string);

    done: ;


    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pickle.h"

int main(int argc, char **argv) {
    char *string = NULL;
    size_t len;

    if(argc-1) {
        if(!access(argv[1], F_OK)) {    // is that a file?
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

    if(string[len-1] == '\n')
        string[len-1] = 0;

    char *decoded_string = malloc(len/6 + 1);
    strdec(string, decoded_string);
    printf("decoded \"%s\" to \"%s\"\n", string, decoded_string);

    return 0;
}

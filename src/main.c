#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "pickle.h"

int main(int argc, char **argv) {
    char *string = NULL;
    size_t len;
    bool decode = false;

    for(int i = 1; i < argc; ++i) {
        if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            puts("\e[1m\e[32mpickle\e[0m - encode or decode text into pickle encoding");
            puts("\n\e[1m\e[32mUSAGE\e[0m:");
            puts("\tpickle [FLAGS] [FILE] ~ input can be given from a file or stdin.");
            puts("\n\e[1m\e[32mFLAGS\e[0m:");
            puts("\t\e[1m\e[32m-h\e[0m, \e[1m\e[32m--help\e[0m\t\tPrint this help message.");
            puts("\t\e[1m\e[32m-d\e[0m, \e[1m\e[32m--decode\e[0m\t\tDecode the given input instead of encoding it.");

            return 0;
        } else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--decode")) {
            decode = true;
        } else if(!access(argv[i], F_OK)) {    // is that a file?
            FILE *fp = fopen(argv[1], "r");
            if(!fp) {
                fprintf(stderr, "\e[1m\e[31mPickle Error\e[0m: Unable to open %s", argv[i]);
                return 1;
            }

            fseek(fp, 0, SEEK_END);
            len = ftell(fp);
            rewind(fp);

            string = malloc(len+1);
            string[fread(string, 1, len, fp)] = 0;
            fclose(fp);

            printf("%s\n", string);
        } else {
            fprintf(stderr, "\e[1m\e[31mPickle Error\e[0m: \"%s\": No such file or directory", argv[i]);
            return 1;
        }
    }

    if(!string) {
        string = malloc(0x4000);
        fgets(string, 0x4000-1, stdin);

        len = strlen(string);
    }

    if(decode) {
        len /= 6;

        char *decoded_string = malloc(len+1);

        strndec(string, decoded_string, len);

        printf("%s\n", decoded_string);
        return 0;
    }

    char *encoded_string = malloc(len*6 + 1);

    strnenc(string, encoded_string, len);

    printf("%s\n", encoded_string);
    return 0;
}

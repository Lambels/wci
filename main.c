#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char const *);

int main(int argc, char *argv[]) {
    // for (int i = 0; i <= argc; ++i)
    // {
    //     printf("Value %d: %s\n", i, argv[i]);
    //     printf("Equal to null? %d\n", argv[i] == NULL); // boolean, char, short -> int when called
    //                                                     // in variadic variables.
    // }
    int ch;
    char *ingnoreseq = NULL;

    while ((ch = getopt(argc, argv, "i:")) != -1) {
        switch (ch) {
            case 'i':
                ingnoreseq = optarg;
                break;
            case '?':
            default:
                usage("");
                break;
        }
    }
    argc -= optind; // decrement the number of arguments by the amount read.
    argv += optind; // advance using pointer arithmetic the argv pointer to point to the
                    // optind index.

    if (argc != 1) usage("Expected 1 positional argument: file\n");

    FILE *fp = fopen(argv[0], "r");
    if (fp == NULL) perror("Error while opening the file");
}

void usage(char const *ctx) {
    printf("%s", ctx);
    printf("USAGE: wci [-i ignore] file\n");
    exit(1);
}

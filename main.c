#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void usage(char const *);
int isignore(int);

static FILE *fp;
static char *ignore;

int main(int argc, char *argv[]) {
    // for (int i = 0; i <= argc; ++i)
    // {
    //     printf("Value %d: %s\n", i, argv[i]);
    //     printf("Equal to null? %d\n", argv[i] == NULL); // boolean, char, short -> int when called
    //                                                     // in variadic variables.
    // }
    int ch;

    while ((ch = getopt(argc, argv, "i:")) != -1) {
        switch (ch) {
            case 'i':
                ignore = optarg;
                break;
            case '?':
            default:
                usage("");
                break;
        }
    }
    argc -= optind; // set the new argument count.
    argv += optind; // advance using pointer arithmetic the argv pointer to point to the
                    // optind index.

    if (argc != 1) usage("Expected only 1 positional argument: file\n");

    fp = fopen(argv[0], "r");
    if (fp == NULL) perror("Error while opening the file");

    int wordc, charc, linec, ignorec;
    wordc = charc = linec = ignorec = 0;
    bool inword = false;
    bool newline = true;
    while (!ferror(fp) && !feof(fp)) {
        ch = fgetc(fp);
        charc++;

        // We are still in a newline and hit an ignore sequence, we ignore the line.
        if (newline && isignore(ch)) {
            ignorec++;
            newline = false;
        }

        // We are in a newline and hit a character which isnt part of an ignore sequence
        // we leave the newline state.
        if (newline && !isspace(ch)) newline = false;

        // word count logic.
        if (inword && isspace(ch)) {
            inword = false;
            wordc++;
        } else {
            inword = true;
        }
    }

    printf("\t%d\t%d\t%d\t%d %s\n", linec, wordc, charc, ignorec, argv[0]);
}

int isignore(int ch) {
    // FASTPATH: no ignore sequence is set, return false.
    if (ignore == NULL) return 0;

    // FASTPATH: first char doesnt match.
    if (ignore[0] != ch) return 0;

    // If length is 1 then we return true since we know that
    // length cannot be 0 and first char matches.
    if (ignore[1] == 0) return 1;

    int i = 0;
    int match = 1;
    while ((ch = ignore[i+1]) != 0) {
        i++;
        if (ch != fgetc(fp)) {
            match = 0;
            break;
        }
    }

    // rewind the pointer to re process the stream with our full state machine.
    if (fseek(fp, -i, SEEK_CUR)) {
        perror("");
        return -1;
    }

    return match;
}

void usage(char const *ctx) {
    printf("%s", ctx);
    printf("USAGE: wci [-i ignore] file\n");
    exit(1);
}

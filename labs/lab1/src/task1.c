/* --------------------------------------------
 * Subject: Introduction to CS and Programming
 * List: Lab 1
 * Task: Task 1
 * Author: Denis Stocki
 * --------------------------------------------
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_MAX_LEN 50

static void printUsage(
    const char* const progName
) {
    fprintf(stderr, "Usage: %s [-w <word>]\n", progName);
    fprintf(stderr, "\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -w <word>\tSpecifies a word which is used to print a triangle.\n");
    fprintf(stderr, "           \tIf omitted, program runs in interactive mode.\n");
}

static void printWord(
    const char* const w,
    const size_t l
) {
    for (size_t i = 0; i < l; ++i) {
        printf("%c ", w[i]);
    }
    printf("\n");
}

static void printSpaces(
    const size_t count
) {
    if (count == 0) return;
    printf("%*s", (int)count, "");
}

static void printTriangle(
    const char* const wordToPrint
) {
    size_t l = strlen(wordToPrint);
    
    for (size_t i = 0; i < l; ++i) {
        printSpaces(i);
        printWord(wordToPrint, l - i);
    }
}

static bool includesWhitespaces(
    const char* const buff
) {
    for (size_t i = 0; buff[i] != '\0'; ++i) {
        if (isspace((unsigned char)buff[i])) return true;
    }

    return false;
}

static char* dupString(
    const char* s
) {
    const size_t sLen = strlen(s);
    char* dup = malloc(sLen + 1);
    if (dup == NULL) {
        fprintf(stderr, "Failed to allocate memory!\n");
        return NULL;
    }

    strcpy(dup, s);
    return dup;
}

static bool getWord(
    char** const word,
    const int argc,
    const char** const argv,
    const bool useInteractiveMode
) {
    if (useInteractiveMode) {
        printf("Word (confirm with enter): ");
        fflush(stdout); // Ensures printf finishes before fgets starts

        char buffer[BUFFER_MAX_LEN];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Failed to read input!\n");
            return false;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0) {
            fprintf(stderr, "Input cannot be empty!\n");
            return false;
        }

        if (includesWhitespaces(buffer)) {
            fprintf(stderr, "Input cannot contain whitespaces!\n");
            return false;
        }

        char* copy = dupString(buffer);
        if (copy == NULL) return false;

        *word = copy;
    } else {
        if (argc != 3) {
            printUsage(argv[0]);
            return false;
        }

        if (strcmp(argv[1], "-w") != 0) {
            printUsage(argv[0]);
            return false;
        }

        if (argv[2][0] == '\0') {
            fprintf(stderr, "Input cannot be empty!\n");
            return false;
        }

        if (includesWhitespaces(argv[2])) {
            fprintf(stderr, "Input cannot contain whitespaces!\n");
            return false;
        }

        char* copy = dupString(argv[2]);
        if (copy == NULL) return false;

        *word = copy;
    }

    return true;
}

int main(
    const int argc,
    const char** const argv
) {
    const bool useInteractiveMode = (argc == 1);
    char* wordToPrint = NULL;

    if (!getWord(&wordToPrint, argc, argv, useInteractiveMode)) {
        return EXIT_FAILURE;
    }

    if (useInteractiveMode) printf("\n");
    printTriangle(wordToPrint);

    free(wordToPrint);
    return EXIT_SUCCESS;
}

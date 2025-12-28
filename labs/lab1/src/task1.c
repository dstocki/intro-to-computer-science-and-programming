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

#define MAX_BUFF_LEN 100

static bool printPrefix(
    const char* const word,
    const size_t nChars
) {
    const size_t maxChars = strlen(word);
    
    if (nChars > maxChars)
        return false;

    for (size_t i = 0; i < nChars; ++i)
        printf("%c ", word[i]);
    printf("\n");

    return true;
}

static void printSpaces(
    const size_t amount
) {
    if (amount == 0)
        return;

    printf("%*s", (int)amount, "");
}

static bool printTriangle(
    const char* const word
) {
    size_t nChars = strlen(word);
    
    for (size_t i = 0; i < nChars; ++i) {
        printSpaces(i);
        if (!printPrefix(word, nChars - i))
            return false;
    }

    return true;
}

static bool hasSpaces(
    const char* const s
) {
    const size_t nChars = strlen(s);

    for (size_t i = 0; i < nChars; ++i)
        if (isspace((unsigned char)s[i]))
            return true;

    return false;
}

static char* dupString(
    const char* s
) {
    const size_t nChars = strlen(s);

    char* dup = malloc(nChars + 1);
    if (dup == NULL)
        return NULL;

    strcpy(dup, s);
    return dup;
}

static bool getWord(
    char** const word
) {
    char buffer[MAX_BUFF_LEN];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return false;

    buffer[strcspn(buffer, "\n")] = '\0';

    if (hasSpaces(buffer))
        return false;

    char* const copy =
        dupString(buffer);

    if (copy == NULL)
        return false;

    *word = copy;
    return true;
}

int main(
    void
) {
    char* wordToPrint;
    if (!getWord(&wordToPrint))
        return EXIT_FAILURE;

    if (!printTriangle(wordToPrint))
        return EXIT_FAILURE;

    free(wordToPrint);
    return EXIT_SUCCESS;
}

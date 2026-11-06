/* validation.c - Shared input validation and string helper functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validation.h"

/* Remove leftover characters up to end of line */
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Read a line from the keyboard and strip the newline */
static int readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return 0;
    }
    if (strchr(buffer, '\n') != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        clearInputBuffer();          /* input was longer than buffer */
    }
    return 1;
}

/* Read an integer between min and max (inclusive), re-prompting on error */
int readInt(const char *prompt, int min, int max)
{
    char buffer[64];
    char *end;
    long value;

    while (1) {
        printf("%s", prompt);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }
        value = strtol(buffer, &end, 10);
        if (end == buffer || *end != '\0') {
            printf("  [ERROR] Please enter a whole number.\n");
        } else if (value < min || value > max) {
            printf("  [ERROR] Value must be between %d and %d.\n", min, max);
        } else {
            return (int)value;
        }
    }
}

/* Read a non-negative number; zero allowed only if allowZero is 1 */
double readPositiveDouble(const char *prompt, int allowZero)
{
    char buffer[64];
    char *end;
    double value;

    while (1) {
        printf("%s", prompt);
        if (!readLine(buffer, sizeof(buffer))) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }
        value = strtod(buffer, &end);
        if (end == buffer || *end != '\0') {
            printf("  [ERROR] Please enter a valid number (e.g. 15000.50).\n");
        } else if (value < 0) {
            printf("  [ERROR] Negative values are not accepted.\n");
        } else if (value == 0 && !allowZero) {
            printf("  [ERROR] Value must be greater than zero.\n");
        } else {
            return value;
        }
    }
}

/* Read a string that is not empty and not only spaces */
void readNonEmptyString(const char *prompt, char *dest, int size)
{
    int i, onlySpaces;

    while (1) {
        printf("%s", prompt);
        if (!readLine(dest, size)) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }
        onlySpaces = 1;
        for (i = 0; dest[i] != '\0'; i++) {
            if (!isspace((unsigned char)dest[i])) {
                onlySpaces = 0;
                break;
            }
        }
        if (strlen(dest) == 0 || onlySpaces) {
            printf("  [ERROR] This field cannot be empty.\n");
        } else {
            return;
        }
    }
}

/* Simple email check: one '@' and a '.' after it */
void readEmail(const char *prompt, char *dest, int size)
{
    char *at;
    while (1) {
        readNonEmptyString(prompt, dest, size);
        at = strchr(dest, '@');
        if (at != NULL && at != dest && strchr(at, '.') != NULL
            && strchr(at + 1, '@') == NULL && dest[strlen(dest) - 1] != '.') {
            return;
        }
        printf("  [ERROR] Invalid email format (example: info@company.na).\n");
    }
}

/* Phone may contain digits, spaces, '+' and '-' and must have 7-15 digits */
void readPhone(const char *prompt, char *dest, int size)
{
    int i, digits, valid;
    while (1) {
        readNonEmptyString(prompt, dest, size);
        digits = 0;
        valid = 1;
        for (i = 0; dest[i] != '\0'; i++) {
            if (isdigit((unsigned char)dest[i])) {
                digits++;
            } else if (dest[i] != ' ' && dest[i] != '+' && dest[i] != '-') {
                valid = 0;
            }
        }
        if (valid && digits >= 7 && digits <= 15) {
            return;
        }
        printf("  [ERROR] Invalid phone number (example: +264 61 123 4567).\n");
    }
}

/* Copy src into dest converted to upper case */
void toUpperCase(const char *src, char *dest, int size)
{
    int i;
    for (i = 0; src[i] != '\0' && i < size - 1; i++) {
        dest[i] = (char)toupper((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

/* Returns 1 if keyword appears in text (case-insensitive) */
int containsIgnoreCase(const char *text, const char *keyword)
{
    char upperText[TEXT_LEN * 2];
    char upperKey[TEXT_LEN * 2];
    toUpperCase(text, upperText, sizeof(upperText));
    toUpperCase(keyword, upperKey, sizeof(upperKey));
    return strstr(upperText, upperKey) != NULL;
}

void pauseScreen(void)
{
    printf("\nPress ENTER to continue...");
    clearInputBuffer();
}

void printLine(void)
{
    printf("------------------------------------------------------------------------\n");
}

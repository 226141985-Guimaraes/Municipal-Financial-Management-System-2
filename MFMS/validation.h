/* validation.h - Input reading and validation helpers */
#ifndef VALIDATION_H
#define VALIDATION_H

#define NAME_LEN 50
#define TEXT_LEN 60

void clearInputBuffer(void);
int readInt(const char *prompt, int min, int max);
double readPositiveDouble(const char *prompt, int allowZero);
void readNonEmptyString(const char *prompt, char *dest, int size);
void readEmail(const char *prompt, char *dest, int size);
void readPhone(const char *prompt, char *dest, int size);
void toUpperCase(const char *src, char *dest, int size);
int containsIgnoreCase(const char *text, const char *keyword);
void pauseScreen(void);
void printLine(void);

#endif

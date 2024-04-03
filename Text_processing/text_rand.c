#include "rand_malloc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeText(char **, int, int);
void printReversedWords(char **, int);
void reverseWords(char **, int);
void expandLine(char **, int);
void expandText(char ***, int);
char *concatenateWords(char **, int, int);
char *getLine(void);
char **getWholeText(int *);

char *strdup(const char *s)
{
    char *p = 0;
    p = malloc(strlen(s) + 1);
    if (p == NULL)
    {
        errno = ENOMEM;
    }
    if (p)
        strcpy(p, s);
    return p;
}

int main(int argc, char const *argv[])
{
    errno = 0;
    int no_of_lines;
    char **text = getWholeText(&no_of_lines);
    if (text == NULL)
    {
        exit(1);
    }
    reverseWords(text, no_of_lines);
    if (!errno)
    {
        printReversedWords(text, no_of_lines);
    }
    freeText(text, no_of_lines, 0);

    return 0;
}
void freeText(char **text, int no_of_lines, int start)
{
    for (size_t i = start; i < no_of_lines; i++)
    {
        if (text[i] != NULL)
        {
            free(text[i]);
            text[i] = 0;
        }
    }
    free(text);
}
void printReversedWords(char **text, int no_of_lines)
{
    for (int i = no_of_lines - 1; i >= 0; i--)
    {
        printf("%s\n", text[i]);
    }
}
void expandLine(char **line, int size)
{
    char *temp = (char *)realloc(*line, sizeof(char) * (size + 1));
    if (temp == NULL)
    {
        errno = ENOMEM;
    }
    else
    {
        temp[size] = '\0';
        *line = temp;
    }
}
char *getLine()
{
    errno = 0;
    char *line = NULL;
    int len = 0;
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        expandLine(&line, len);
        if (errno == ENOMEM)
        {
            free(line);
            line = 0;
            return NULL;
        }

        line[len] = (char)c;
        len++;
    }
    if (line != NULL || c != EOF)
    {
        expandLine(&line, len);
        if (errno == ENOMEM)
        {
            free(line);
            line = 0;
            return NULL;
        }

        line[len] = '\0';
    }

    return line;
}
void expandText(char ***text, int size)
{
    char **temp = (char **)realloc(*text, sizeof(char *) * (size + 1));
    if (temp == NULL)
    {
        errno = ENOMEM;
    }
    else
    {
        *text = temp;
    }
}
char **getWholeText(int *lines)
{
    char **input = NULL;
    int no_of_lines = 0;
    char *s;
    while ((s = getLine()))
    {
        expandText(&input, no_of_lines);
        if (errno == ENOMEM)
        {
            free(s);
            s = 0;
            freeText(input, no_of_lines, 0);
            return NULL;
        }
        input[no_of_lines] = s;
        no_of_lines++;
    }
    *lines = no_of_lines;
    return input;
}
char **tokenize(char *line, int *word_count)
{
    errno = 0;
    char **words = NULL;
    for (char *s = strtok(line, " "); s != NULL; s = strtok(NULL, " "))
    {
        expandText(&words, *word_count);
        if (errno == ENOMEM)
        {
            freeText(words, *word_count, 0);
            return NULL;
        }
        words[*word_count] = strdup(s);
        if (errno == ENOMEM)
        {
            freeText(words, *word_count, 0);
            return NULL;
        }
        (*word_count)++;
    }
    return words;
}
char *concatenateWords(char **words, int word_count, int word_length)
{
    errno = 0;
    char *buffer_string = malloc(word_length + 1);
    if (buffer_string == NULL)
    {
        errno = ENOMEM;
        return NULL;
    }
    buffer_string[0] = '\0';
    for (int j = word_count - 1; j >= 0; j--)
    {
        strcat(buffer_string, words[j]);
        if (j > 0)
        {
            strcat(buffer_string, " ");
        }
        free(words[j]);
        words[j] = 0;
    }
    return buffer_string;
}
void reverseWords(char **text, int no_of_lines)
{
    errno = 0;
    for (size_t i = 0; i < no_of_lines; i++)
    {
        int word_count = 0;
        char *temp = strdup(text[i]);
        if (temp == NULL && text[i][0] != '\0')
        {
            errno = ENOMEM;
            break;
        }
        char **words = tokenize(temp, &word_count);
        if (errno == ENOMEM)
        {
            free(temp);
            temp = NULL;
            break;
        }
        char *buffer_string = concatenateWords(words, word_count, strlen(text[i]) + 1);
        free(words);
        words = 0;
        if (errno == ENOMEM)
        {
            free(temp);
            temp = NULL;
            break;
        }
        free(temp);
        temp = NULL;
        free(text[i]);
        text[i] = buffer_string;
    }
}
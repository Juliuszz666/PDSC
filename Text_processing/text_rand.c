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
<<<<<<< HEAD
        errno = ENOMEM;
=======
        failureFree((void *)*line, temp);
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
    }
    else
    {
        temp[size] = '\0';
        *line = temp;
    }
}
<<<<<<< HEAD
char *getLine()
{
    errno = 0;
=======
void expandText(char ***text, int size)
{
    char **temp = (char **)realloc(*text, sizeof(char *) * (size + 1));
    if (temp == NULL)
    {
        failureFree((void *)*text, temp);
    }
    else
    {
        *text = temp;
    }
}
char *getLine()
{
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
    char *line = NULL;
    int len = 0;
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        expandLine(&line, len);
<<<<<<< HEAD
        if (errno == ENOMEM)
        {
            free(line);
            line = 0;
            return NULL;
        }

=======
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
        line[len] = (char)c;
        len++;
    }
    if (line != NULL || c != EOF)
    {
        expandLine(&line, len);
<<<<<<< HEAD
        if (errno == ENOMEM)
        {
            free(line);
            line = 0;
            return NULL;
        }

=======
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
        line[len] = '\0';
    }

    return line;
}
<<<<<<< HEAD
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
=======
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
char **getWholeText(int *lines)
{
    char **input = NULL;
    int no_of_lines = 0;
    char *s;
    while ((s = getLine()))
    {
        expandText(&input, no_of_lines);
<<<<<<< HEAD
        if (errno == ENOMEM)
        {
            free(s);
            s = 0;
            freeText(input, no_of_lines, 0);
            return NULL;
        }
=======
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df
        input[no_of_lines] = s;
        no_of_lines++;
    }
    *lines = no_of_lines;
    return input;
}
<<<<<<< HEAD
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
=======
char ***parseWords(char **text, int no_of_lines, int *word_count)
{
    char ***words = malloc(sizeof(char **) * no_of_lines);
    if (words == NULL)
    {
        return words;
    }
    for (size_t i = 0; i < no_of_lines; i++)
    {
        words[i] = (char **)malloc(sizeof(char **));
        if (words[i] == NULL)
        {
            return NULL;
        }
    }

    for (size_t i = 0; i < no_of_lines; i++)
    {
        char *temp = strdup(text[i]);
        for (char *s = strtok(temp, " "); s != NULL; s = strtok(NULL, " "))
        {
            expandText(&words[i], word_count[i]);
            words[i][word_count[i]] = strdup(s);
            word_count[i]++;
        }
        free(temp);
        temp = 0;
    }
    return words;
}
void freeWords(char ***words, int *word_count, int no_of_lines)
{
    for (size_t i = 0; i < no_of_lines; i++)
    {
        for (size_t j = 0; j < word_count[i]; j++)
        {
            free(words[i][j]);
            words[i][j] = 0;
        }
        free(words[i]);
        words[i] = 0;
    }
    free(words);
    words = 0;
}
void freeText(char **text, int no_of_lines)
{
    for (size_t i = 0; i < no_of_lines; i++)
    {
        free(text[i]);
        text[i] = 0;
    }
    free(text);
    text = 0;
}
int main(int argc, char const *argv[])
{
    int no_of_lines;
    char **text = getWholeText(&no_of_lines);
    if (text == NULL)
    {
        goto text_err;
    }
    int *word_count = calloc(no_of_lines, sizeof(int));
    if (word_count == NULL)
    {
        goto word_count_err;
    }
    char ***words = parseWords(text, no_of_lines, word_count);
    if (words == NULL)
    {
        goto words_err;
    }
    printReversedWords(words, word_count, no_of_lines);

words_err:
    freeWords(words, word_count, no_of_lines);
word_count_err:
    free(word_count);
text_err:
    freeText(text, no_of_lines);

    word_count = 0;

    return 0;
}
>>>>>>> 9bacdebec35f261b325a6397e3f12d5f31dd04df

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeText(char **text, int no_of_lines);
void printReversedWords(char **text, int no_of_lines);
void reverseWords(char **text, int no_of_lines);
void expandLine(char **line, int size);
void expandText(char ***text, int size);
void freeString(char *string);
char *concatenateWords(char **words, int word_count, int word_length);
char *getLine();
char **getWholeText(int *lines);
char **tokenize(char *line, int *word_count);
void reverseLine(char **text);

char *strdup(const char *s)
{
    errno = 0;
    char *p = 0;
    p = malloc(strlen(s) + 1);
    if (!p)
        errno = ENOMEM;
    else
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
    freeText(text, no_of_lines);
    return 0;
}
void freeText(char **text, int no_of_lines)
{
    for (size_t i = 0; i < no_of_lines; i++)
    {
        if (text[i] != NULL)
        {
            freeString(text[i]);
        }
    }
    free(text);
}
void freeString(char *string)
{
    free(string);
    string = 0;
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
    char *line = NULL;
    int len = 0;
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        expandLine(&line, len);
        if (errno == ENOMEM)
        {
            freeString(line);
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
            freeString(line);
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
            freeString(s);
            freeText(input, no_of_lines);
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
    char **words = NULL;
    for (char *s = strtok(line, " "); s != NULL; s = strtok(NULL, " "))
    {
        expandText(&words, *word_count);
        if (errno == ENOMEM)
        {
            freeText(words, *word_count);
            return NULL;
        }
        words[*word_count] = strdup(s);
        if (errno == ENOMEM)
        {
            freeText(words, *word_count);
            return NULL;
        }
        (*word_count)++;
    }
    return words;
}
char *concatenateWords(char **words, int word_count, int word_length)
{
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
    }
    return buffer_string;
}
void reverseWords(char **text, int no_of_lines)
{
    for (size_t i = 0; i < no_of_lines; i++)
    {
        reverseLine(&text[i]);
        if (errno == ENOMEM)
        {
            break;
        }
    }
}
void reverseLine(char **text)
{
    int word_count = 0;
    char *temp = strdup(*text);
    if (errno == ENOMEM)
    {
        return;
    }
    char **words = tokenize(temp, &word_count);
    if (errno == ENOMEM)
    {
        freeString(temp);
        return;
    }
    char *buffer_string = concatenateWords(words, word_count, strlen(*text) + 1);
    freeText(words, word_count);
    freeString(temp);
    if (errno == ENOMEM)
    {
        return;
    }
    freeString(*text);
    *text = buffer_string;
}

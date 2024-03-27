#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printReversedWords(char **text, int no_of_lines)
{
    for (int i = no_of_lines - 1; i >= 0; i--)
    {
        printf("%s\n", text[i]);
    }
}
void failureFree(void *main, void *temp)
{
    printf("Failed to allocate memory\n");
    free(temp);
    temp = 0;
    free(main);
    main = 0;
    exit(1);
}
void expandLine(char **line, int size)
{
    char *temp = (char *)realloc(*line, sizeof(char) * (size + 1));
    if (temp == NULL)
    {
        failureFree((void *)*line, temp);
    }
    else
    {
        temp[size] = '\0';
        *line = temp;
    }
}
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
    char *line = NULL;
    int len = 0;
    int c = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        expandLine(&line, len);
        line[len] = (char)c;
        len++;
    }
    if (line != NULL || c != EOF)
    {
        expandLine(&line, len);
        line[len] = '\0';
    }

    return line;
}
char **getWholeText(int *lines)
{
    char **input = NULL;
    int no_of_lines = 0;
    char *s;
    while ((s = getLine()))
    {
        expandText(&input, no_of_lines);
        input[no_of_lines] = s;
        no_of_lines++;
    }
    *lines = no_of_lines;
    return input;
}
void parseWords(char **text, int no_of_lines)
{
    for (size_t i = 0; i < no_of_lines; i++)
    {
        int word_count = 0;
        char **words = NULL;
        char *temp = strdup(text[i]);

        for (char *s = strtok(temp, " "); s != NULL; s = strtok(NULL, " "))
        {
            expandText(&words, word_count);
            words[word_count] = strdup(s);
            word_count++;
        }

        char *buffer_string = malloc(strlen(text[i]) + 1);
        buffer_string[0] = '\0';

        for (int j = word_count - 1; j >= 0; j--)
        {
            strcat(buffer_string, words[j]);
            if (j > 0)
            {
                strcat(buffer_string, " ");
            }
            free(words[j]);
        }

        free(words);
        words = 0;
        free(text[i]);
        text[i] = buffer_string;
    }
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
    parseWords(text, no_of_lines);
    printReversedWords(text, no_of_lines);

text_err:
    freeText(text, no_of_lines);

    return 0;
}
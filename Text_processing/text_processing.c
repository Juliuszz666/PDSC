#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printReversedWords(char ***words, int *word_count, int no_of_lines)
{
    for (int i = no_of_lines - 1; i >= 0; i--)
    {
        for (int j = word_count[i] - 1; j >= 0; j--)
        {
            printf("%s", words[i][j]);
            if (j > 0)
            {
                printf(" ");
            }
        }
        printf("\n");
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
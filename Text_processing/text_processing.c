#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void expandLine(char **line, int size)
{
    char *temp = (char *)realloc(*line, sizeof(char) * (size + 1));
    if (temp == NULL)
    {
        printf("Failed to allocate memory\n");
        free(temp);
        temp = 0;
        free(*line);
        line = 0;
        exit(1);
    }
    else
    {
        temp[size] = '\0';
        *line = temp;
    }
}
void expandInput(char ***text, int size)
{
    char **temp = (char **)realloc(*text, sizeof(char *) * (size + 1));
    if (temp == NULL)
    {
        printf("Failed to allocate memory\n");
        free(temp);
        temp = 0;
        free(*text);
        text = 0;
        exit(1);
    }
    else
    {
        *text = temp;
    }
}
void expandWords(char ***words, int size)
{
    char **temp = (char **)realloc(*words, sizeof(char *) * (size + 1));
    if (temp == NULL)
    {
        printf("Failed to allocate memory\n");
        free(temp);
        temp = 0;
        free(words);
        words = 0;
        exit(1);
    }
    else
    {
        *words = temp;
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
    if (c != EOF || line != NULL)
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
        expandInput(&input, no_of_lines);
        input[no_of_lines] = s;
        no_of_lines++;
    }
    *lines = no_of_lines;
    return input;
}
char ***parseWords(char **text, int no_of_lines, int *word_count)
{
    char ***words = malloc(sizeof(char **) * no_of_lines);
    for (size_t i = 0; i < no_of_lines; i++)
    {
        words[i] = (char **)malloc(sizeof(char **));
        char *temp = strdup(text[i]);
        for (char *s = strtok(temp, " "); s != NULL; s = strtok(NULL, " "))
        {
            expandWords(&words[i], word_count[i]);
            words[i][word_count[i]] = strdup(s);
            word_count[i]++;
        }
        free(temp);
        temp = 0;
    }
    return words;
}
void printReversedWords(char ***words, int *word_count, int no_of_lines)
{
    for (int i = no_of_lines - 1; i >= 0; i--)
    {
        for (int j = word_count[i] - 1; j >= 0; j--)
        {
            printf("%s", words[i][j]);
            switch (j)
            {
            case 0:
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
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
    int *word_count = calloc(no_of_lines, sizeof(int));
    char ***words = parseWords(text, no_of_lines, word_count);
    printReversedWords(words, word_count, no_of_lines);
    freeWords(words, word_count, no_of_lines);
    freeText(text, no_of_lines);
    free(word_count);
    word_count = 0;

    return 0;
}

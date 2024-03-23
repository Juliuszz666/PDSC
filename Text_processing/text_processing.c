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
        free(line);
        line = 0;
        exit(1);
    }
    else
    {
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
        free(text);
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
    if (c != EOF)
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
        words[i] = malloc(sizeof(char *));
        for (char *s = strtok(text[i], " "); s != NULL; s = strtok(NULL, " "))
        {
            expandWords(&words[i], word_count[i]);
            words[i][word_count[i]] = malloc(strlen(s) + 1);
            strcpy(words[i][word_count[i]], s);
            word_count[i]++;
        }
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

int main(int argc, char const *argv[])
{
    int no_of_lines;
    char **text = getWholeText(&no_of_lines);
    int *word_count = calloc(no_of_lines, sizeof(int));
    char ***words = parseWords(text, no_of_lines, word_count);
    printReversedWords(words, word_count, no_of_lines);

    return 0;
}

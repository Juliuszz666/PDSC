/*INCLUDES BEGIN*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*INCLUDES END*/
/*DEFINES BEGIN*/
#define STR_SIZE 25
#define SEC_BUFFER 64
/*DEFINES END*/
/*STRUCTS BEGIN*/

/*Struct for storing person's name and surname*/
typedef struct
{
    char name[STR_SIZE];
    char surname[STR_SIZE];
} person;
/*STRUCTS END*/
/*FUNCTION DECLARATIONS BEGIN*/

/*Swapping values of struct with given indexes*/
void swap(person *array, int index_1, int index_2);

/*Component of quicksorting*/
int dividePivot(person *array, int left, int right);

/*Sorting names. Firstly it sort by surname (second string) then by name (if surnames are the same)*/
void quickSort(person *array, int left, int right);

/*
Function responsible for getting input from a .txt file.
Function handles:
-too long input (it only returns lines that would not overflow person struct)
-- in addition function prints lines where invalid input occured
-invalid file and its extention
*/
person *getInput(const char *filename, int *lines);

/*Saving names*/
void saveNames(person *array, const char *filename, int lines);

/*
Function return expanded array by 1 if it is possible, otherwise it terminates program
parameter file is to close file in case of unsuccessful reallocing
*/
int expandInput(person *array, int size);
/*FUNCTION DECLARATIONS END*/
/*MAIN FUNCTION BEGIN*/
int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Invalid number of arguments\n");
        exit(1);
    }
    int lines = 0;
    person *input = getInput(argv[1], &lines);
    quickSort(input, 0, lines - 1);
    saveNames(input, argv[2], lines);
    free(input);

    return 0;
}
/*MAIN FUNCTION END*/

void saveNames(person *array, const char *filename, int lines)
{
    if (strstr(filename, ".txt") == NULL)
    {
        perror("Invalid filename");
        exit(1);
    }
    FILE *output_file;
    output_file = fopen(filename, "w");
    for (int i = 0; i < lines; i++)
    {
        fprintf(output_file, "%s %s\n", array[i].name, array[i].surname);
    }

    fclose(output_file);
}

person *getInput(const char *filename, int *lines)
{
    if (strstr(filename, ".txt") == NULL)
    {
        printf("Invalid filename\n");
        exit(1);
    }

    FILE *input_file;
    input_file = fopen(filename, "r");

    if (input_file == NULL)
    {
        perror("Invalid file");
        exit(1);
    }

    person *input = NULL;

    int line = 0;
    int file_line = 0;
    char first_buffer[SEC_BUFFER];

    while (fgets(first_buffer, SEC_BUFFER, input_file))
    {
        printf("%s\n", first_buffer);
        /*Initial verification of input*/
        file_line++;
        if (strlen(first_buffer) == 1)
        {
            printf("Invalid line no: %d\t - empty line\n", file_line);
            continue;
        }
        else if (strstr(first_buffer, " ") == NULL)
        {
            printf("Invalid line no: %d\t - no space\n", file_line);
            continue;
        }

        char buffer[2][STR_SIZE];
        /*No defense against line with wrongly placed space*/
        sscanf(first_buffer, "%s %s", buffer[0], buffer[1]);
        bool noOverflow = ((strlen(buffer[0]) < STR_SIZE - 1) && (strlen(buffer[1]) < STR_SIZE - 1));
        if (!noOverflow)
        {
            printf("Invalid line no: %d\t - too long name of surname\n", file_line);
            continue;
        }
        else
        {
            if (expandInput(input, line))
                break;
            strcpy(input[line].name, buffer[0]);
            strcpy(input[line].surname, buffer[1]);
            line++;
        }
    }
    fclose(input_file);

    *lines = line;

    return input;
}

void quickSort(person *array, int left, int right)
{
    if (left < right)
    {
        int pivot = dividePivot(array, left, right);
        quickSort(array, left, pivot - 1);
        quickSort(array, pivot + 1, right);
    }
}

int dividePivot(person *array, int left, int right)
{
    int diveder_index = left + ((right - left) / 2);
    person diveder = array[diveder_index];

    /*Moving diveder index to the right*/
    swap(array, diveder_index, right);

    int current_pos = left;

    for (int i = left; i < right; i++)
    {
        bool surnameToSwap = strcmp(array[i].surname, diveder.surname) < 0;
        bool nameToSawp = strcmp(array[i].surname, diveder.surname) == 0 && strcmp(array[i].name, diveder.name) < 0;
        if (surnameToSwap || nameToSawp)
        {
            swap(array, current_pos, i);
            current_pos++;
        }
    }

    /*Returning diveder index to its destined posistion*/
    swap(array, current_pos, right);

    return current_pos;
}

void swap(person *array, int index_1, int index_2)
{
    char buffer[STR_SIZE];
    memmove(buffer, array[index_1].name, STR_SIZE);
    memmove(array[index_1].name, array[index_2].name, STR_SIZE);
    memmove(array[index_2].name, buffer, STR_SIZE);

    memmove(buffer, array[index_1].surname, STR_SIZE);
    memmove(array[index_1].surname, array[index_2].surname, STR_SIZE);
    memmove(array[index_2].surname, buffer, STR_SIZE);
}

int expandInput(person *array, int size)
{
    person *temp_val = (person *)realloc(array, sizeof(person) * (size + 1));
    if (temp_val == NULL)
    {
        printf("No memory available, possibility of memory leak\nFurther reading lines is stopped\n");
        return 1;
    }
    else
    {
        array = temp_val;
        return 0;
    }
}

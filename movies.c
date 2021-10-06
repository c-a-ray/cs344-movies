#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LANGUAGES 5
#define MAX_LANG_LEN 20

struct Movie
{
    char *title;
    int year;
    char languages[MAX_LANGUAGES][MAX_LANG_LEN];
    float rating;
    struct Movie *next;
};

char *getFilepath(int, char **);
struct Movie *processFile(char *, int *);
struct Movie *createMovie(char *);
void parseLanguages(struct Movie *, char *);
int getQueryOption(void);
void printOptions(void);
void runQueryOption(struct Movie *, int);
void printMoviesWithYear(struct Movie *, int);

int main(int argc, char *argv[])
{
    char *filepath = getFilepath(argc, argv);
    if (strlen(filepath) == 0)
    {
        return EXIT_FAILURE;
    }

    int totalMovies;
    struct Movie *list = processFile(filepath, &totalMovies);

    printf("Processed file %s and parsed data for %d movies\n\n", filepath, totalMovies);

    int queryOption;
    do
    {
        queryOption = getQueryOption();
        runQueryOption(list, queryOption);
    } while (queryOption != 4);

    printf("Exiting\n");

    return EXIT_SUCCESS;
}

char *getFilepath(int argc, char **argv)
{
    char *filepath = "";
    if (argc == 2)
    {
        filepath = argv[1];
    }
    else
    {
        printf("Please provide path to movies CSV\n");
        printf("e.g. ./data/movies.csv\n");
    }
    return filepath;
}

struct Movie *processFile(char *filepath, int *totalMovies)
{
    FILE *moviesCSV = fopen(filepath, "r");

    char *line = NULL;
    struct Movie *head = NULL;
    struct Movie *tail = NULL;
    size_t len = 0;
    ssize_t nread;
    int row = 0;

    while ((nread = getline(&line, &len, moviesCSV)) != -1)
    {
        if (row > 0)
        {
            struct Movie *newNode = createMovie(line);

            if (head == NULL)
            {
                head = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
        }
        row++;
    }
    free(line);
    fclose(moviesCSV);
    *totalMovies = row - 1;
    return head;
}

struct Movie *createMovie(char *line)
{
    struct Movie *movie = malloc(sizeof(struct Movie));
    char *saveptr;

    char *token = strtok_r(line, ",", &saveptr);
    int col = 0;
    while (token != NULL)
    {
        if (col == 0)
        {
            movie->title = calloc(strlen(token) + 1, sizeof(char));
            strcpy(movie->title, token);
        }
        else if (col == 1)
        {
            movie->year = atoi(token);
        }
        else if (col == 2)
        {
            parseLanguages(movie, token);
        }
        else if (col == 3)
        {
            movie->rating = atof(token);
        }
        token = strtok_r(NULL, ",", &saveptr);
        col++;
    }
    movie->next = NULL;
    return movie;
}

void parseLanguages(struct Movie *movie, char *token)
{
    int languagesIdx = 0;
    int langIdx = 0;
    for (int tokenIdx = 1; tokenIdx < strlen(token) - 1; tokenIdx++)
    {
        if (token[tokenIdx] == ';')
        {
            langIdx = 0;
            languagesIdx++;
        }
        else
        {
            movie->languages[languagesIdx][langIdx] = token[tokenIdx];
            langIdx++;
        }
    }
}

int getQueryOption(void)
{
    int option;
    printOptions();
    scanf("%d", &option);
    while (option < 1 || option > 4)
    {
        printf("You entered an incorrect choice. Try again.\n\n");
        printOptions();
        scanf("%d", &option);
    }
    return option;
}

void printOptions(void)
{
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rate movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
}

void runQueryOption(struct Movie *list, int queryOption)
{
    if (queryOption == 1)
    {
        printf("Enter the year for which you want to see movies: ");
        int year;
        scanf("%d", &year);
        printMoviesWithYear(list, year);
    }
}

void printMoviesWithYear(struct Movie *list, int year)
{
    struct Movie *head = list;
    int nMoviesWithYear = 0;
    while (list != NULL)
    {
        if (list->year == year)
        {
            printf("%s\n", list->title);
            nMoviesWithYear++;
        }
        list = list->next;
    }
    if (nMoviesWithYear == 0)
    {
        printf("No data about movies released in year %d", year);
    }
    printf("\n\n");
    list = head;
}
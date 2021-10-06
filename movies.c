#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LANGUAGES 5
#define MAX_LANG_LEN 20
#define MIN_YEAR 1990
#define MAX_YEAR 2021

struct movie
{
    char *title;
    int year;
    char languages[MAX_LANGUAGES][MAX_LANG_LEN];
    float rating;
    struct movie *next;
};

char *getFilepath(int, char **);
struct movie *processFile(char *, int *, int *, int *);
struct movie *createMovie(char *);
void parseLanguages(struct movie *, char *);
int getQueryOption(void);
void printOptions(void);
void runQueryOption(struct movie *, int, int, int);
void printMoviesWithYear(struct movie *, int);
void printHighestRatedMovies(struct movie *, int, int);
void printMoviesWithLanguage(struct movie *, char[]);

int main(int argc, char *argv[])
{
    char *filepath = getFilepath(argc, argv);
    if (strlen(filepath) == 0)
    {
        return EXIT_FAILURE;
    }

    int totalMovies;
    int minYear = MAX_YEAR;
    int maxYear = MIN_YEAR;
    struct movie *list = processFile(filepath, &totalMovies, &minYear, &maxYear);

    printf("Processed file %s and parsed data for %d movies\n\n", filepath, totalMovies);

    int queryOption;
    do
    {
        queryOption = getQueryOption();
        runQueryOption(list, queryOption, minYear, maxYear);
    } while (queryOption != 4);

    printf("Exiting movies program\n");

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

struct movie *processFile(char *filepath, int *totalMovies, int *minYear, int *maxYear)
{
    FILE *moviesCSV = fopen(filepath, "r");

    char *line = NULL;
    struct movie *head = NULL;
    struct movie *tail = NULL;
    size_t len = 0;
    ssize_t nread;
    int row = 0;

    while ((nread = getline(&line, &len, moviesCSV)) != -1)
    {
        if (row > 0)
        {
            struct movie *newMovie = createMovie(line);

            if (head == NULL)
            {
                head = newMovie;
                tail = newMovie;
            }
            else
            {
                tail->next = newMovie;
                tail = newMovie;
            }

            if (newMovie->year < *minYear)
            {
                *minYear = newMovie->year;
            }
            if (newMovie->year > *maxYear)
            {
                *maxYear = newMovie->year;
            }
        }
        row++;
    }
    free(line);
    fclose(moviesCSV);
    *totalMovies = row - 1;
    return head;
}

struct movie *createMovie(char *line)
{
    struct movie *newMovie = malloc(sizeof(struct movie));
    char *saveptr;

    char *token = strtok_r(line, ",", &saveptr);
    int col = 0;
    while (token != NULL)
    {
        if (col == 0)
        {
            newMovie->title = calloc(strlen(token) + 1, sizeof(char));
            strcpy(newMovie->title, token);
        }
        else if (col == 1)
        {
            newMovie->year = atoi(token);
        }
        else if (col == 2)
        {
            parseLanguages(newMovie, token);
        }
        else if (col == 3)
        {
            newMovie->rating = atof(token);
        }
        token = strtok_r(NULL, ",", &saveptr);
        col++;
    }
    newMovie->next = NULL;
    return newMovie;
}

void parseLanguages(struct movie *newMovie, char *token)
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
            newMovie->languages[languagesIdx][langIdx] = token[tokenIdx];
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

void runQueryOption(struct movie *list, int queryOption, int minYear, int maxYear)
{
    if (queryOption == 1)
    {
        printf("Enter the year for which you want to see movies: ");
        int year;
        scanf("%d", &year);
        printMoviesWithYear(list, year);
    }
    else if (queryOption == 2)
    {
        printHighestRatedMovies(list, minYear, maxYear);
    }
    else if (queryOption == 3)
    {
        printf("Enter the language for which you want to see movies: ");
        char language[MAX_LANG_LEN];
        scanf("%s", language);
        printMoviesWithLanguage(list, language);
    }
}

void printMoviesWithYear(struct movie *list, int year)
{
    struct movie *head = list;
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
        printf("No data about movies released in year %d\n", year);
    }
    printf("\n");
    list = head;
}

void printHighestRatedMovies(struct movie *list, int minYear, int maxYear)
{
    struct movie *head = list;

    for (int year = minYear; year <= maxYear; year++)
    {
        struct movie highestRated;
        highestRated.rating = 0;

        while (list != NULL)
        {
            if (list->year == year && list->rating > highestRated.rating)
            {
                highestRated = *list;
            }
            list = list->next;
        }

        if (highestRated.rating > 0)
        {
            printf("%d %.1f %s\n", highestRated.year, highestRated.rating, highestRated.title);
        }
        list = head;
    }
    printf("\n");
    return;
}

void printMoviesWithLanguage(struct movie *list, char language[])
{
    struct movie *head = list;
    int nMoviesWithLanguage = 0;

    while (list != NULL)
    {
        for (int i = 0; i < MAX_LANGUAGES; i++)
        {
            if (strcmp(list->languages[i], language) == 0)
            {
                printf("%d %s\n", list->year, list->title);
                nMoviesWithLanguage++;
                break;
            }
        }
        list = list->next;
    }

    if (nMoviesWithLanguage == 0)
    {
        printf("No data about movies released in %s\n", language);
    }
    printf("\n");

    list = head;
    return;
}

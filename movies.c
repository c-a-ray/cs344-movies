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

struct Movie *processFile(char *, int *);
struct Movie *createMovie(char *);
void parseLanguages(struct Movie *, char *);
void printMovie(struct Movie *);
void printMovieList(struct Movie *, int);

int main()
{
    char *filepath = "data/movies_sample_1.csv";
    int totalMovies;
    struct Movie *list = processFile(filepath, &totalMovies);
    printMovieList(list, totalMovies);
    return EXIT_SUCCESS;
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

void printMovie(struct Movie *movie)
{
    printf("Title: %s\n", movie->title);
    printf("Year: %d\n", movie->year);
    printf("Languages: ");
    for (int i = 0; i < MAX_LANGUAGES; i++)
    {
        if (strlen(movie->languages[i]) > 0)
        {
            printf("%s\t", movie->languages[i]);
        }
    }
    printf("\nRating: %g\n\n", movie->rating);
}

void printMovieList(struct Movie *list, int totalMovies)
{
    printf("Processed %d movies...\n\n", totalMovies);
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}
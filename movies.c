#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Movie
{
    char title[50];
    int year;
    char languages[10][50];
    float rating;
    struct Movie *next;
};

struct Movie *processFile(char *, int *);
struct Movie *createMovie(char *);
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
    *totalMovies = row;
    return head;
}

struct Movie *createMovie(char *line)
{
    struct Movie *movie = malloc(sizeof(struct Movie));
    char *token = strtok(line, ",");
    int col = 0;
    while (token != NULL)
    {
        if (col == 0)
        {
            strcpy(movie->title, token);
        }
        else if (col == 1)
        {
            movie->year = atoi(token);
        }
        else if (col == 2)
        {
            int languagesIdx = 0;
            int langIdx = 0;
            for (int i = 1; i < strlen(token); i++)
            {
                if (token[i] == ';')
                {
                    langIdx = 0;
                    languagesIdx++;
                }
                else if (token[i] != ']')
                {
                    movie->languages[languagesIdx][langIdx] = token[i];
                    langIdx++;
                }
            }
        }
        else if (col == 3)
        {
            movie->rating = atof(token);
        }
        token = strtok(NULL, ",");
        col++;
    }
    movie->next = NULL;
    return movie;
}

void printMovie(struct Movie *movie)
{
    printf("Title: %s\n", movie->title);
    printf("Year: %d\n", movie->year);
    printf("Languages: ");
    for (int i = 0; i < 10; i++)
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
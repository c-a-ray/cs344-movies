#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Movie {
    char title[50];
    int year;
    char languages[10][50];
    float rating;
};

struct Node {
    struct Movie *movie;
    struct Node *next;
};

struct Node *processFile(char*);
struct Movie *createMovie(char*, int);
void printMovie(struct Movie*);
void printMovieList(struct Node*);

int main() {
    char *filepath = "data/movies_sample_1.csv";
    struct Node *list = processFile(filepath);
    printMovieList(list);
    return EXIT_SUCCESS;
}

struct Node *processFile(char *filepath) {
    FILE *moviesCSV = fopen(filepath, "r");

    char *line = NULL;
    struct Node *head = NULL;
    struct Node *tail = NULL;
    int row = 0;

    while (fgets(line, sizeof(line), moviesCSV)) {
        if (row == 0) {
            row++;
            continue;
        }

        struct Node *newNode;
        newNode->movie = createMovie(line, row);
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        row++;
    }
    free(line);
    fclose(moviesCSV);
    return head;
}

struct Movie *createMovie(char *line, int row) {
    struct Movie *movie = malloc(sizeof(struct Movie));
    char *token = strtok(line, ",");
    int col = 0;
    while (token != NULL) {
        if (col == 0) {
            strcpy(movie->title, token);
        } else if (col == 1) {
            movie->year = atoi(token);
        } else if (col == 2) {
            int languagesIdx = 0;
            int langIdx = 0;
            for (int i = 1; i < strlen(token); i++) {
                if (token[i] == ';') {
                    langIdx = 0;
                    languagesIdx++;
                } else if (token[i] != ']') {
                    movie->languages[languagesIdx][langIdx] = token[i];
                    langIdx++;
                }
            }
        } else if (col == 3) {
            movie->rating = atof(token);
        }
        token = strtok(NULL, ",");
        col++;
    }
    return movie;
}

void printMovie(struct Movie *movie)  {
    printf("Title: %s\n", movie->title);
    printf("Year: %d\n", movie->year);
    printf("Languages: ");
    for (int i = 0; i < 10; i++) {
        if (strlen(movie->languages[i]) > 0) {
            printf("%s\t", movie->languages[i]);
        }
    }
    printf("\nRating: %2f\n\n", movie->rating);
}

void printMovieList(struct Node *list) {
    while (list != NULL) {
        printMovie(list->movie);
        list = list->next;
    }
}
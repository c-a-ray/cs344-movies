#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 50;
#define MAX_ROW_LEN 300;
#define MAX_COLS 5;

struct Movie {
    char title[50];
    int year;
    char languages[10][50];
    float rating;
};

int main() {
    char const* csvPath = "data/movies_sample_1.csv";
    FILE* movies_csv = fopen(csvPath, "r");
    if (movies_csv == NULL) {
        perror("Unable to open CSV file");
        return 1;
    }

    struct Movie movies[50]; 
    char line[1000];
    unsigned short row = 0;

    while (fgets(line, sizeof(line), movies_csv)) {
        if (row == 0) { // Skip header
            row++;
            continue;
        }

        struct Movie movie;
        const char *delim = ",";
        char *token;
        unsigned short col = 0;

        token = strtok(line, delim);
        while (token != NULL) {
            if (col == 0) {
                strcpy(movie.title, token);
            } else if (col == 1) {
                movie.year = atoi(token);
            } else if (col == 2) {
                char languages[500];
                for (int i = 1; i < strlen(token) - 1; i++) { // Remove brackets
                    languages[i] = token[i];
                }

                const char *langDelim = ";";
                char *language = strtok(languages, langDelim);
                unsigned short langIndex = 0;
                while (language != NULL) {
                    strcpy(movie.languages[langIndex], language);
                    langIndex++;
                }
            } else if (col == 3) {
                movie.rating = atof(token);
            }

            token = strtok(NULL, delim);
            col++;
        }
        movies[row] = movie;
        row++;
    }

    printf("Movie Name: %s", movies[0].title);
    // printf("Year: %d", movies[0].year);
    // printf("Languages: ");
    // for (int i = 0; i < 10; i++) {
    //     if (strlen(movies[0].languages[i]) > 0) {
    //         printf("%s\n", movies[0].languages[i]);
    //     }
    // }
    // printf("Rating: %f\n\n", movies[0].rating);
    return 0;
}
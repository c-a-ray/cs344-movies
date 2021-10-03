#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 50;
#define MAX_ROW_LEN 300;
#define MAX_COLS 5;

struct Movie {
    char title[50];
    int year;
    char* languages[10];
    float rating;
};

short lastOccurrence(char[], char);
char* getLanguagesStr(char[]);

int main() {
    char const* csvPath = "data/movies_sample_1.csv";
    FILE* movies_csv = fopen(csvPath, "r");
    if (movies_csv == NULL) {
        perror("Unable to open CSV file");
        return 1;
    }

    struct Movie movies[50]; 
    char line[300];
    unsigned short row = 0;

    while (fgets(line, sizeof(line), movies_csv)) {
        if (row == 0) { // Skip header
            row++;
            continue;
        }

        struct Movie movie;
        const char delim[2] = ",";
        char* token;
        unsigned short col = 0;

        token = strtok(line, delim);
        while (token != NULL) {
            switch (col) {
                case 0:
                    strcpy(movie.title, token);
                    break;
                case 1:
                    movie.year = atoi(token);
                    break;
                case 2:
                    const char languageDelim[2] = ";";
                    char* languagesStr = getLanguagesStr(token);
                    char* language =  strtok(languagesStr, languageDelim);
                    unsigned short languageIndex = 0;
                    while (language != NULL) {
                        movies->languages[languageIndex] = language;
                        languageIndex++;
                    }
                    break;
                case 3:
                    movie.rating = atof(token);
                    break;
            }

            token = strtok(NULL, delim);
        }
        row++;
    }

    printf("Movie: %s \n Year: %d \n Rating %f", movies[0].title, movies[0].year, movies[0].rating);

    return 0;
}

short lastOccurrence(char str[], char ch) {
    short index = -1;
    for (int i = 0; i < strlen(&str); i++) {
        if (&str[i] == ch) {
            index = i;
        }
    }
    return index;
}

char* getLanguagesStr(char* token) {
    char languagesStr[200];
    for (int i = 0; i < strlen(token) - 1; i++) {
        if (i != 0) {
            languagesStr[i] = token[i];
        }
    }
    return languagesStr;
}
/**
 * @file movies.c
 * @author Cody Ray <rayc2@oregonstate.edu>
 * @version 1.0
 * @section DESCRIPTION
 *
 * For OSU CS 344
 * Assignment 1
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LANGUAGES 5 // Max number of languages a movie can have
#define MAX_LANG_LEN 20 // Max character length of a single language
#define MIN_YEAR 1900   // Min year a movie can have
#define MAX_YEAR 2021   // Max year a movie can have

/** 
 * @brief structure for storing movie values
 */
struct movie
{
    char *title;
    int year;
    char languages[MAX_LANGUAGES][MAX_LANG_LEN];
    float rating;
    struct movie *next;
};

/**
 * Gets path to movies CSV from console arguments
 *
 * If the number of console arguments is correct, returns the 
 * path to movies data CSV from console argument
 *
 * @param  argc is the number of console arguments provided
 * @param  argv are the console arguments
 * @return provided filepath is returned if valid number of args,
 *         else returns a char * with length 0
 */
char *getFilepath(int, char **);

/**
 * Reads movies from CSV and stores them in a linked list
 *
 * Opens the CSV file and processes the file line-by-line,
 * creating a linked list of movie structs. Also determines
 * the minimum and maximum years of all movies in the CSV,
 * for use in printHighestRatedMovies.
 * 
 *
 * @param  filepath is the path to the movies CSV
 * @param  totalMovies is a variable used to store the number of movies seen
 * @param  minYear is the minimum year of all movies in the CSV
 * @param  maxYear is the maximum year of all movies in the CSV
 * @return a pointer to the first movie in a linked list of movies
 */
struct movie *processFile(char *, int *, int *, int *);

/**
 * Creates a new movie from a single CSV row
 *
 * Parses a token (a single CSV row) and stores the title, 
 * year, languages and rating in a new movie struct
 *
 * @param  line is a token representing a single CSV row
 * @return a pointer to a new movie struct
 */
struct movie *createMovie(char *);

/**
 * Parses and stores languages token
 *
 * Parses a character array of the format [Language1;Language2;Language3],
 * storing each in the new movie struct's languages field
 *
 * @param  newMovie is the movie to store the languages in
 * @param  token is the unparsed character array of languages
 */
void parseLanguages(struct movie *, char *);

/**
 * Gets a valid top-level option input from user
 *
 * Prompts user with options and asks for an integer between 1 and 4.
 * Prints a warning message and tries again if input is invalid.
 *
 * @return a valid option (integer between 1 and 4)
 */
int getQueryOption(void);

/**
 * Prints top-level user options
 *
 * Prints 4 options to the console:
 *  1. Show movies released in the specified year
 *  2. Show highest rated movie for each year
 *  3. Show the title and year of release of all movies in a specific language
 *  4. Exit from the program
 * and asks user to choose one
 */
void printOptions(void);

/**
 * Driver function for analysis options
 *
 * Runs the movie analysis option chosen by user. Takes in the min
 * and max years of all movies for use in printHighestRatedMovies
 * 
 * @param list is the linked list of movies
 * @param queryOption is the analysis option chosen by the user
 * @param minYear is the min year of all movies 
 * @param maxYear is the max year of all movies
 */
void runQueryOption(struct movie *, int, int, int);

/**
 * Shows movies released in the specified year
 *
 * Iterates through the linked list of movies, printing
 * each movie title with the specified year
 * 
 * @param list is the linked list of movies
 * @param year is the user-chosen value for which movies to print
 */
void printMoviesWithYear(struct movie *, int);

/**
 * Shows the highest rated movie for each year
 *
 * Iterates through all years between minYear and maxYear,
 * printing the year, rating and title of the highest rated
 * movie for each year
 * 
 * @param list is the linked list of movies
 * @param minYear is the minimum year of all movies
 * @param maxYear is the maximum year of all movies
 */
void printHighestRatedMovies(struct movie *, int, int);

/**
 * Shows the title and year of release of all movies in a specific language
 *
 * Iterates through all movies, printing the title 
 * and year if the movie has the specified language
 * 
 * @param list is the linked list of movies
 * @param language is the language specified by the user
 */
void printMoviesWithLanguage(struct movie *, char[]);

int main(int argc, char *argv[])
{
    char *filepath = getFilepath(argc, argv);
    if (strlen(filepath) == 0) // Invalid number of args given
    {
        return EXIT_FAILURE;
    }

    // Values to be determined in initial CSV processing for later use
    int totalMovies;
    int minYear = MAX_YEAR;
    int maxYear = MIN_YEAR;

    struct movie *list = processFile(filepath, &totalMovies, &minYear, &maxYear);

    printf("Processed file %s and parsed data for %d movies\n\n", filepath, totalMovies);

    int queryOption;
    do
    {
        queryOption = getQueryOption();                      // Get valid option (1-4)
        runQueryOption(list, queryOption, minYear, maxYear); // Run the specified option
    } while (queryOption != 4);

    printf("Exiting movies program\n");

    return EXIT_SUCCESS;
}

char *getFilepath(int argc, char **argv)
{
    char *filepath = ""; // If filepath still has length 0 when returned, program will exit in parent function
    if (argc == 2)       // Ensure correct number of console arguments was provided
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
        if (row > 0) // Skip the header row
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

            // Calculating min and max movie years here for efficiency
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
    *totalMovies = row - 1; // Don't count header row in total movies count
    return head;
}

struct movie *createMovie(char *line)
{
    struct movie *newMovie = malloc(sizeof(struct movie));
    char *saveptr;

    char *token = strtok_r(line, ",", &saveptr); // Get the first token
    int col = 0;
    while (token != NULL)
    {
        if (col == 0) // Zeroth column is movie title
        {
            newMovie->title = calloc(strlen(token) + 1, sizeof(char));
            strcpy(newMovie->title, token);
        }
        else if (col == 1) // First column is the movie year
        {
            newMovie->year = atoi(token);
        }
        else if (col == 2) // Second column is the movie languages array
        {
            parseLanguages(newMovie, token);
        }
        else if (col == 3) // Third language is the movie rating
        {
            newMovie->rating = atof(token);
        }

        // Move to the next column
        token = strtok_r(NULL, ",", &saveptr);
        col++;
    }
    newMovie->next = NULL;
    return newMovie;
}

void parseLanguages(struct movie *newMovie, char *token)
{
    int languagesIdx = 0;                                            // The index of a movie language
    int langIdx = 0;                                                 // The index of a character within a single language
    for (int tokenIdx = 1; tokenIdx < strlen(token) - 1; tokenIdx++) // Iterate through all chars inside of brackets
    {
        if (token[tokenIdx] == ';') // Token is semi-colon delimited; skip and begin a new language
        {
            langIdx = 0;    // First character of the next language
            languagesIdx++; // Next movie language
        }
        else
        {
            newMovie->languages[languagesIdx][langIdx] = token[tokenIdx]; // Store the character
            langIdx++;                                                    // Movie to next character in the current language
        }
    }
}

int getQueryOption(void)
{
    int option;
    printOptions();
    scanf("%d", &option);
    while (option < 1 || option > 4) // Keep prompting user for valid input whil input is invalid
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
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n\n");
    printf("Enter a choice from 1 to 4: ");
}

void runQueryOption(struct movie *list, int queryOption, int minYear, int maxYear)
{
    if (queryOption == 1) // Print all movies with specified year
    {
        printf("Enter the year for which you want to see movies: ");
        int year;
        scanf("%d", &year);
        printMoviesWithYear(list, year);
    }
    else if (queryOption == 2) // Print highest rated movie per year
    {
        printHighestRatedMovies(list, minYear, maxYear);
    }
    else if (queryOption == 3) // Print all movies with specified language
    {
        printf("Enter the language for which you want to see movies: ");
        char language[MAX_LANG_LEN];
        scanf("%s", language);
        printMoviesWithLanguage(list, language);
    }
}

void printMoviesWithYear(struct movie *list, int year)
{
    struct movie *head = list; // Keep track of the head so the list can be reused

    int nMoviesWithYear = 0; // To keep track of whether there are any movies for the given year
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
    list = head; // Restore list to head for reuse
}

void printHighestRatedMovies(struct movie *list, int minYear, int maxYear)
{
    struct movie *head = list; // Keep track of the head so the list can be reused

    for (int year = minYear; year <= maxYear; year++) // Iterate through all years between min and max movie years
    {
        struct movie highestRated; // Keep track of highest rated movie for given year
        highestRated.rating = 0;   // Min rating is 1; rating 0 indicates no movie has been seen yet for given year

        while (list != NULL)
        {
            if (list->year == year && list->rating > highestRated.rating)
            {
                highestRated = *list;
            }
            list = list->next;
        }

        if (highestRated.rating > 0) // Don't print info for years without any movies
        {
            printf("%d %.1f %s\n", highestRated.year, highestRated.rating, highestRated.title);
        }
        list = head; // Restore list to head for reuse
    }
    printf("\n");
    return;
}

void printMoviesWithLanguage(struct movie *list, char language[])
{
    struct movie *head = list;   // Keep track of the head so the list can be reused
    int nMoviesWithLanguage = 0; // Keep track of whether any movies have specified language

    while (list != NULL)
    {
        for (int i = 0; i < MAX_LANGUAGES; i++)
        {
            if (strcmp(list->languages[i], language) == 0) // Movie has specified language
            {
                printf("%d %s\n", list->year, list->title);
                nMoviesWithLanguage++;
                break; // Only print the movie info once
            }
        }
        list = list->next;
    }

    if (nMoviesWithLanguage == 0) // Print special message if no movies have specified language
    {
        printf("No data about movies released in %s\n", language);
    }
    printf("\n");

    list = head; // Restore list to head for reuse
    return;
}

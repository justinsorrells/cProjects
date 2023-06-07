#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/*
    Create variable for arrays and character arrays
*/
#define COMPUTE_AVERAGE(x,y) (double) x / y 
#define CRUISE_DESCRIPTION 37 
#define CRUISE_DETAILS 200
#define CRUISE_SUMMARIES 20
#define EMPTY_SUMMARY ((cruiseSummary_t) { 0 })
#define LINE_BUFFER 250
#define MAX_DAYS 21
#define STANDARD_STRING 81
#define WIDTH_DETAIL 35
#define WIDTH_SUMMARY 60

typedef struct 
{
    int cruiseNumber;
    char cruiseDescription[CRUISE_DESCRIPTION];
    int numberOfDays;
    char destinations[MAX_DAYS][STANDARD_STRING];
} cruiseSummary_t;

typedef struct
{
    int cruiseNumber;
    int departureYear;
    int departureMonth;
    double price;
} cruiseDetail_t;



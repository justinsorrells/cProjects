/*
    Author: Justin W. Sorrells
    Course: COP2220C
    Date: April 20, 2023
*/

#include "project6.h"

/*
    I created a global const for OPTIONS. I felt that this approach
    was better than an arbitrary array or conditional nested within
    a function.
*/
const char OPTIONS[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0' }; 

void displayCredits(void);
void displayData(cruiseSummary_t *cruise);
void dataHeader(int id);
void displayMenu(void);
void displayWelcomeMessage(void);
int getCruiseLength(char *message);
double getUserDouble(char *message);
char* getUserStringInput(char *message, int lengthOfInput);
bool isPositive(double x, char *errorMessage);
bool isStringTooLong(char *string, int length, char *errorMessage);
bool isStringTooShort(char *string, int length, char *errorMessage);
bool isValidOption(char option, const char VALID_OPTIONS[], char *errorMessage);
/*
    In the two function below, I declared lineLegth so I could pass the
    defined LINE_BUFFER into the function.
*/
int loadDetailData(cruiseDetail_t cruiseDetailData[], int maxNumCruiseDetails, int lineLength);
int loadSummaryData(cruiseSummary_t cruiseSummaryData[], int maxNumCruiseSummaries, int lineLength);
void maintain(cruiseSummary_t cruiseSummaryData[], cruiseDetail_t cruiseDetail_t[], int *cruiseSummaries, int *cruiseDetails);
void pause(char *message);
void printCruiseDetail(cruiseDetail_t *cruise);
void printCruiseSummaries(cruiseSummary_t cruiseSummaryData[], int numCruises);
void printDash(int numDashes);
void printDetailHeader(int id);
void searchDetailDataByMaxPrice(cruiseDetail_t *cruiseDetailData, int numDetails, cruiseSummary_t *cruiseSummaryData, int numSummaries);
void searchSummaryDataByDestination(cruiseSummary_t cruiseSummaryData[], int numCruises, int destinationStringLength);
void searchSummaryDataByLength(cruiseSummary_t cruiseSummaryData[], int numCruises);
void searchSummaryDataByMaxLength(cruiseSummary_t cruiseSummaryData[], int numCruises);
char selectOption(const char VALID_OPTIONS[], void (*fnPtr) (void)); // I passed in an OPTIONS array to make the OPTIONS easier to update.
int sortByCruiseNumber(const void *a, const void *b);
int sortByDescription(const void *a, const void *b);
char * stringToLower(char *string);
char * stripNewline(char *string);

int main (void)
{
    /*
        Declare arrays of structs for cruiseSummaryData
        and cruiseDetailData
    */
    cruiseSummary_t cruiseSummaryData[CRUISE_SUMMARIES] = { 0 };
    cruiseDetail_t cruiseDetailData[CRUISE_DETAILS] = { 0 };
    
    char repeat = 'y'; // Declare loop variable
    char option = '\0'; // Declare user input variable
    int cruiseSummaries = 0; 
    int cruiseDetails = 0;

    cruiseSummaries = loadSummaryData(cruiseSummaryData, CRUISE_SUMMARIES, LINE_BUFFER); // Store the number of summaries loaded
    cruiseDetails = loadDetailData(cruiseDetailData, CRUISE_DETAILS, LINE_BUFFER); // Store the number of details loaded

    displayWelcomeMessage();

    while ( repeat == 'y' )
    {
        option = selectOption(OPTIONS, displayMenu); // Get user input option
        switch (option)
        {
            case '1':
                printCruiseSummaries(cruiseSummaryData, cruiseSummaries);
                break;
            case '2':
                searchSummaryDataByLength(cruiseSummaryData, cruiseSummaries);
                break;
            case '3':
                searchSummaryDataByDestination(cruiseSummaryData, cruiseSummaries, STANDARD_STRING);
                break;
            case '4':
                searchSummaryDataByMaxLength(cruiseSummaryData, cruiseSummaries);
                break;
            case '5':
                searchDetailDataByMaxPrice(cruiseDetailData, cruiseDetails, cruiseSummaryData, cruiseSummaries);
                break;
            case '6':
                qsort((void *) cruiseSummaryData, cruiseSummaries, sizeof(cruiseSummaryData[0]), sortByDescription);
                break;
            case '7':
                qsort((void *) cruiseSummaryData, cruiseSummaries, sizeof(cruiseSummaryData[0]), sortByCruiseNumber);
                break;
            case '8':
                maintain(cruiseSummaryData, cruiseDetailData, &cruiseSummaries, &cruiseDetails);
                break;
            case '9':
                repeat = 'n';
                break;
            default:
                printf("*** Error ***\n");
                break;
        }
        fflush(stdin);
    }

    displayCredits();

    return 0;
}

/*
    void displayCredits(void)
    Return Type - void
    Parameters - void
    Purpose - Print the program credits to the terminal window.
*/
void displayCredits(void)
{
    printf("Thank you for using Justin's cruise encyclopedia!\n");
}

/*
    void displayData(cruiseSummary_t *cruise)
    Return Type - Returns nothing
    Parameters - Take a pointer to a cruiseSummary_t struct
    Purpose - Accept a pointer to a cruiseSummary_t struct and print 
    the information stored within it in a formatted manner.
*/
void displayData(cruiseSummary_t *cruise)
{
    int i = 0;
    char *blankSpace = " "; // Used to help format the table
    printf("|%2i|%-36s|%-5i|%-40s\n", cruise->cruiseNumber, cruise->cruiseDescription, cruise->numberOfDays, cruise->destinations[i]);
    for (i = 1; i < cruise->numberOfDays; i++)
    {
        printf("|%2s|%-36s|%-5s|%-40s\n", blankSpace, blankSpace, blankSpace, cruise->destinations[i]);
    }
    printDash(WIDTH_SUMMARY);
}

/*
    void dataHeader(int id)
    Return Type - Return nothing
    Parameters - Accept an int ID from a cruiseSummary_t struct, so that it can be
    printed in the header
    Purpose - Print the headers for The displayData function. The headers
    align with the cruiseSummary_t structure. We accept an argument id
    so that we can include it in the headers. It makes the information
    easier to look through when 10+ cruises are printed on the terminal.
*/
void dataHeader(int id)
{
    /*
        The five strings below are column header strings. Storing them in variables
        instead of hardcoding them makes them easier to change, and more 
        importantly, we can format them using printf more easily.
    */
    char *idString = "ID";
    char *descriptionString = "Description";
    char *daysString = "#Days";
    char *destinationString = "Destinations";
    char *blankSpace = " "; // A blank string that assists with formatting
    printDash(WIDTH_SUMMARY); // A function to print an input a number of dashes 
    printf("|CRUISE #%3i SUMMARY\n", id);
    printDash(WIDTH_SUMMARY);
    printf("|%2s|%-36s|%-5s|%-40s\n", idString, descriptionString, daysString, destinationString);
    printDash(WIDTH_SUMMARY);
}

/*
    void displayMenu(void)
    Return Type - Return nothing
    Parameters - Expect no arguements
    Purpose - Print the program's menu options to the terminal.
*/
void displayMenu(void)
{
    printf("1. Display summary data for cruises\n2. Search for summary data by cruise length\n3. Search for summary data by destination\n4. Search data for maximum length\n5. Search data for maximum price per day\n");
    printf("6. Sort data by description\n7. Sort data by cruise number\n8. Maintain the data\n9. Exit the program\n");
}

/*
    void displayWelcomeMessage(void)
    Return Type - Nothing
    Parameters - Expect no arguments
    Purpose - Print the program's welcome message to the terminal.
*/
void displayWelcomeMessage(void)
{
    printf("Welcome to Justin's cruise encyclopedia!\n");
}

/*
    double getUserDouble(char *message)
    Return Type - return a user input non-negative double
    Parameters - expect a character string as an arguement to act as a user prompt
    Purpose - Repeatedly prompt the user with an input string message to input a
    positive double until a valid double is entered. Don't let the input double be 0.
*/
double getUserDouble(char *message)
{
    double input = -1;
    do
    {
        printf("%s", message);
        while ( scanf("%lf", &input) != 1)
        {
            fflush(stdin);
        }
    } while ( !(isPositive(input, "*** Error please input a positive, non-zero double ***")) );

    return input;
}

/*
    int getCruiseLength(char *message)
    Return Type - return a user input non-negative integer
    Parameters - expect a character string to act as a user prompt
    Purpose - Repeatedly prompt the user with an input string message 
    to input a positive integer. Do not let the positive integer be 0.
*/
int getCruiseLength(char *message)
{
    int input = -1;
    do
    {
        printf("%s", message);
        while ( scanf("%i", &input) != 1)
        {
            fflush(stdin);
        }
    } while ( !(isPositive(input, "*** Error please input a positive, non-zero integer ***")) );

    return input;
}

/*
    char *getUserStringInput(char *message, int lengthOfInput)
    Return Type - return a pointer to a user input character string
    Parameters - accept a character pointer that will act as a user prompt, and
    accept an integer length to determine the size of the user input string.
    Purpose - Prompt the user with an input message to enter a string of input length.
    The function gives malloc 4 attempts to be successful before returning a NULL value 
    to indicate failure. If malloc is successful, the function enters a do while loop
    to continuously reprompt the user with the input message until the enter a string
    that is not empty, but is shorter than the allowed amount. The function then returns
    the pointer to the input string. MAKE SURE TO USE "free" WHEREEVER THIS FUNCTION IS 
    CALLED!
*/
char* getUserStringInput(char *message, int lengthOfInput)
{
    char *buffer = malloc(sizeof(char) * (lengthOfInput + 1));
    char *newlinePtr = NULL;
    if ( buffer != NULL)
    {
        do
        {
            printf("%s", message);
            fgets(buffer, lengthOfInput, stdin);
            fflush(stdin);
            if ( (newlinePtr = strchr(buffer, '\n')) )
            {
                *newlinePtr = '\0';
            } 
        } while ( isStringTooLong(buffer, lengthOfInput - 1, "*** ERROR - Input destination is too long ***") || isStringTooShort(buffer, 1, "*** ERROR - Input destination is too short ***") );
    }
    return buffer;
}

/*
    bool isPositive(double x, char *errorMessage)
    Return Type - return true if the input double is not negative or zero. 
    otherwise, return false.
    Parameters - expect an input double to evaluate and an input errorMessage if 
    the function evaluates false.
    Purpose - Input a double x and evaluate if it is positive. The
    function accepts a double x because it accomodates integer 
    input. 
*/
bool isPositive(double x, char *errorMessage)
{
    bool result = false;
    if (x <= 0)
    {
        printf("%s\n", errorMessage);
    }
    else
    {
        result = true;
    }
    return result;
}

/*
    bool isStringTooLong(char *string, int length, char *errorMessage)
    Return Type - return true if the input string is as long as the input int length.
    otherwise, return false.
    Parameters - expect a string, an integer length, and an errorMessage.
    if the input string's length is equal to or longer than the input length, then 
    print the error message and return true. otherwise, return false.
    Purpose - This function is utilized in a while loop with fgets in 
    the getUserStringInput function. Its purpose is to make sure that
    the user did not enter a string that exceeds the length of the buffer. 
    It accepts the buffer and the input length of the buffer. If the buffer's
    length is equal to the input length, then the string is too long so the 
    function prints an error message and returns true. Otherwise, the function
    will return false.
*/
bool isStringTooLong(char *string, int length, char *errorMessage)
{
    bool result = false;
    if ( strlen(string) >= length )
    {
        printf("%s\n", errorMessage);
        result = true;
    }
    return result;
    
}

/*
    bool isStringTooShort(char *string, int length, char *errorMessage)
    Return Type - if the input string length is shorter than the input int length,
    then print the error message and return true. otherwise, return false.
    Parameters - expect an input string, an input int length, and an errorMessage.
    the length of the input string will be evaluated against the input length.
    Purpose - This function is used alongside fgets in a while loop within
    the getUserStringInput function. Its purpose is to accept a string as an
    argument and make sure that it is not under the minimum input length. If
    the string is under the minimum input length, then function prints an 
    error message and returns true. Otherwise, the function returns false.
*/
bool isStringTooShort(char *string, int length, char *errorMessage)
{
    bool result = false;
    if ( strlen(string) < length )
    {
        printf("%s\n", errorMessage);
        result = true;
    }
    return result;
}

/*
    bool isValidOption(char option, const char VALID_OPTIONS[], char *errorMessage)
    Return Type - if the input char option is within the char array VALID_OPTIONS,
    then return true. otherwise, print the errorMessage and return false
    Parameters - accept a char option, an array of chars VALID_OPTIONS[], and an
    errorMessage. the function will search for the option within the VALID_OPTIONS[]
    array
    Purpose - This function accepts a character option and a constant array
    of VALID_OPTIONS. If the input option is found within the VALID_OPTIONS array,
    then we know that the input option is a valid option. Otherwise, the input option
    is not a valid option so print the error message and return false.
*/
bool isValidOption(char option, const char VALID_OPTIONS[], char *errorMessage)
{
    bool results = false;
    int i = 0;

    while ( VALID_OPTIONS[i] != '\0' )
    {
        if ( option == VALID_OPTIONS[i] )
            results = true;
        i++;
    }
    if (!results)
        printf("%s\n", errorMessage);

    return results;
}

/*
    int loadDetailData(cruiseDetail_t cruiseDetailData[], int maxNumCruiseDetails, int lineLength)
    Return Type - returns an integer total of the number of lines read by the function from the file 
    "Viking Prices 2023.txt"
    Parameters - Accept an array of cruiseDetail_t structs, an integer maximum number of cruise details that can be read
    in, and a lineLength to determine the buffer[] size when reading in data from the file.
    Purpose - This function accepts an array of cruiseDetail_t structs, a defined integer
    for the maximum number of cruise details, and a defined line length for reading lines from the file. 
    The function tries to open the file "Viking Prices 2023.txt" in read mode. If the 
    function fails to open the file, the program exits (This is critical information and 
    user will have to run the program again anyways). If the function successfully opens the file, 
    it will read line by line inputting each delimited data point into a cruiseDetail_t struct 
    stored within the cruiseDetailData[] array. Finally, it will return the number of records 
    read into the program by fgets.
*/
int loadDetailData(cruiseDetail_t cruiseDetailData[], int maxNumCruiseDetails, int lineLength)
{
    int cruiseNum = 0;
    int destinationNum = 0;

    FILE *cruiseDetails = fopen("Viking Prices 2023.txt", "r");
    if ( cruiseDetails == NULL )
    {
        printf("Unable to open file...\n");
        return 0;
    }
    else
    {
        char buffer[lineLength + 1];
        char *startPtr, *endPtr;
        while ( fgets(buffer, lineLength, cruiseDetails) && cruiseNum < maxNumCruiseDetails )
        {
            destinationNum = 0;
            startPtr = buffer;

            while ( *startPtr < '1' || *startPtr > '9')
                startPtr++;

            endPtr = strchr(startPtr, '|');
            cruiseDetailData[cruiseNum].cruiseNumber = atoi(startPtr);
            startPtr = endPtr + 1;

            endPtr = strchr(startPtr, '|');
            cruiseDetailData[cruiseNum].departureYear = atoi(startPtr);
            startPtr = endPtr + 1;

            endPtr = strchr(startPtr, '|');
            cruiseDetailData[cruiseNum].departureMonth = atoi(startPtr);
            startPtr = endPtr + 1;

            endPtr = strchr(startPtr, '|');
            cruiseDetailData[cruiseNum].price = (double) atoi(startPtr);
            startPtr = endPtr + 1;
            
            cruiseNum++;
        }
        fclose(cruiseDetails);
    }

    return cruiseNum;
}

/*
    int loadSummaryData(cruiseSummary_t cruiseSummaryData[], int maxNumCruiseSummaries, int lineLength)
    Return Type - return an integer total of the number of lines read in from the file
    "Viking Cruises 2023.txt"
    Parameters - Accept an array of cruiseSummary_t, an integer maximum number of cruise summaries 
    that can be read in from the file, and an integer lineLength that determines the size of the
    buffer[] to be read in.
    Purpose - Accepts an array of cruiseSummary_t structs, a defined maximum number of 
    cruise summaries, and a defined line length for reading lines in from the file. The function 
    attempts to open the file "Viking Cruises 2023.txt" in read mode. If the function fails 
    in opening the file, the program exits (This information is critical to the program, so the 
    user would have to rerun it again anyways). If the function succeeds in opening the file, then
    the function will read from the file line by line. It will input each delimited data point into 
    a cruiseSummary_t struct stored within the cruiseSummaryData[] array. Finally, it will return the
    number of records read in by fgets.
*/

int loadSummaryData(cruiseSummary_t cruiseSummaryData[], int maxNumCruiseSummaries, int lineLength)
{
    int cruiseNum = 0;
    int destinationNum = 0;

    FILE *cruiseSummaries = fopen("Viking Cruises 2023.txt", "r");
    if ( cruiseSummaries == NULL )
    {
        printf("Unable to open file...\n");
        return 0;
    }
    else
    {
        char buffer[lineLength + 1];
        char *startPtr, *endPtr, *temp;
        int i = 0;

        while ( fgets(buffer, lineLength, cruiseSummaries) && cruiseNum < maxNumCruiseSummaries )
        {
            startPtr = buffer;

            while ( *startPtr < '1' || *startPtr > '9')
                startPtr++;
                
            endPtr = strchr(startPtr, '|');
            cruiseSummaryData[cruiseNum].cruiseNumber = atoi(startPtr);
            startPtr = endPtr + 1;
            
            endPtr = strchr(startPtr, '|');
            strncpy(cruiseSummaryData[cruiseNum].cruiseDescription, startPtr, (endPtr - startPtr));
            startPtr = endPtr + 1;

            endPtr = strchr(startPtr, '|');
            cruiseSummaryData[cruiseNum].numberOfDays = atoi(startPtr);
            startPtr = endPtr + 1;

            for (i = 0; i < cruiseSummaryData[cruiseNum].numberOfDays; i++)
            {
                if ( (endPtr = strchr(startPtr, '|')) ) 
                {
                    strncpy(cruiseSummaryData[cruiseNum].destinations[i], startPtr, (endPtr - startPtr));
                    startPtr = endPtr + 1;
                }
                else 
                {
                    strcpy(cruiseSummaryData[cruiseNum].destinations[i], startPtr);
                }
            }

            temp = cruiseSummaryData[cruiseNum].destinations[i-1];
            temp = stripNewline(temp);

            cruiseNum++;
        }
        fclose(cruiseSummaries);
    }

    return cruiseNum;
}

/*
    void pause(char *message)
    Return Type - return nothing
    Parameters - accept a message to prompt the user
    Purpose - Accept a message to prompt the user to hit the return button
    to achieve some outcome. This function is used within other functions
    to give the program a more interactive feel.
*/
void pause(char *message)
{
    char input = '\0';
    do
    {
        printf("%s\n", message);
        scanf("%c", &input);
        fflush(stdin);
    } while ( input != '\n');
}

/*
    void printCruiseDetail(cruiseDetail_t *cruise)
    Return Type - return nothing
    Parameters - accept a cruiseDetail_t pointer so that the struct can be printed
    Purpose - Accept a pointer to a cruiseDetail_t struct and print out
    the relevant information in a formatted way.
*/
void printCruiseDetail(cruiseDetail_t *cruise)
{
    printf("|%2i | %5i | %5i | $%-12.2lf|\n", cruise->cruiseNumber, cruise->departureYear, cruise->departureMonth, cruise->price);
    printDash(WIDTH_DETAIL);
}

/*
    void printCruiseSummaries(cruiseSummary_t cruiseSummaryData[], int numCruises)
    Return Type - return nothing
    Parameters - accept an array of cruiseSummary_t structs and an integer number of 
    cruise summaries
    Purpose - Accept an array of cruiseSummary_t structs and a number of cruises.
    Continually call the displayData function with the current cruiseSummaryData[] index.
    The function will run until it has printed the input number of cruises.  
*/
void printCruiseSummaries(cruiseSummary_t cruiseSummaryData[], int numCruises)
{
    char input = '\0';
    for (int i = 0; i < numCruises; i++)
    {
        if (i != 0 && (i % 10) == 0)
        {
            pause("Press return to continue\n");
        }
        dataHeader(cruiseSummaryData[i].cruiseNumber);
        displayData(&cruiseSummaryData[i]);
    }
    pause("Press return to return to the main menu\n");
}

/*
    void printDash(int numDashes)
    Return Type - return nothing
    Parameters - accept an integer number of dashes
    Purpose - Accept an integer number and then print a line to the terminal
    with the input number of dashes followed by a new line character. This
    function is used throughout the print functions to help format.
*/
void printDash(int numDashes)
{
    for (int i = 0; i < numDashes; i++)
        printf("-");
    printf("\n");
}


/*
    void printDetailHeader(int id)
    Return Type - return nothing
    Parameters - accept an id of a cruiseDetail_t struct so that it can be displayed
    within the detail headers
    Purpose - Print the header for a cruiseDeatil_t struct. It accepts the 
    cruiseID because it makes it easier for the user to keep track of which
    cruises that they are looking at while using the program.
*/
void printDetailHeader(int id)
{
    /*
        The 5 strings below are the header strings. Storing them in variables
        makes them easier to change throughout the function as well as makes it
        easier to format them with printf statements. 
    */
    char *idString = "ID";
    char *departureYear = "Year";
    char *departureMonth = "Month";
    char *price = "Price";
    char *blankSpace = " ";
    printDash(WIDTH_DETAIL);
    printf("|%6s CRUISE #%3i DETAILS %7s|\n", blankSpace, id, blankSpace);
    printDash(WIDTH_DETAIL);
    printf("|%2s | %5s | %5s | %-13s|\n", idString, departureYear, departureMonth, price);
    printDash(WIDTH_DETAIL);
}

/*
    void searchDetailDataByMaxPrice(cruiseDetail_t *cruiseDetailData, int numDetails, cruiseSummary_t *cruiseSummaryData, int numSummaries)
    Return Type - return nothing
    Parameters - accept a pointer to a cruiseDetail_t array, an integer number of elements within the cruiseDetail_t
    array, accept a pointer to a cruiseSummary_t array, and an integer number of elements within the cruiseSummary_t
    array.
    Purpose - Accepts a pointer to an array of cruiseDetail_t structs and the number of items in the array. It
    also accepts a pointer to an array of cruiseSummary_t structs and the number of items in that array as well.
    The function prompts the user to enter the maximum price that they would like to pay per day. Upon receiving
    the input maximum price, it creates a loop to iterate over every item in the cruiseSummaryData array. Within 
    that loop it creates a second loop to iterate over every item in the cruiseDetailData array. In the second loop
    the function computes the average price by calling the computeAverage function and passing the function the price
    from the struct within cruiseDetailData and the number of days from the struct within the cruiseSumamryData.
*/
void searchDetailDataByMaxPrice(cruiseDetail_t *cruiseDetailData, int numDetails, cruiseSummary_t *cruiseSummaryData, int numSummaries)
{
    double maxPrice = getUserDouble("Please enter the maximum price you would like to pay per day: ");
    int j = 0;
    int counter = 0;
    int lock = 0;
    double average = 0;
    for (int i = 0; i < numSummaries; i++)
    {
        lock = 0; // Declare a lock variable to prevent a cruiseSummary_t struct from being printed 2x
        while (cruiseSummaryData->cruiseNumber == cruiseDetailData->cruiseNumber && j < numDetails)
        {
            average = COMPUTE_AVERAGE(cruiseDetailData->price, (double) cruiseSummaryData->numberOfDays);
            if (average <= maxPrice)
            {
                if (lock == 0)
                {
                    dataHeader(cruiseSummaryData->cruiseNumber);
                    displayData(cruiseSummaryData);
                    printDetailHeader(cruiseDetailData->cruiseNumber); 
                    lock = 1;
                }
                printCruiseDetail(cruiseDetailData); // Print the details for cruises that meet the critera
                counter++;
            }
            cruiseDetailData++;
            j++;
        }
        cruiseSummaryData++;
    }
    printf("We found %i matches\n", counter);
}

/*
    void searchSummaryDataByDestination(cruiseSummary_t cruiseSummaryData[], int numCruises, int destinationStringLength)
    Return Type - nothing
    Parameters - accept an array of cruiseSummary_t elements, an integer number of elements within the array,
    and accept an integer string to determine the size of the user input pattern
    Purpose - Accept an array of cruiseSummary_t sturcts, the number of elements within the array, and 
    the length of a destination string. The function will prompt the user to input a string. If the 
    input returns NULL, then the function will report an error and the function will return. However,
    if the funciton successfully gets a user input string, it will enter a pair of nested loops that 
    compares the user's input string to every destination stored within every cruiseSummary_t struct. 
    This comparison is case-insensitive and looks for partial matches. If any matches are found, 
    the information for that cruise summary will be printed. 
*/

void searchSummaryDataByDestination(cruiseSummary_t cruiseSummaryData[], int numCruises, int destinationStringLength)
{
    char *pattern = getUserStringInput("Enter a destination you would like to search for: ", destinationStringLength);
    if ( pattern == NULL )
    {
        printf("*** Error - Unable to process request at this time ***\n");
    }
    else
    {
        char temp[destinationStringLength];
        char *lowerTemp;
        pattern = stringToLower(pattern);
        int j = 0, counter = 0;
        for ( int i = 0; i < numCruises; i++ )
        {
            for (j = 0; j < cruiseSummaryData[i].numberOfDays; j++)
            {
                strcpy(temp, cruiseSummaryData[i].destinations[j]);
                lowerTemp = stringToLower(temp);
                if ( (strstr(lowerTemp, pattern)) )
                {
                    dataHeader(cruiseSummaryData[i].cruiseNumber);
                    displayData(&cruiseSummaryData[i]);
                    counter++;
                    break;
                }
            }
        }
        printf("We found %i matches\n", counter);
        free(pattern); // If pattern successfully returns, you have to free it to prevent memory leaks
    }
}

/*
    void searchSummaryDataByLength(cruiseSummary_t cruiseSummaryData[], int numCruises)
    Return Type - void
    Parameters - accept an array of cruiseSummary_t and an integer number of elements within the array.
    Purpose - Accept an array of cruiseSummary_t structs and the number of elements within the array.
    Prompt the user to input an integer length to see how long of a cruise they are looking for. After
    receiving user input, the function enters a loop that compares the input length to the numberOfDays
    within each cruiseSummary_t struct stored in the cruiseSummaryData[] array. If the input length 
    matches the numberOfDays within a cruiseSummary_t struct, then we print the entire cruise summary
    for that cruise.
*/
void searchSummaryDataByLength(cruiseSummary_t cruiseSummaryData[], int numCruises)
{
    int length = getCruiseLength("Please enter the cruise length you are looking for: ");
    int counter = 0;
    for (int i = 0; i < numCruises; i++)
    {
        if (length == cruiseSummaryData[i].numberOfDays)
        {
            dataHeader(cruiseSummaryData[i].cruiseNumber);
            displayData(&cruiseSummaryData[i]);
            counter++;
        }
    }
    printf("We found %i matches\n", counter);
}

/*
    void searchSummaryDataByMaxLength(cruiseSummary_t cruiseSummaryData[], int numCruises)
    Return Type - void
    Parameters - accept an array of cruiseSummary_t elements and an integer number of elements within
    the array.
    Purpose - Accept an array of cruiseSummary_t structs and the number of elements in the array.
    Prompt the user to input the maximum length that they would like to stay on a cruise. After
    receiving user input, iterate over every struct in the cruiseSummaryData[] array. If any 
    cruiseSummary_t struct's number of days is less than or equal to the user input length, then
    print out the cruise summary for that cruise.
*/
void searchSummaryDataByMaxLength(cruiseSummary_t cruiseSummaryData[], int numCruises)
{
    int length = getCruiseLength("Please enter the max length you would like to search for: ");
    int counter = 0;
    for (int i = 0; i < numCruises; i++)
    {
        if (length >= cruiseSummaryData[i].numberOfDays)
        {
            dataHeader(cruiseSummaryData[i].cruiseNumber);
            displayData(&cruiseSummaryData[i]);
            counter++;
        }
    }
    printf("We found %i matches\n", counter);
}


/*
    char selectOption(const char VALID_OPTIONS[], void (*fnPtr) (void))
    Return Type - a validated, user input char
    Parameters - accept a const array of chars, VALID_OPTIONS[], that will be used to validate the input option.
    also, accept a function pointer that will be called to display a specific message to the user!
    Purpose - This function accepts a constant array of valid options to pass to the validation function.
    It prompts the user for input by displaying the program's menu, collects input, and passes the input and
    valid options array to the validation function. The function repeats until a valid option is selected. 
    Then it returns the validated option.
*/
char selectOption(const char VALID_OPTIONS[], void (*fnPtr) (void)){
    char option = '\0';
    do
    {
        fnPtr();
        scanf("\n%c", &option);
        fflush(stdin); 
    } while ( !isValidOption(option, VALID_OPTIONS, "*** Error - Not a valid menu option ***") );
    
    return option;
}

/*
    int sortByCruiseNumber(const void *a, const void *b)
    Return Type - return a positive, negative, or zero integer to allow quicksort to sort in
    ascending order.
    Parameters - accept a void pointer a and a void pointer b.
    Purpose - This function will be passed to qsort and will evaluate an array of cruiseSummary_t
    elements by their cruiseNumbers.
*/
int sortByCruiseNumber(const void *a, const void *b)
{
    if ( ((cruiseSummary_t *)a)->cruiseNumber > ((cruiseSummary_t *)b)->cruiseNumber )
    {
        return 1;
    }
    else if ( ((cruiseSummary_t *)a)->cruiseNumber < ((cruiseSummary_t *)b)->cruiseNumber )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*
    int sortByDescription(const void *a, const void *b)
    Return Type - return a positive, negative, or zero integer to allow quicksort to sort in
    ascending order.
    Parameters - accept a void pointer a and a void pointer b
    Purpose - This function will be passed to qsort and will sort an array of cruiseSummary_t 
    elements by their descriptions
*/
int sortByDescription(const void *a, const void *b)
{
    if ( strcmp( (( (cruiseSummary_t *) a)->cruiseDescription), (( (cruiseSummary_t *) b)->cruiseDescription) ) > 0 )
    {
        return 1;
    }
    else if ( strcmp( (( (cruiseSummary_t *) a)->cruiseDescription), (( (cruiseSummary_t *) b)->cruiseDescription) ) < 0 )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*
    char* stringToLower(char *string)
    Return Type - return a char pointer to the input string
    Parameters - accept a pointer to the input string, so that it can be iterated over
    Purpose - Accept an argument character pointer as an agrument, then iterate over the entire string
    converting it to lowercase. Then return a pointer to the lowercase string.
*/
char* stringToLower(char *string)
{
    char *strPtr = string;
    while ( *strPtr )
    {
        *strPtr = tolower(*strPtr);
        strPtr++;
    }
    return string;
}

/*
    char* stripNewline(char *string)
    Return Type - return a char pointer to the input string
    Parameters - accept a pointer to the input string, so that it can be iterated over
    Purpose - Iterate over the string and eliminate newline characters and carriage returns
*/
char* stripNewline(char *string)
{
    char *strPtr = string;
    while ( *strPtr != '\0' )
    {
        if (*strPtr == '\n' || *strPtr == '\r')
            *strPtr = '\0';
        strPtr++;
    }

    return string;
}


/*
Test 1 - Erroneous Input
    Test Case 1:
        'abcde\n'
    Expected Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        abcde
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        abcde
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 2:
        '\n\n\n'
    Expected Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program



    Sample Output:
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program



    Test Case 3:
        'mmmmmmmmmmmmmmm\n'
    Expected Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        mmmmmmmmmmmmmmm
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        mmmmmmmmmmmmmmm
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 2 - Main Menu Option 1
    Test Case 1:
        '1\n\n'
    Expected Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu
    
    Test Case 2:
        'abc\n1\n\n\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        abc
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        abc
        *** Error - Not a valid menu option ***
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 3:
        '1\nabc\n\n\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue

        abc
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue

        abc
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 3 - Main Menu Option 2
    Test Case 1:
        '2\n10\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: 10
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        We found 3 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: 10
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        We found 3 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    
    Test Case 2: 
        '2\n-1\n0\n15\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: -1
        *** Error please input a positive, non-zero integer ***
        Please enter the cruise length you are looking for: 0
        *** Error please input a positive, non-zero integer ***
        Please enter the cruise length you are looking for: 15
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: -1
        *** Error please input a positive, non-zero integer ***
        Please enter the cruise length you are looking for: 0
        *** Error please input a positive, non-zero integer ***
        Please enter the cruise length you are looking for: 15
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 3:
        '2\n\n\n3'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: 


        3
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        2
        Please enter the cruise length you are looking for: 


        3
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 4 - Main Menu Option 3
    Test Case 1:
        '3\nbuda\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: buda
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        We found 5 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: buda
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        We found 5 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 2: 
        '3\n\nBordrecht\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: 
        *** ERROR - Input destination is too short ***
        Enter a destination you would like to search for: Bordrecht
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: 
        *** ERROR - Input destination is too short ***
        Enter a destination you would like to search for: Bordrecht
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 3: 
        '3\nWittenberg\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: Wittenberg
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        3
        Enter a destination you would like to search for: Wittenberg
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        We found 1 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 5 - Main Menu Option 4
    Test Case 1: 
        '4\n20\n'
    Expected Output:    
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: 20
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        We found 15 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: 20
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        We found 15 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    
    Test Case 2: 
        '4\n-1\n0\n9\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: -1
        *** Error please input a positive, non-zero integer ***
        Please enter the max length you would like to search for: 0
        *** Error please input a positive, non-zero integer ***
        Please enter the max length you would like to search for: 9
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        We found 6 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output:
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: -1
        *** Error please input a positive, non-zero integer ***
        Please enter the max length you would like to search for: 0
        *** Error please input a positive, non-zero integer ***
        Please enter the max length you would like to search for: 9
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        We found 6 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 3:
        '4\n4\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: 4
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        4
        Please enter the max length you would like to search for: 4
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 6 - Main Menu Option 5
    Test Case 1:
        '5\n250.00\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: 250.00
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  1 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 1 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  3 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 3 |  2023 |     3 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |    11 | $1999.00     |
        -----------------------------------
        We found 7 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: 250.00
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  1 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 1 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  3 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 3 |  2023 |     3 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |    11 | $1999.00     |
        -----------------------------------
        We found 7 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 2: 
        '5\n-1\n225\n'
    Expected Outcome:  
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: -1
        *** Error please input a positive, non-zero double ***
        Please enter the maximum price you would like to pay per day: 225
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: -1
        *** Error please input a positive, non-zero double ***
        Please enter the maximum price you would like to pay per day: 225
        We found 0 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 3:
        '5\n1000\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: 1000
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  1 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 1 |  2023 |     4 | $2499.00     |
        -----------------------------------
        | 1 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 1 |  2023 |     6 | $2699.00     |
        -----------------------------------
        | 1 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     9 | $2899.00     |
        -----------------------------------
        | 1 |  2023 |    10 | $2399.00     |
        -----------------------------------
        | 1 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  2 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 2 |  2023 |     4 | $2199.00     |
        -----------------------------------
        | 2 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 2 |  2023 |     6 | $2499.00     |
        -----------------------------------
        | 2 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 2 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 2 |  2023 |     9 | $3099.00     |
        -----------------------------------
        | 2 |  2023 |    10 | $2199.00     |
        -----------------------------------
        | 2 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  3 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 3 |  2023 |     3 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     4 | $2099.00     |
        -----------------------------------
        | 3 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 3 |  2023 |     6 | $2499.00     |
        -----------------------------------
        | 3 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     9 | $3099.00     |
        -----------------------------------
        | 3 |  2023 |    10 | $2299.00     |
        -----------------------------------
        | 3 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  4 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 4 |  2023 |     4 | $4699.00     |
        -----------------------------------
        | 4 |  2023 |     5 | $5299.00     |
        -----------------------------------
        | 4 |  2023 |     6 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     7 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     8 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     9 | $5299.00     |
        -----------------------------------
        | 4 |  2023 |    10 | $4299.00     |
        -----------------------------------
        | 4 |  2023 |    11 | $3999.00     |
        -----------------------------------
        | 4 |  2023 |    12 | $4299.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  5 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 5 |  2023 |     4 | $4299.00     |
        -----------------------------------
        | 5 |  2023 |     5 | $5199.00     |
        -----------------------------------
        | 5 |  2023 |     6 | $4799.00     |
        -----------------------------------
        | 5 |  2023 |     7 | $4299.00     |
        -----------------------------------
        | 5 |  2023 |     8 | $4999.00     |
        -----------------------------------
        | 5 |  2023 |     9 | $3799.00     |
        -----------------------------------
        | 5 |  2023 |    10 | $3499.00     |
        -----------------------------------
        | 5 |  2023 |    11 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  6 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 6 |  2023 |     5 | $3999.00     |
        -----------------------------------
        | 6 |  2023 |     6 | $3799.00     |
        -----------------------------------
        | 6 |  2023 |     7 | $3499.00     |
        -----------------------------------
        | 6 |  2023 |     8 | $3499.00     |
        -----------------------------------
        | 6 |  2023 |     9 | $3999.00     |
        -----------------------------------
        | 6 |  2023 |    10 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  7 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 7 |  2023 |     5 | $4299.00     |
        -----------------------------------
        | 7 |  2023 |     6 | $3299.00     |
        -----------------------------------
        | 7 |  2023 |     7 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |     8 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |     9 | $3799.00     |
        -----------------------------------
        | 7 |  2023 |    10 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |    11 | $2299.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  8 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 8 |  2023 |     3 | $2699.00     |
        -----------------------------------
        | 8 |  2023 |     4 | $2999.00     |
        -----------------------------------
        | 8 |  2023 |     5 | $3399.00     |
        -----------------------------------
        | 8 |  2023 |     6 | $3199.00     |
        -----------------------------------
        | 8 |  2023 |     7 | $2499.00     |
        -----------------------------------
        | 8 |  2023 |     8 | $2499.00     |
        -----------------------------------
        | 8 |  2023 |     9 | $3399.00     |
        -----------------------------------
        | 8 |  2023 |    10 | $2499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  9 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 9 |  2023 |     4 | $3399.00     |
        -----------------------------------
        | 9 |  2023 |     5 | $3699.00     |
        -----------------------------------
        | 9 |  2023 |     6 | $3399.00     |
        -----------------------------------
        | 9 |  2023 |     7 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |     8 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |     9 | $3699.00     |
        -----------------------------------
        | 9 |  2023 |    10 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |    11 | $2199.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 10 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |10 |  2023 |     6 | $3999.00     |
        -----------------------------------
        |10 |  2023 |     7 | $3599.00     |
        -----------------------------------
        |10 |  2023 |     8 | $3599.00     |
        -----------------------------------
        |10 |  2023 |     9 | $4399.00     |
        -----------------------------------
        |10 |  2023 |    10 | $3199.00     |
        -----------------------------------
        |10 |  2023 |    11 | $3199.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 11 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |11 |  2023 |     4 | $5499.00     |
        -----------------------------------
        |11 |  2023 |     5 | $5699.00     |
        -----------------------------------
        |11 |  2023 |     6 | $5499.00     |
        -----------------------------------
        |11 |  2023 |     7 | $5299.00     |
        -----------------------------------
        |11 |  2023 |     8 | $5299.00     |
        -----------------------------------
        |11 |  2023 |     9 | $5699.00     |
        -----------------------------------
        |11 |  2023 |    10 | $5499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 12 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |12 |  2023 |     3 | $2999.00     |
        -----------------------------------
        |12 |  2023 |     4 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     5 | $3999.00     |
        -----------------------------------
        |12 |  2023 |     6 | $3699.00     |
        -----------------------------------
        |12 |  2023 |     7 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     8 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     9 | $3999.00     |
        -----------------------------------
        |12 |  2023 |    10 | $3299.00     |
        -----------------------------------
        |12 |  2023 |    11 | $2999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 13 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |13 |  2023 |     3 | $3699.00     |
        -----------------------------------
        |13 |  2023 |     4 | $3499.00     |
        -----------------------------------
        |13 |  2023 |     5 | $4299.00     |
        -----------------------------------
        |13 |  2023 |     6 | $3999.00     |
        -----------------------------------
        |13 |  2023 |     7 | $3699.00     |
        -----------------------------------
        |13 |  2023 |     9 | $4299.00     |
        -----------------------------------
        |13 |  2023 |    10 | $3599.00     |
        -----------------------------------
        |13 |  2023 |    11 | $2999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 14 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |14 |  2023 |     5 | $4499.00     |
        -----------------------------------
        |14 |  2023 |     6 | $4499.00     |
        -----------------------------------
        |14 |  2023 |     7 | $3799.00     |
        -----------------------------------
        |14 |  2023 |     8 | $3799.00     |
        -----------------------------------
        |14 |  2023 |     9 | $4999.00     |
        -----------------------------------
        |14 |  2023 |    10 | $3999.00     |
        -----------------------------------
        |14 |  2023 |    11 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 15 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |15 |  2023 |     5 | $5799.00     |
        -----------------------------------
        |15 |  2023 |     8 | $5299.00     |
        -----------------------------------
        |15 |  2023 |     9 | $5499.00     |
        -----------------------------------
        |15 |  2023 |    10 | $5999.00     |
        -----------------------------------
        |15 |  2023 |    11 | $6299.00     |
        -----------------------------------
        |15 |  2023 |    12 | $5999.00     |
        -----------------------------------
        |15 |  2024 |     1 | $6499.00     |
        -----------------------------------
        |15 |  2024 |     2 | $6499.00     |
        -----------------------------------
        |15 |  2024 |     3 | $6299.00     |
        -----------------------------------
        We found 117 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output:
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        5
        Please enter the maximum price you would like to pay per day: 1000
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  1 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 1 |  2023 |     4 | $2499.00     |
        -----------------------------------
        | 1 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 1 |  2023 |     6 | $2699.00     |
        -----------------------------------
        | 1 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 1 |  2023 |     9 | $2899.00     |
        -----------------------------------
        | 1 |  2023 |    10 | $2399.00     |
        -----------------------------------
        | 1 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  2 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 2 |  2023 |     4 | $2199.00     |
        -----------------------------------
        | 2 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 2 |  2023 |     6 | $2499.00     |
        -----------------------------------
        | 2 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 2 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 2 |  2023 |     9 | $3099.00     |
        -----------------------------------
        | 2 |  2023 |    10 | $2199.00     |
        -----------------------------------
        | 2 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  3 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 3 |  2023 |     3 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     4 | $2099.00     |
        -----------------------------------
        | 3 |  2023 |     5 | $2899.00     |
        -----------------------------------
        | 3 |  2023 |     6 | $2499.00     |
        -----------------------------------
        | 3 |  2023 |     7 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     8 | $1999.00     |
        -----------------------------------
        | 3 |  2023 |     9 | $3099.00     |
        -----------------------------------
        | 3 |  2023 |    10 | $2299.00     |
        -----------------------------------
        | 3 |  2023 |    11 | $1999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  4 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 4 |  2023 |     4 | $4699.00     |
        -----------------------------------
        | 4 |  2023 |     5 | $5299.00     |
        -----------------------------------
        | 4 |  2023 |     6 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     7 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     8 | $4499.00     |
        -----------------------------------
        | 4 |  2023 |     9 | $5299.00     |
        -----------------------------------
        | 4 |  2023 |    10 | $4299.00     |
        -----------------------------------
        | 4 |  2023 |    11 | $3999.00     |
        -----------------------------------
        | 4 |  2023 |    12 | $4299.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  5 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 5 |  2023 |     4 | $4299.00     |
        -----------------------------------
        | 5 |  2023 |     5 | $5199.00     |
        -----------------------------------
        | 5 |  2023 |     6 | $4799.00     |
        -----------------------------------
        | 5 |  2023 |     7 | $4299.00     |
        -----------------------------------
        | 5 |  2023 |     8 | $4999.00     |
        -----------------------------------
        | 5 |  2023 |     9 | $3799.00     |
        -----------------------------------
        | 5 |  2023 |    10 | $3499.00     |
        -----------------------------------
        | 5 |  2023 |    11 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  6 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 6 |  2023 |     5 | $3999.00     |
        -----------------------------------
        | 6 |  2023 |     6 | $3799.00     |
        -----------------------------------
        | 6 |  2023 |     7 | $3499.00     |
        -----------------------------------
        | 6 |  2023 |     8 | $3499.00     |
        -----------------------------------
        | 6 |  2023 |     9 | $3999.00     |
        -----------------------------------
        | 6 |  2023 |    10 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  7 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 7 |  2023 |     5 | $4299.00     |
        -----------------------------------
        | 7 |  2023 |     6 | $3299.00     |
        -----------------------------------
        | 7 |  2023 |     7 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |     8 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |     9 | $3799.00     |
        -----------------------------------
        | 7 |  2023 |    10 | $2799.00     |
        -----------------------------------
        | 7 |  2023 |    11 | $2299.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  8 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 8 |  2023 |     3 | $2699.00     |
        -----------------------------------
        | 8 |  2023 |     4 | $2999.00     |
        -----------------------------------
        | 8 |  2023 |     5 | $3399.00     |
        -----------------------------------
        | 8 |  2023 |     6 | $3199.00     |
        -----------------------------------
        | 8 |  2023 |     7 | $2499.00     |
        -----------------------------------
        | 8 |  2023 |     8 | $2499.00     |
        -----------------------------------
        | 8 |  2023 |     9 | $3399.00     |
        -----------------------------------
        | 8 |  2023 |    10 | $2499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE #  9 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        | 9 |  2023 |     4 | $3399.00     |
        -----------------------------------
        | 9 |  2023 |     5 | $3699.00     |
        -----------------------------------
        | 9 |  2023 |     6 | $3399.00     |
        -----------------------------------
        | 9 |  2023 |     7 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |     8 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |     9 | $3699.00     |
        -----------------------------------
        | 9 |  2023 |    10 | $2699.00     |
        -----------------------------------
        | 9 |  2023 |    11 | $2199.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 10 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |10 |  2023 |     6 | $3999.00     |
        -----------------------------------
        |10 |  2023 |     7 | $3599.00     |
        -----------------------------------
        |10 |  2023 |     8 | $3599.00     |
        -----------------------------------
        |10 |  2023 |     9 | $4399.00     |
        -----------------------------------
        |10 |  2023 |    10 | $3199.00     |
        -----------------------------------
        |10 |  2023 |    11 | $3199.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 11 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |11 |  2023 |     4 | $5499.00     |
        -----------------------------------
        |11 |  2023 |     5 | $5699.00     |
        -----------------------------------
        |11 |  2023 |     6 | $5499.00     |
        -----------------------------------
        |11 |  2023 |     7 | $5299.00     |
        -----------------------------------
        |11 |  2023 |     8 | $5299.00     |
        -----------------------------------
        |11 |  2023 |     9 | $5699.00     |
        -----------------------------------
        |11 |  2023 |    10 | $5499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 12 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |12 |  2023 |     3 | $2999.00     |
        -----------------------------------
        |12 |  2023 |     4 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     5 | $3999.00     |
        -----------------------------------
        |12 |  2023 |     6 | $3699.00     |
        -----------------------------------
        |12 |  2023 |     7 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     8 | $3499.00     |
        -----------------------------------
        |12 |  2023 |     9 | $3999.00     |
        -----------------------------------
        |12 |  2023 |    10 | $3299.00     |
        -----------------------------------
        |12 |  2023 |    11 | $2999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 13 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |13 |  2023 |     3 | $3699.00     |
        -----------------------------------
        |13 |  2023 |     4 | $3499.00     |
        -----------------------------------
        |13 |  2023 |     5 | $4299.00     |
        -----------------------------------
        |13 |  2023 |     6 | $3999.00     |
        -----------------------------------
        |13 |  2023 |     7 | $3699.00     |
        -----------------------------------
        |13 |  2023 |     9 | $4299.00     |
        -----------------------------------
        |13 |  2023 |    10 | $3599.00     |
        -----------------------------------
        |13 |  2023 |    11 | $2999.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 14 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |14 |  2023 |     5 | $4499.00     |
        -----------------------------------
        |14 |  2023 |     6 | $4499.00     |
        -----------------------------------
        |14 |  2023 |     7 | $3799.00     |
        -----------------------------------
        |14 |  2023 |     8 | $3799.00     |
        -----------------------------------
        |14 |  2023 |     9 | $4999.00     |
        -----------------------------------
        |14 |  2023 |    10 | $3999.00     |
        -----------------------------------
        |14 |  2023 |    11 | $3499.00     |
        -----------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        -----------------------------------
        |       CRUISE # 15 DETAILS        |
        -----------------------------------
        |ID |  Year | Month | Price        |
        -----------------------------------
        |15 |  2023 |     5 | $5799.00     |
        -----------------------------------
        |15 |  2023 |     8 | $5299.00     |
        -----------------------------------
        |15 |  2023 |     9 | $5499.00     |
        -----------------------------------
        |15 |  2023 |    10 | $5999.00     |
        -----------------------------------
        |15 |  2023 |    11 | $6299.00     |
        -----------------------------------
        |15 |  2023 |    12 | $5999.00     |
        -----------------------------------
        |15 |  2024 |     1 | $6499.00     |
        -----------------------------------
        |15 |  2024 |     2 | $6499.00     |
        -----------------------------------
        |15 |  2024 |     3 | $6299.00     |
        -----------------------------------
        We found 117 matches
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 7 - Main Menu Option 6
    Test Case 1: 
        '6\n1\n\n\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

    Test Case 2:
        '8\n1\nMy new cruise\nport 1\nport 2\nport 3\nq\n5\n6\n1\n\n\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        1
        Please enter a cruise description between 1 and 36 characters: My new cruise
        Please enter a destination and click 'enter': port 1
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: port 2
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: port 3
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: q
        New summary successfully created
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        5
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 16 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |16|My new cruise                       |3    |port 1                                  
        |  |                                    |     |port 2                                  
        |  |                                    |     |port 3                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        1
        Please enter a cruise description between 1 and 36 characters: My new cruise
        Please enter a destination and click 'enter': port 1
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: port 2
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: port 3
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: q
        New summary successfully created
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        5
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 16 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |16|My new cruise                       |3    |port 1                                  
        |  |                                    |     |port 2                                  
        |  |                                    |     |port 3                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 8 - Main Menu Option 7
    Test Case 1: 
        '6\n7\n1\n\n\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        7
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
    Sample Output: 
                Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        6
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        7
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        1
        ------------------------------------------------------------
        |CRUISE #  1 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 1|Rhine Getaway                       |8    |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rudesheim                       
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Breisach                                
        |  |                                    |     |Basel                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  2 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 2|Romantic Danube                     |7    |Budapest                                
        |  |                                    |     |Budapest/Danube                         
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Passau                                  
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Regensburg                              
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  3 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 3|Danbe Waltz                         |8    |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Bratislava                              
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Krems/Danube                            
        |  |                                    |     |Linz                                    
        |  |                                    |     |Passau                                  
        |  |                                    |     |Passau                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  4 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 4|Grand European Tour                 |15   |Amsterdam                               
        |  |                                    |     |Kinderkijk                              
        |  |                                    |     |Cologne                                 
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Miltenberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Bamberg                                 
        |  |                                    |     |Nuremburg                               
        |  |                                    |     |Regensburg                              
        |  |                                    |     |Passau                                  
        |  |                                    |     |Melk                                    
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  5 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 5|Paris to Swiss Alps                 |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz/Rhine                           
        |  |                                    |     |Mainz                                   
        |  |                                    |     |Speyer                                  
        |  |                                    |     |Strasbourg                              
        |  |                                    |     |Basel/Zurich                            
        |  |                                    |     |Zurich                                  
        |  |                                    |     |Zurich                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  6 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 6|Cities of Light                     |12   |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |Reims/Trier                             
        |  |                                    |     |Trier/Bernkastel                        
        |  |                                    |     |Cochem                                  
        |  |                                    |     |Koblenz                                 
        |  |                                    |     |Heidelberg                              
        |  |                                    |     |Wurzburg                                
        |  |                                    |     |Main River/Bamberg                      
        |  |                                    |     |Nurembur/Prague                         
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  7 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 7|Paris and the Heart of Normandy     |8    |Paris                                   
        |  |                                    |     |Paris                                   
        |  |                                    |     |La Roche-Guyon/Giverny                  
        |  |                                    |     |Seine/Rouen                             
        |  |                                    |     |Normandy Beaches                        
        |  |                                    |     |Seine/Les Andelys                       
        |  |                                    |     |Le Pecq/Paris                           
        |  |                                    |     |Paris                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  8 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 8|Lyon & Provence                     |8    |Avignon                                 
        |  |                                    |     |Aries                                   
        |  |                                    |     |Avignon                                 
        |  |                                    |     |Viviers                                 
        |  |                                    |     |Tourmon/Vienne                          
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        |  |                                    |     |Lyon                                    
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE #  9 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        | 9|Chateaux, Rivers & Wine             |8    |Bordeaux                                
        |  |                                    |     |Bordeaux/Cadillac                       
        |  |                                    |     |Cadillac/Garonne & Dordogne Rivers      
        |  |                                    |     |Libourne/Saint-Emilion                  
        |  |                                    |     |Bourg/Blaye                             
        |  |                                    |     |Medoc & Margaux Wine Country            
        |  |                                    |     |Gironde River/Bordeaux                  
        |  |                                    |     |Bordeaux                                
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 10 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |10|Holland & Belgium                   |10   |Amsterdam                               
        |  |                                    |     |Amsterdam                               
        |  |                                    |     |Hoorn                                   
        |  |                                    |     |Bordrecht/Rotterdam                     
        |  |                                    |     |Kinderdijk                              
        |  |                                    |     |Wesel                                   
        |  |                                    |     |Nijmegen                                
        |  |                                    |     |Maastrich                               
        |  |                                    |     |Antwerp                                 
        |  |                                    |     |Antwerp                                 
        ------------------------------------------------------------
        Press return to continue


        ------------------------------------------------------------
        |CRUISE # 11 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |11|Capitals of Eastern Europe          |17   |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Vienna                                  
        |  |                                    |     |Bratislave/Danube Bend                  
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Mohacs                                  
        |  |                                    |     |Vukovar/Ilok                            
        |  |                                    |     |Novi Sad                                
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Donji Milanovac/Danube River            
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Pleven                                  
        |  |                                    |     |Constantja                              
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 12 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |12|Passage to Eastern Europe           |11   |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Budapest                                
        |  |                                    |     |Kalocsa                                 
        |  |                                    |     |Osijek                                  
        |  |                                    |     |Belgrade                                
        |  |                                    |     |Golubac/Danube River                    
        |  |                                    |     |Vidin                                   
        |  |                                    |     |Veliko Tarnovo                          
        |  |                                    |     |Bucharest                               
        |  |                                    |     |Bucharest                               
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 13 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |13|Elegant Elbe                        |10   |Berlin                                  
        |  |                                    |     |Berlin                                  
        |  |                                    |     |Potsdam/Wittenberg                      
        |  |                                    |     |Wittenberg                              
        |  |                                    |     |Meissen                                 
        |  |                                    |     |Dresden                                 
        |  |                                    |     |Elbe River/Saxon Switzerland            
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        |  |                                    |     |Prague                                  
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 14 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |14|Portugal's River of Gold            |10   |Lisbon                                  
        |  |                                    |     |Lisbon                                  
        |  |                                    |     |Porto                                   
        |  |                                    |     |Regua                                   
        |  |                                    |     |Douro River/Barca d'Alva                
        |  |                                    |     |Salamanca                               
        |  |                                    |     |Douro River/Porto                       
        |  |                                    |     |Regua/Porto                             
        |  |                                    |     |Porto                                   
        |  |                                    |     |Porto                                   
        ------------------------------------------------------------
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|Pharaohs & Pyramids                 |12   |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Luxor                                   
        |  |                                    |     |Qena                                    
        |  |                                    |     |Esna                                    
        |  |                                    |     |Aswan                                   
        |  |                                    |     |Aswan/Kom Ombo                          
        |  |                                    |     |Edfu/Luxor                              
        |  |                                    |     |Cairo                                   
        |  |                                    |     |Cairo                                   
        ------------------------------------------------------------
        Press return to return to the main menu


        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program

Test 9 - Main Menu Option 8
    Test Case 1: 
        '8\n'
    Expected Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu

Test 10 - Main Menu Option 9
    Test Case 1: 
        '9\n'
    Expected Output:
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        9
        Thank you for using Justin's cruise encyclopedia!
    Sample Output: 
        Welcome to Justin's cruise encyclopedia!
        1. Display summary data for cruises
        2. Search for summary data by cruise length
        3. Search for summary data by destination
        4. Search data for maximum length
        5. Search data for maximum price per day
        6. Sort data by description
        7. Sort data by cruise number
        8. Maintain the data
        9. Exit the program
        9
        Thank you for using Justin's cruise encyclopedia!

*/
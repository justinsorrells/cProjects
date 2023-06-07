#include "project6.h"

/*
    Declare function prototypes
*/
bool createNewCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries);
bool deleteCrusieSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries);
void displayMaitenanceMenu(void);
void displayModificationMenu(void);
int getUserIntegerInput(char *message);
char* getUserStringInput(char *message, int lengthOfInput);
int highestCruiseID(cruiseSummary_t cruiseSummaryData[], int *cruiseSummary);
bool isValidOption(char option, const char VALID_OPTIONS[], char *errorMessage);
bool modifyCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries);
int searchForID(cruiseSummary_t cruiseSummaryData[], int *cruiseSumaries, int id);
char selectOption(const char VALID_OPTIONS[], void (*fnPtr) (void));
bool saveCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries);
bool updateCruiseDescription(cruiseSummary_t *cruiseSummary, int length);
int updateDestinations(cruiseSummary_t *cruiseSummary);

/*
    Declare the necessary menu options arrays for the file
*/
const char MAINTAIN_OPTIONS[] = { '1', '2', '3', '4', '5', '\0' }; // This array will validate the maintain functions input
const char MOD_OPTIONS[] = { '1', '2', '3', '4', '\0' }; // This array will validate the modification option's input

// Pass the cruiseSummaryData and the cruiseDetail data into the maintain function. 
// Also, pass the number of elements in each array.
void maintain(cruiseSummary_t cruiseSummaryData[], cruiseDetail_t cruiseDetail_t[], int *cruiseSummaries, int *cruiseDetails)
{
    char repeat = 'y';
    char option = '\0';
    while ( repeat == 'y' )
    {
        option = selectOption(MAINTAIN_OPTIONS, displayMaitenanceMenu);
        switch (option)
        {
            case '1':
                if (createNewCruiseSummary(cruiseSummaryData, cruiseSummaries))
                {
                    printf("New summary successfully created\n");
                }
                else
                {
                    printf("***Error creating cruise summary***\n");
                }
                break;
            case '2':
                if ( !modifyCruiseSummary(cruiseSummaryData, cruiseSummaries) )
                {
                    printf("***Error invalid ID***\n");
                }
                else
                {
                    printf("Cruise Summary Successfully updated!\n");
                }
                break;
            case '3':
                if ( !deleteCrusieSummary(cruiseSummaryData, cruiseSummaries) )
                {
                    printf("***Error invalid ID***\n");
                }
                else 
                {
                    printf("Cruise Summary Successfully deleted!\n");
                }
                break;
            case '4':
                if (!saveCruiseSummary(cruiseSummaryData, cruiseSummaries))
                {
                    printf("***Error saving new file***\n");
                }
                else
                {
                    printf("File successfully saved!\n");
                }
                break;
            case '5':
                repeat = 'n';
                break;
            default:
                printf("*** ERROR ***\n");
                break;
        }
    }
}

/*
    bool createNewCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
    Return Type - Return true if successful and false if unsuccessful.
    Parameters - Accept the cruiseSummaryData array and the number of elements in the array
    Purpose - If the cruiseSummary count is under the maximum, then allow the user to create 
    a new cruiseSummaryData[] entry. Otherwise, return false.
*/
bool createNewCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
{
    bool result = false;
    bool updateDescription = false;
    if (*cruiseSummaries < CRUISE_SUMMARIES)
    {
        cruiseSummary_t *newCruiseSummary = &cruiseSummaryData[*cruiseSummaries]; // create a pointer to the first open array element
        updateDescription = updateCruiseDescription(newCruiseSummary, CRUISE_DESCRIPTION);
        int counter = updateDestinations(newCruiseSummary);
        newCruiseSummary->numberOfDays = counter; // set the new summary's number of days equal to the return value of the counter
        newCruiseSummary->cruiseNumber =  (highestCruiseID(cruiseSummaryData, cruiseSummaries) + 1); // give the cruise an id = to the current highest id + 1
        /*
            if both of the function to take user input return successfully then
            increment the global cruiseSummaries count by 1 and return true 
        */
        if (counter > 0 && updateDescription)
        {
            *cruiseSummaries += 1;
            result = true;
        }
    }
    else
    {
        printf("***Max cruise summaries***");
    }
    return result;
}

/*
    bool deleteCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
    Return Type - Return true if the entery was successfully delete. otherwise, return false
    Parameters - Accept the cruiseSummaryData[] array and the number of elements in the array.
    Purpose - Allow the user to input a cruise ID, then search for the index of the element with
    the matching cruise ID in the cruiseSummaryData array. Once found, NULLIFY the entry, decrement
    the number of cruiseSummaries, and move all of the array elements past index in
    cruiseSummaryData[] toward 0 by 1.
*/
bool deleteCrusieSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
{
    bool result = false;
    int userInput = getUserIntegerInput("Please enter the cruise ID that you would like to delete: ");
    int index = 0;
    int i;
    if ( (index = searchForID(cruiseSummaryData, cruiseSummaries, userInput)) != -1)
    {
        cruiseSummaryData[index] = EMPTY_SUMMARY; // Set the element to a null cruiseSummary_t struct
        for (i = index + 1; i < *(cruiseSummaries); i++) // Push all other elements past index toward 0 by 1
        {
            cruiseSummaryData[i - 1] = cruiseSummaryData[i]; 
        }
        *cruiseSummaries -= 1; // decrement the number of cruise summaries.
        result = true;
    }

    return result;
}

/*
    void displayMaintenanceMenu(void)
    Return Type - Return nothing
    Parameters - Accpet nothing
    Purpose - Display the maintenance menu options to the user.
*/
void displayMaitenanceMenu(void)
{
    printf("1. Add data for a cruise summary\n2. Modify data for a cruise summary\n3. Delete data for a cruise summary\n");
    printf("4. Save all cruise summary to a new file\n5. Return to the main menu\n");
}

/*
    void displayModificationMenu(void)
    Return Type - Return Nothing
    Parameters - Accept Nothing
    Purpose - Display the modification menu options to the user.
*/
void displayModificationMenu(void)
{
    printf("1. Modify Cruise Description\n2. Modify Destinations\n3. Return to maintenance menu\n");
}

/*
    int getUserIntegerInput(char *message)
    Return Type - Return a user input integer
    Parameters - Accept a string to prompt the user
    Purpose - Prompt the user to input an integer, then return the integer
*/
int getUserIntegerInput(char *message)
{
    int temp = 0;
    do
    {
        printf("%s", message);
        fflush(stdin);
    } while ( (scanf("%i", &temp)) != 1 );
    
    return temp;
}

/*
    int highestCruiseID(cruiseSummary_t cruiseSummaryData[], int *cruiseSummary)
    Return Type - Return either 0 or the current highest cruise ID stored within
    the cruiseSummaryData[] array
    Parameters - Accept the cruiseSummaryData[] array and the number of elements stored
    within it
    Purpose - Iterate over the cruiseSummaryData[] array and find the highest cruise ID, 
    then return it
*/
int highestCruiseID(cruiseSummary_t cruiseSummaryData[], int *cruiseSummary)
{
    int highest = 0;
    if (*cruiseSummary > 0)
    {
        highest = cruiseSummaryData[0].cruiseNumber;
    }
    for (int i = 1; i < *cruiseSummary; i++)
    {
        if (highest < cruiseSummaryData[i].cruiseNumber)
            highest = cruiseSummaryData[i].cruiseNumber;
    }

    return highest;
}

/*
    bool modifyCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
    Return Type - Return true if the desired element was successfully accessed and false
    if the function was unable to access the desired element
    Parameters - Accept the cruiseSummaryData[] array and the number of elements within
    it
    Purpose - Allow the user to input a cruise ID, then search for the index of the cruise ID
    within the cruiseSummaryData[] array. If successfully returned, allow the user to repeatedly 
    modify the array element's description and destinations. Otherwise, return false
*/
bool modifyCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
{
    int userInput = getUserIntegerInput("Please enter the cruise ID that you would like to modify: ");
    int index = 0;
    bool result = false;
    if ( (index = searchForID(cruiseSummaryData, cruiseSummaries, userInput)) != -1)
    {
        cruiseSummary_t *tempPointer = &cruiseSummaryData[index];
        printf("ID: %i\nDescription: %s\nNumber of Days: %i\nDestinations: ", tempPointer->cruiseNumber, tempPointer->cruiseDescription, tempPointer->numberOfDays);
        for (int i = 0; i < tempPointer->numberOfDays; i++)
        {
            printf("%s\n", tempPointer->destinations[i]);
        }

        char repeat = 'y';
        char option = '\0';
        int count = 0;
        while ( repeat == 'y' )
        {
            option = selectOption(MOD_OPTIONS, displayModificationMenu);
            switch (option)
            {
                case '1':
                    if (updateCruiseDescription(tempPointer, CRUISE_DESCRIPTION))
                    {
                        printf("Cruise description successfully updated!\n");
                    }
                    else
                    {
                        printf("***Error updating cruise description***\n");
                    }
                    break;
                case '2':
                    count = updateDestinations(tempPointer);
                    if (count > 0)
                    {
                        tempPointer->numberOfDays = count;
                        printf("Destinations successfully updated!\n");
                    }
                    else
                    {
                        printf("***Error updating destinations***\n");
                    }
                    break;
                case '3':
                    repeat = 'n';
                    break;
                default:
                    printf("***Error invalid option\n");
                    break;
            }
        }
        result = true;
    }

    return result;
}

/*
    int searchForId(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries, int id)
    Return Type - Return the index of the array element with a matching cruise ID to the user
    input cruise ID, or return -1 if the cruise ID was not successfully found
    Parameters - Accept the cruiseSummaryData[] array, the total number of elements stored
    within the array, and the user input id to search for
    Purpose - Iterate over the cruiseSummaryData[] looking for a matching cruise ID to the
    one that the user input
*/
int searchForID(cruiseSummary_t cruiseSummaryData[], int *cruiseSumaries, int id)
{
    int index = -1;
    for (int i = 0; i < *cruiseSumaries; i++)
    {
        if (id == cruiseSummaryData[i].cruiseNumber)
        {
            index = i;
            break;
        }
    }

    return index;
}

/*
    bool saveCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
    Return Type - Return true if the file was successfully opened and return false if otherwise
    Parameters - Accept the cruiseSummaryData[] array and the number of elements stored within it
    Purpose - Get a user input filename, open the file, and then write all of the data stored within
    the cruiseSummaryData[] array. Delemit the data with the '|' or pipe delimiter
*/
bool saveCruiseSummary(cruiseSummary_t cruiseSummaryData[], int *cruiseSummaries)
{
    char *fileName = getUserStringInput("Please enter the name of the file you would like to save your data to: ", STANDARD_STRING);
    FILE *file = fopen(fileName, "w");
    bool result = false;

    if ( file != NULL)
    {
        char *test;
        for (int i = 0; i < *(cruiseSummaries); i++)
        {
            fprintf(file, "%i|%s|%i", cruiseSummaryData[i].cruiseNumber, cruiseSummaryData[i].cruiseDescription, cruiseSummaryData[i].numberOfDays);          
            for (int j = 0; j < cruiseSummaryData[i].numberOfDays; j++)
            {
                fprintf(file, "|%s", cruiseSummaryData[i].destinations[j]);
            }
            if (i < (*cruiseSummaries - 1))
                fprintf(file, "\n");
        }
        fclose(file);
        result = true;
    }

    if (fileName != NULL)
        free(fileName);

    return result;
}

/*
    bool updateCruiseDescription(cruiseSummary_t *cruiseSummary, int length)
    Return Type - Return true if the function successfully read in a user input string
    and return false if otherwise
    Parameters - Accept a pointer to a cruiseSummary_t struct and the allowed length of the user input
    Purpose - Pass a pointer to a cruiseSummary_t struct and allow the user to write to the description
    member of the struct.
*/
bool updateCruiseDescription(cruiseSummary_t *cruiseSummary, int length)
{
    bool result = false;
    char *temp = getUserStringInput("Please enter a cruise description between 1 and 36 characters: ", length);
    if (temp != NULL)
    {
        strcpy(cruiseSummary->cruiseDescription, temp);
        free(temp);
        result = true;
    }
    return result;
}

/*
    int updateDestinations(cruiseSummary_t *cruiseSummary)
    Return Type - returns the count of the destinations successfully entered by the user
    Parameters - Accepts a pointer to a cruiseSummary_t struct
    Purpose - Allow the user to continuously enter strings into a cruiseSummary's destinations
    array.
*/
int updateDestinations(cruiseSummary_t *cruiseSummary)
{
    int counter = 0;
    char *temp = getUserStringInput("Please enter a destination and click 'enter': ", STANDARD_STRING);
    if (temp != NULL)
    {
        strcpy(cruiseSummary->destinations[counter], temp);
        counter++;
        free(temp);
    }
    while ( counter < MAX_DAYS && (temp = getUserStringInput("Type 'q' then 'enter' to quit or continue to enter destinations up to 21: ", STANDARD_STRING)))
    {
        if ( !(strcmp("q", temp)) )
        {
            break;
        }
        strcpy(cruiseSummary->destinations[counter], temp);
        counter++;
        free(temp);
    }

    if (temp != NULL)
        free(temp);

    return counter;
}

/*
Test 1 - Maintenance Menu Option 1 
    Test Case 1: 
        '8\n1\nMy new cruise\nport 1\nport 2\nport 3\nq\n5\n2\n3\n'
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
        2
        Please enter the cruise length you are looking for: 3
        ------------------------------------------------------------
        |CRUISE # 16 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |16|My new cruise                       |3    |port 1                                  
        |  |                                    |     |port 2                                  
        |  |                                    |     |port 3                                  
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
        2
        Please enter the cruise length you are looking for: 3
        ------------------------------------------------------------
        |CRUISE # 16 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |16|My new cruise                       |3    |port 1                                  
        |  |                                    |     |port 2                                  
        |  |                                    |     |port 3                                  
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

Test 2 - Maintenance Menu Option 2
    Test Case 1:
        '8\n2\n15\n1\nnew cruise\n2\nhome\nq\n3\n5\n2\n\1\n'
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
        2
        Please enter the cruise ID that you would like to modify: 15
        ID: 15
        Description: Pharaohs & Pyramids
        Number of Days: 12
        Destinations: Cairo
        Cairo
        Cairo
        Luxor
        Luxor
        Qena
        Esna
        Aswan
        Aswan/Kom Ombo
        Edfu/Luxor
        Cairo
        Cairo
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        1
        Please enter a cruise description between 1 and 36 characters: new cruise
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        2
        Please enter a destination and click 'enter': home
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: q
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        3
        Cruise Summary Successfully updated!
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
        2
        Please enter the cruise length you are looking for: 1
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|new cruise                          |1    |home                                    
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
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        2
        Please enter the cruise ID that you would like to modify: 15
        ID: 15
        Description: Pharaohs & Pyramids
        Number of Days: 12
        Destinations: Cairo
        Cairo
        Cairo
        Luxor
        Luxor
        Qena
        Esna
        Aswan
        Aswan/Kom Ombo
        Edfu/Luxor
        Cairo
        Cairo
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        1
        Please enter a cruise description between 1 and 36 characters: new cruise
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        2
        Please enter a destination and click 'enter': home
        Type 'q' then 'enter' to quit or continue to enter destinations up to 21: q
        1. Modify Cruise Description
        2. Modify Destinations
        3. Return to maintenance menu
        3
        Cruise Summary Successfully updated!
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
        2
        Please enter the cruise length you are looking for: 1
        ------------------------------------------------------------
        |CRUISE # 15 SUMMARY
        ------------------------------------------------------------
        |ID|Description                         |#Days|Destinations                            
        ------------------------------------------------------------
        |15|new cruise                          |1    |home                                    
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

Test 3 - Maintenance Menu Option 3
    Test Case 1: 
        '8\n3\n1\n3\n2\n3\n3\n3\n4\n3\n5\n3\n6\n3\n7\n3\n8\n3\n9\n3\n10\n5\n\1\n\n'
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
        3
        Please enter the cruise ID that you would like to delete: 1
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 2
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 3
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 4
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 5
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 6
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 7
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 8
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 9
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 10
        Cruise Summary Successfully deleted!
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
        8
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 1
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 2
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 3
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 4
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 5
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 6
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 7
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 8
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 9
        Cruise Summary Successfully deleted!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu
        3
        Please enter the cruise ID that you would like to delete: 10
        Cruise Summary Successfully deleted!
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
    
Test 4 - Maintenance Menu Option 4
    Test Case 1:
        '8\n4\nnewFile.txt\n'
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
        4
        Please enter the name of the file you would like to save your data to: newFile.txt
        File successfully saved!
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
        4
        Please enter the name of the file you would like to save your data to: newFile.txt
        File successfully saved!
        1. Add data for a cruise summary
        2. Modify data for a cruise summary
        3. Delete data for a cruise summary
        4. Save all cruise summary to a new file
        5. Return to the main menu

Test 5 - Maintenance Menu Option 5
    Test Case 1:
        '8\n5\n9\n'
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
        8
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
        9
        Thank you for using Justin's cruise encyclopedia!

*/
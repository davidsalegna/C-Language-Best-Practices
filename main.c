/**
 * @author David Salegna, Jonathan Bogue, Noah Conn, Gian Garnica, Derik Schmitz
 * @brief This program manages students in a course. Files can be created, read, downloaded, and loaded
 * @date 2023-03-06
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#define NAME_LENGTH 10

// DCL36-C: This static double is declared with static external linkage to be used in a later macro function and cannot change
static double student_tax = 27;

// PRE01-C: Parentheses are properly placed around macro parameter names
// PRE02-C: Macro replacement list is properly parenthesized
// PRE11-C: Macro definition does not conclude with a semicolon
#define CALCULATE_COST(x, y) ((x) * (y))

// PRE10-C: Multistatement Macro definition is wrapped in a do-while loop so it can be used as a single line statement
#define CALCULATE_TAX(x, y)                              \
    do                                                   \
    {                                                    \
        double cost_total = (double)CALCULATE_COST(x, y); \
        printf("%.2f", (cost_total * (1 + (student_tax / 100))));          \
    } while (0)

typedef struct student
{
    char name[NAME_LENGTH]; // STR30-C: Uses array representation in case the name needs to be modified
    int gender;
    int age;
} student;

void menu(int flag, int *num);
void read(student *p, int *num);
void init(int *num);
void show(student *p, int *num);
void open(student *p, int *num, int flag);
void save(student *p, int *num);
void cost(int *num);
void logUser();
void *erase(void *pointer);

student *data = NULL;
// STR11-C: No specified dimensions so string literal assignment will automatically include a null terminator
char organization_name[] = "ISU IT"; 

/**
 * @brief Begins the program
 *
 * @return int Returns 0 if the program ran an quit successfully
 */
int main()
{
    // DCL30-C: Declares following variables within execution of program but not outside of it
    int num = 0;
    int flag = 0;

    logUser();
    menu(flag++, &num);
    return 0;
}

/**
 * @brief Prompts the user to choose a function of the program
 *
 * @param flag Indicates whether data already exists (and needs to be freed)
 * @param num The number of students in the class
 */
void menu(int flag, int *num)
{
    int choice;
    // STR30-C: Uses array representation because it will be modified
    // STR11-C: Array is not initialized to a string literal, so explicit dimensions are valid
    char num_buffer[4];
    int len;    // Length of the input string
    char c;     // Character used to flush stdin

    printf("\nWelcome to the %s class management system!\n", organization_name);
    do
    {
        choice = -1;
        printf("\t1) Add Students\n\t2) View Class\n\t3) Save Class as File\n\t4) Load Class File\n\t5) Calculate Cost of Class\n\t6) Quit\nEnter Option: ");

        /* STR31-C: All uses of fgets use the middle variable to know how many characters to read
                    In all occurences, it is at most the length of the buffer (fgets will stop just 
                    before using the whole buffer)
        */
        fgets(num_buffer, 2, stdin);
        while ((c = getchar()) != '\n' && c != EOF); // There's at least one trailing \n
        // ERR33-C: The error doesn't need to be checked because it will be caught by the switch
        sscanf(num_buffer, "%d", &choice); // Convert char to integer

        // MSC20-C: Switch statement does not transition into complex blocks, instead performs simplistic instructions for each case.
        // MSC01-C: Logical completeness in break statements for cases as appropriate, also seen in if statements with matching else statements.
        // MSC17-C: Each switch case has a break statement with the (acceptable) exception of a clearly acknowledged instance of fallthrough.
        switch (choice)
        {
        case 1:
            printf("\nEnter the number of students in the class: ");
            // STR32-C: fgets ensures only as many as a 3-digit number can be entered into the array of size 4
            if ((len = strlen(fgets(num_buffer, 4, stdin))) == 3 && num_buffer[2] != '\n') 
            {
                while ((c = getchar()) != '\n' && c != EOF);
            }
            if (sscanf(num_buffer, "%d", num) != 1 || *num < 1) 
            {
                *num = 0;
                printf("\nERROR: Invalid input. Input must be an integer in range (1-999).\nWARNING: Numbers above this range will be truncated.\nERROR: Add Students function failed. Please try again.\n");
                break;
            }
            init(num);
            read(data, num);
            break;
        case 2:
            show(data, num);
            // ***Fallthrough intended here.
        case 3:
            save(data, num);
            break;
        case 4:
            open(data, num, flag);
            break;
        case 5:
            cost(num);
            break;
        case 6:
            printf("\nQuitting application...");
            break;
        default:
            printf("\nERROR: Invalid input. Please enter an integer (1-6).\nWARNING: Integers greater than 1 digit will be truncated.\n");
            break;
        }
        printf("\n");
    } while (choice != 6);
    exit(0);
}

/**
 * @brief Allocates sufficient data for num students
 *
 * @param num The number of students in the class
 */
void init(int *num)
{
    // MEM35-C: Sufficient memory is allocated for data based on size of student
    data = (student *)malloc((*num) * sizeof(student));
    // MSC15-C: In loops such as this one, rather than checking if the current element has gone beyond its bounds (which could easily
    // lead to undefined behavior), the element is checked to ensure it remains within its bounds, ending the loop upon reaching said bounds.
    for (int i = 0; i < *num; i++)
    {
        memset(data->name, 0, NAME_LENGTH * sizeof(char));
        data->gender = 0;
        data->age = 0;
        data++;
    }
    data -= *num;
    read(data, num);
}

/**
 * @brief Fills students information using data provided by the user
 *
 * @param p The pointer to the first of num student structs
 * @param num The number of students in the class
 */
void read(student *p, int *num)
{
    int len = 0;
    char num_buffer[3]; // Used to accept gender and age data (<= 2 digits + \n)
    char c; // Used to flush stdin on attempted buffer overflow
    int read_failed = 0;

    for (int i = 0; i < *num; i++)
    {
        printf("\n\tStudent Name: ");
        // If the entire buffer is used, name may be truncated and input may need to be flushed
        if ((len = strlen(fgets(p->name, NAME_LENGTH, stdin))) == NAME_LENGTH - 1) 
        {
            printf("\nWARNING: Potential overflow! Student's name may be truncated. Name's should be less than %d characters long.\n\n", NAME_LENGTH);
            while ((c = getchar()) != '\n' && c != EOF);
        }
        // Trim newline from student name
        p->name[strlen(p->name) - 1] = '\0';
        
        printf("\tStudent Gender [1=Male, 2=Female, 3=Other]: ");
        fgets(num_buffer, 2, stdin);
        while ((c = getchar()) != '\n' && c != EOF); // There's at least one trailing \n

        // ERR33-C: Checking to see if a integer was parsed from the string (and if it's valid)
        if (sscanf(num_buffer, "%d", &(p->gender)) != 1 || p->gender < 1 || p->gender > 3) 
        {
            printf("\nERROR: Ivalid input. Input should be an integer (1-3).\nWARNING: Integers greater than 1 digit will be truncated.\nERROR: Add Students function failed. Please try again.\n");
            *num = 0; // Number of students set to zero, making the data unreadable
            read_failed = 1;
            break;
        } 

        printf("\tStudent Age: ");
        if ((len = strlen(fgets(num_buffer, 3, stdin))) == 2 && num_buffer[1] != '\n') 
        {
            while ((c = getchar()) != '\n' && c != EOF); // There's at least one trailing \n
        }
        // ERR33-C: Checking to see if a integer was parsed from the string (and if it's valid)
        if (sscanf(num_buffer, "%d", &(p->age)) != 1 || p->age < 0) 
        {
            printf("\nERROR: Ivalid input. Input should have be a positive integer (1-99).\nWARNING: Integers greater than 1 digit will be truncated.\nERROR: Add Students function failed. Please try again.\n");
            *num = 0; // Number of students set to zero, making the data unreadable
            read_failed = 1;
            break;
        } 

        printf("\nStudent added! %d students left to add.\n", *num - i - 1);
        p++;
    }
    if (read_failed != 1) {
        printf("All students have been added.\n");
    }
    menu(1, num);
}

/**
 * @brief Outputs the current class data
 *
 * @param student_ptr The pointer to the first of num student structs
 * @param num The number of students in the class
 */
void show(student *student_ptr, int *num)
{
    if (*num < 1)
    {
        printf("\nNo student data to display. You may enter new, or load existing data.\n");
    }
    else
    {
        for (int i = 0; i < *num; i++)
        {
            printf("\nStudent Name:\t%s\n", student_ptr->name);

            if (student_ptr->gender == 1)
            {
                printf("\tGender:\tMale\n");
            }
            else if (student_ptr->gender == 2)
            {
                printf("\tGender:\tFemale\n");
            }
            else
            {
                printf("\tGender:\tOther\n");
            }

            printf("\tAge:\t%d\n", student_ptr->age);

            // Increment student
            student_ptr++;
        }
    }

    menu(1, num); // Return to user prompt
}

/**
 * @brief Saves the student data to class_list.txt
 *
 * @param p The pointer to the student data
 * @param num The number of students in the class
 */
void save(student *p, int *num)
{
    FILE *fp = fopen("class_list.txt", "w+");

    if (fp)
    {
        fwrite(p, sizeof(student), *num, fp);
        fclose(fp);
    }
    printf("\nClass Saved.\n");
    menu(1, num);
}

/**
 * @brief Loads in student data from class_list.txt
 *
 * @param p The pointer to the student data
 * @param num The number of students in the class
 * @param flag Flag to indicate if data needs to be freed
 */
void open(student *p, int *num, int flag)
{
    *num = 0;
    FILE *fp = fopen("class_list.txt", "r");
    if (fp) // ERR33-C: If fopen fails, the user is alerted and the program returns to the prompt 
    { 
        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        *num = size / (sizeof(student));
        fseek(fp, 0L, SEEK_SET);

        if (flag == 1)
            free(data); // MEM34-C: The data variable was dynamically allocated and thus can be freed
        data = (student *) malloc((*num) * sizeof(student));
        p = data;

        fread(data, sizeof(student), *num, fp);
        printf("\nClass of %d students loaded.\n", *num);
    }
    else
    {
        printf("Read Failed!\n");
    }

    menu(1, num);
}

/**
 * @brief Utlizies macro statements to calculate the costs to maintain a student class
 *
 * @param p The Pointer to the first of num student structs
 * @param num The number of students in the class
 */
void cost(int *num)
{
    char curr_type[2];
    char num_buffer[6];
    char c;
    char dollar = '$';
    // STR00-C: This scenario necessitates the wchar_t type
    wchar_t euro = L'€';
    if (*num < 1)
    {
        printf("\nNo student data to calculate. You may enter new, or load existing data.\n");
    }
    else
    {
        printf("\n\tCurrency Type [1=Dollars, 2=Euro]: ");
        fgets(curr_type, 2, stdin);
        while ((c = getchar()) != '\n' && c != EOF); // There's at least one trailing \n
        // ERR33-C: Checking to see if string input is valid
        if (!(curr_type[0] == '1' || curr_type[0] == '2')) {
            printf("\nERROR: Invalid input. Currency Type should be an integer (1-2).\nERROR: Calculate Cost of Class function failed. Please try again.\n");
            return;
        }

        int *cost_per = malloc(sizeof(int));
        printf("\tPlease enter the cost per student: ");
        
        // Print correct currency type
        // STR38-C: wchar_t functions used to print character to stdout
        if (curr_type[0] == '1') {
            putchar(dollar);
        }
        else if (curr_type[0] == '2') {
            putwchar(euro);
        }

        // Get cost-per-student
        if (strlen(fgets(num_buffer, 6, stdin)) == 5 && num_buffer[4] != '\n') {
            while ((c = getchar()) != '\n' && c != EOF);
        }
        if (sscanf(num_buffer, "%d", cost_per) != 1 || *cost_per < 0) 
        {
            printf("\nERROR: Ivalid input. Input should have been a positive integer.\nWARNING: Integers greater than 5 digits will be truncated.\nERROR: Calculate Cost of Class function failed. Please try again.\n");
            return;
        } 

        // Print results
        if (curr_type[0] == '1') {
            printf("\tTotal Cost: $%d\n", CALCULATE_COST(*cost_per, *num));
            printf("\tTotal Cost w/ %.1f%% Tax Rate: $",  student_tax);
            CALCULATE_TAX(*cost_per, *num); // Make call to multi-statement macro to calculate taxes
            printf("\n");
        }
        else if (curr_type[0] == '2') {
            printf("\tTotal Cost: ");
            putwchar(euro); // STR38-C: wchar_t functions used to print character to stdout
            printf("%d\n\tTotal Cost w/ %.1f%% Rate: ", CALCULATE_COST(*cost_per, *num), student_tax);
            putwchar(euro); // STR38-C: wchar_t functions used to print character to stdout
            CALCULATE_TAX(*cost_per, *num); // Make call to multi-statement macro to calculate taxes
            printf("\n");
        }
        // MEM34-C: The cost_per variable was dynamically allocated and thus can be freed
        // MEM30-C: Since cost_per is being freed here, it will not be accessed anymore
        free(cost_per);
    }
    menu(1, num);
}

// MSC41-C: This code to log the last user is erased immediately after the file it is written to is closed, ensuring its security
// (at least within the application).
/**
 * @brief
 */
void logUser()
{
    char code[20];

    printf("Enter a unique code that is 20 characters or less to log who last used the system (only admins will have access to that file): ");
    fgets(code, sizeof(code), stdin);

    FILE *fp = fopen("user_log.txt", "w+");

    if (fp)
    {
        fwrite(code, sizeof(code), 1, fp);
        fclose(fp);
    }
    erase(code);
}

/**
 * @brief
 *
 * @param pointer
 * @return void*
 */
void *erase(void *pointer)
{
    int size_to_remove = sizeof(pointer);
    volatile unsigned char *p = pointer;

    while (size_to_remove--)
    {
        *p++ = 0;
    }
    return pointer;
}

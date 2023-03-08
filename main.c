/**
 * @author David Salegna, Jonathan Bogue, Noah Conn, Gian Garnica, Derik Schmitz
 * @brief This program manages students in a course. Files can be created, read, downloaded, and loaded
 * @date 2023-03-06
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_LENGTH 40

// PRE01-C: Parentheses are properly placed around macro parameter names
// PRE02-C: Macro replacement list is properly parenthesized
// PRE11-C: Macro definition does not conclude with a semicolon
#define CALCULATE_COST(x, y) ((x) * (y))

typedef struct student {
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
void cost(student *p, int *num);
void logUser();
void *erase(void *pointer);

student *data = NULL;

/**
 * @brief Begins the program
 * 
 * @return int Returns 0 if the program ran an quit successfully
 */
int main() {
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
void menu(int flag, int *num) {
    int choice = 0;

    printf("\nWelcome to the class management system!\n");
    do
    {
        printf("\t1) Add Students\n\t2) View Class\n\t3) Save Class as File\n\t4) Load Class File\n\t5) Cost of Class\n\t6) Quit\nEnter Option: ");

        scanf("%d", &choice);

        // MSC20-C: Switch statement does not transition into complex blocks, instead performs simplistic instructions for each case.
        // MSC01-C: Logical completeness in break statements for cases as appropriate, also seen in if statements with matching else statements.
        // MSC17-C: Each switch case has a break statement with the (acceptable) exception of a clearly acknowledged instance of fallthrough.
        switch (choice)
        {
            case 1:
                printf("\nEnter the number of students in the class: ");
                scanf("%d", num);
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
                cost(data, num);
                break;
            case 6:
                printf("\nQuitting application...");
                break;
            default:
                printf("\nError. Cannot identify your input!\n");
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
void init(int *num) {
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
void read(student *p, int *num) {
    for (int i = 0; i < *num; i++)
    {
        printf("\n\tStudent Name: ");
        scanf("%s", p->name);
        printf("\tStudent Gender [1=Male, 2=Female, 3=Other]: ");
        scanf("%d", &(p->gender));
        printf("\tStudent Age: ");
        scanf("%d", &(p->age));
        printf("Student added! %d students left to add.\n", *num - i - 1);
        p++;
    }
    printf("\nAll students have been added.\n");
    menu(1, num);
}

/**
 * @brief Outputs the current class data
 * 
 * @param student_ptr The pointer to the first of num student structs
 * @param num The number of students in the class
 */
void show(student *student_ptr, int *num) {
    if (*num < 1) {
        printf("\nNo student data to display. You may enter new, or load existing data.\n");
    } else {
        for (int i = 0; i < *num; i++)
        {
            printf("\nStudent Name: %s\n", student_ptr->name);
        
            if (student_ptr->gender == 1) {
                printf("\tGender: Male\n");
            } else if (student_ptr->gender == 2) {
                printf("\tGender: Female\n");
            } else {
                printf("\tGender: Other\n");
            }

            printf("\tAge: %d\n", student_ptr->age);
        
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
void save(student *p, int *num) {
    FILE *fp = fopen("class_list.txt", "w+");
    
    if (fp) {
        fwrite(p, sizeof(student), *num, fp);
        fclose(fp);
    }
    printf("Class Saved.\n");
    menu(1, num);
}

/**
 * @brief Loads in student data from class_list.txt
 * 
 * @param p The pointer to the student data
 * @param num The number of students in the class
 * @param flag Flag to indicate if data needs to be freed
 */
void open(student *p, int *num, int flag) {
    *num = 0;
    FILE *fp = fopen("class_list.txt", "r");
    if (fp) {
        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        *num = size / (sizeof(student));
        fseek(fp, 0L, SEEK_SET);

        if (flag == 1)
            free(data); // MEM34-C: The data variable was dynamically allocated and thus can be freed
        data = (student *)malloc((*num) * sizeof(student));
        p = data;

        fread(data, sizeof(student), *num, fp);
        printf("Class of %d students loaded.\n", *num);
    }
    else {
        printf("Read Failed!\n");
    }

    menu(1, num);
}

/**
 * @brief 
 * 
 * @param p 
 * @param num 
 */
void cost(student *p, int *num) {
    int *costPer = malloc(sizeof(int));
    printf("\tPlease enter the cost per student: ");
    scanf("%d", costPer);
    printf("Total cost will be: %d", CALCULATE_COST(*costPer, *num));
    free(costPer); // MEM30-C: Since costPer is being freed here, it will not be accessed anymore
    menu(1, num);
}

// MSC41-C: This code to log the last user is erased immediately after the file it is written to is closed, ensuring its security
// (at least within the application).
/**
 * @brief 
 * 
 */
void logUser() {
    char code[20];

    printf("Enter a unique code that is 20 characters or less to log who last used the system (only admins will have access to that file): ");
    fgets(code, sizeof(code), stdin);

    FILE *fp = fopen("user_log.txt", "w+");

    if (fp) {
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
void *erase(void *pointer) {
    int sizeToRemove = sizeof(pointer);
    volatile unsigned char *p = pointer;
    
    while (sizeToRemove--) 
    {
        *p++ = 0;
    }
    return pointer;
}

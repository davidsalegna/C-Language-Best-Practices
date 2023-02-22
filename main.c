#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_LENGTH 40

//PRE01-C: Parentheses are properly placed around macro parameter names
//PRE02-C: Macro replacement list is properly parenthesized
//PRE11-C: Macro definition does not conclude with a semicolon
#define CALCULATE_COST(x, y) ((x) * (y))

typedef struct student
{
    char name[NAME_LENGTH];
    int gender;
    int age;
} student;

void menu(int flag, int *num);
void read(student *p, int *num);
void init(int *num);
void show(student *p, int *num);
//void showline();
void open(student *p, int *num, int flag);
void save(student *p, int *num);
void cost(student *p, int *num);

student *data = NULL;

int main()
{
    //DCL30-C: Declares following variables within execution of program but not outside of it
    int num = 0;
    int flag = 0;
    menu(flag++, &num);
    return 0;
}

void menu(int flag, int *num)
{
    int choice = 0;

    printf("Welcome to the class management system!\n");
    printf("\t1) Add Students\n\t2) View Class\n\t3) Save Class as File\n\t4) Load Class File\n\t5) Cost of Class\n\t6) Quit\n");

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Enter the number of students in the class: ");
        scanf("%d", num);
        printf("\n");
        init(num);
        read(data, num);
        return;
    case 2:
        show(data, num);
    case 3:
        save(data, num);
        return;
    case 4:
        open(data, num, flag);
    case 5:
        cost(data, num);
        return;
    case 6:
        exit(0);
    default:
        printf("\033[0;31mError: Cannot identify your input!\033[0m");
        return;
    }
}

void init(int *num)
{
    //MEM35-C: Sufficient memory is allocated for data based on size of student
    data = (student *)malloc((*num) * sizeof(student));
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

void read(student *p, int *num)
{
    for (int i = 0; i < *num; i++)
    {
        printf("\tStudent Name: ");
        scanf("%s", p->name);
        printf("\tStudent Gender [1=Male, 2=Female, _=Other]: ");
        scanf("%d", &(p->gender));
        printf("\tStudent Age: ");
        scanf("%d", &(p->age));
        printf("Student added! %d students left to add.\n", *num - i - 1);
        p++;
    }
    printf("All students have been added.\n");
    menu(1, num);
}

void show(student *p, int *num)
{
    for (int i = 0; i < *num; i++)
    {
        printf("\tStudent Name: %s\n", p->name);
        if (p->gender == 1)
            printf("\tGender: Male\n");
        else if (p->gender == 2)
            printf("\tGender: Female\n");
        else
            printf("\tGender: Other\n");
        printf("\tAge: %d\n", p->age);
        p++;
    }
    menu(1, num);
}

void save(student *p, int *num)
{
    FILE *fp = fopen("class_list.txt", "w+");
    if (fp)
    {
        fwrite(p, sizeof(student), *num, fp);
        fclose(fp);
    }
    menu(1, num);
}

void open(student *p, int *num, int flag)
{
    *num = 0;
    FILE *fp = fopen("class_list.txt", "r");
    if (fp)
    {
        fseek(fp, 0L, SEEK_END);
        long size = ftell(fp);
        *num = size / (sizeof(student));
        fseek(fp, 0L, SEEK_SET);

        if (flag == 1)
            free(data); //MEM34-C: The data variable was dynamically allocated and thus can be freed
        data = (student *)malloc((*num) * sizeof(student));
        p = data;

        fread(data, sizeof(student), *num, fp);
        printf("Class of %d students loaded.\n", *num);
    }
    else
        printf("Read Failed!\n");
    menu(1, num);
}

void cost(student *p, int *num) {
    int *costPer = malloc(sizeof(int));
    printf("\tPlease enter the cost per student: ");
    scanf("%d", &costPer);
    printf("Total cost will be: %d", CALCULATE_COST(*costPer, *num));
    free(costPer); //MEM30-C: Since costPer is being freed here, it will not be accessed anymore
    menu(1, num);
}

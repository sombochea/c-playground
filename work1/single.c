#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define BUFFER_SIZE 128
#define MAX_NO_OF_UNITS 4
#define NAME_SIZE 20

const char *FILE_STUDENT_DATA_PATH = "./data/students.test.txt";

struct person_tag
{
    char name[20];
    char id[10];
};

struct course_tag
{
    char course_name[20];
    int no_of_units;
    int marks[MAX_NO_OF_UNITS];
    float avg;
};

struct student_tag
{
    struct person_tag student_info;
    struct course_tag course_info;
    struct student_tag *next;
} * head;

// typedef for declared to use as data type
typedef struct person_tag PERSON;
typedef struct course_tag COURSE;
typedef struct student_tag STUDENT;

// util functions
void display_menu();
void print_welcome();
void print_student(STUDENT *student);
void release(STUDENT *data);
STUDENT *search_student_by_name_or_id(char search_key[NAME_SIZE]);

// core functions
void display_students();
void search_student();
void find_maximum();
void find_failed();
void update_file();
void read_file();
void quite();

// Linked list functions
void add_student(char student_name[20], char student_id[10], char course_name[20], int no_of_units, int marks[MAX_NO_OF_UNITS]);
int count();

int main(void)
{
    // variable use for menu selected
    int selected;

    print_welcome();

    // init the read data from the file
    read_file();

    while (1)
    {
        display_menu();
        printf("Enter your option: ");
        scanf("%d", &selected);

        switch (selected)
        {
        case 1:
            display_students();
            break;
        case 2:
            search_student();
            break;
        case 3:
            find_maximum();
            break;
        case 4:
            find_failed();
            break;
        case 5:
            update_file();
            break;
        case 6:
            quite();
            break;
        default:
            printf("cannot find your option!");
            break;
        }
    }
}

void print_welcome()
{
    printf("Welcome!\n");
}

void display_students()
{
    read_file();

    STUDENT *student = head;

    if (student == NULL)
    {
        printf("No student!");
        return;
    }

    printf("\n================ Student Details ================\n");
    print_student(student);
}

void print_student(STUDENT *student)
{
    while (student != NULL)
    {
        printf("\n");
        printf("Student Name: %s\n", student->student_info.name);
        printf("Student ID: %s\n", student->student_info.id);
        printf("Course Name: %s\n", student->course_info.course_name);
        printf("No of units: %d\n", student->course_info.no_of_units);

        printf("Marks: [ ");
        for (int i = 0; i < student->course_info.no_of_units; i++)
        {
            printf("%d ", student->course_info.marks[i]);
        }
        printf("]\n");
        printf("Avg: %.2f\n", student->course_info.avg);
        student = student->next;
    }

    // clean up the memory
    // after output the data
    // because we don;t need it anymore after output
    release(student);
}

void display_menu()
{
    printf("\n(1) Display students’ details\n");
    printf("(2) Search for a student’s mark\n");
    printf("(3) Find the details of student with the largest average\n");
    printf("(4) Find the details of failed students\n");
    printf("(5) Add new student to the record\n");
    printf("(6) Quit program\n\n");
}

void add_student(char student_name[20], char student_id[10], char course_name[20], int no_of_units, int marks[MAX_NO_OF_UNITS])
{
    STUDENT *temp, *iterator;
    temp = (struct student_tag *)malloc(sizeof(struct student_tag));
    PERSON info;
    memcpy(info.name, student_name, 20);
    memcpy(info.id, student_id, 10);

    COURSE course;
    memcpy(course.course_name, course_name, 20);
    course.no_of_units = no_of_units;
    // memcpy(course.marks, marks);

    float sum = 0;
    for (int i = 0; i < no_of_units; i++)
    {
        course.marks[i] = marks[i];
        sum += marks[i];
    }
    course.avg = sum / no_of_units;
    temp->student_info = info;
    temp->course_info = course;

    // reference in head
    iterator = head;

    if (head == NULL)
    {
        head = temp;
        head->next = NULL;
    }
    else
    {
        while (iterator->next != NULL)
        {
            iterator = iterator->next;
        }
        temp->next = NULL;
        iterator->next = temp;
    }
}

int count()
{
    int n = 1;
    STUDENT *temp;
    temp = head;
    if (head == NULL)
    {
        return 0;
    }

    while (temp->next != NULL)
    {
        n++;
        temp = temp->next;
    }

    return n;
}

void search_student()
{
    char search_key[NAME_SIZE];

    printf("Enter student name or id to search: ");
    scanf("%s", &search_key);

    STUDENT *found_student = search_student_by_name_or_id(search_key);
    if (found_student == NULL)
    {
        printf("No student found!");
        return;
    }

    print_student(found_student);
}

STUDENT *search_student_by_name_or_id(char search_key[NAME_SIZE])
{
    STUDENT *temp = head;

    while (temp != NULL)
    {
        if (temp->student_info.name == search_key || temp->student_info.id == search_key)
        {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}

void find_maximum()
{
    printf("\nNot implement yet!");
}

void find_failed()
{
    printf("\nNot implement yet!");
}

void update_file()
{
    FILE *file;
    file = fopen(FILE_STUDENT_DATA_PATH, "a");

    char name[20];
    char id[10];
    char course_name[20];
    int no_of_units;
    int marks[MAX_NO_OF_UNITS];

    printf("Enter student name: ");
    scanf("%s", &name);

    printf("Enter student id: ");
    scanf("%s", &id);

    printf("Enter course name: ");
    scanf("%s", &course_name);

again:
    printf("Enter no of units: ");
    scanf("%d", &no_of_units);

    if (no_of_units > MAX_NO_OF_UNITS && no_of_units <= 0)
    {
        printf("you cannot input the units bigger than %d or less than 0\n", MAX_NO_OF_UNITS);
        getchar();
        goto again;
    }

    for (int i = 0; i < no_of_units; i++)
    {
        printf("Enter mark[%d]: ", i + 1);
        scanf("%d", &marks[i]);
    }

    if (count() > 0)
    {
        fputs("\n", file);
    }

    fputs(name, file);
    fputs("\n", file);
    fputs(id, file);
    fputs("\n", file);
    fputs(course_name, file);
    fputs("\n", file);
    fprintf(file, "%d", no_of_units);

    for (int i = 0; i < no_of_units; i++)
    {
        fputs("\n", file);
        fprintf(file, "%d", marks[i]);
    }

    fclose(file);

    printf("\nSaved");

    // reload data into linked list again
    read_file();
}

void read_file()
{
    // release nodes
    // we need to clean up the memory
    if (head != NULL)
    {
        STUDENT *temp;
        while (head != NULL)
        {
            temp = head;
            head = head->next;
            free(temp);
        }
    }

    FILE *file;
    file = fopen(FILE_STUDENT_DATA_PATH, "r");

    if (file == NULL)
    {
        printf("cannot read file: %s", FILE_STUDENT_DATA_PATH);
        exit(EXIT_FAILURE);
    }

    char student_name[20];
    char student_id[10];
    char course_name[20];
    int no_of_units;
    int marks[4];

    int i;
    while (!feof(file))
    {
        char no[BUFFER_SIZE];

        fgets(student_name, sizeof student_name, file);
        fgets(student_id, sizeof student_id, file);
        fgets(course_name, sizeof course_name, file);
        fgets(no, sizeof no, file);

        i = 0;
        while (student_name[i] != '\n')
        {
            i++;
        }

        student_name[i] = '\0';

        i = 0;
        while (student_id[i] != '\n')
        {
            i++;
        }

        student_id[i] = '\0';

        i = 0;
        while (course_name[i] != '\n')
        {
            i++;
        }

        course_name[i] = '\0';

        no_of_units = atoi(no);
        for (int j = 0; j < no_of_units; j++)
        {
            char mark[BUFFER_SIZE];

            fgets(mark, sizeof mark, file);
            sscanf(mark, "%d", &marks[j]);
        }

        // add into linked list
        add_student(student_name, student_id, course_name, no_of_units, marks);
    }

    fclose(file);
}

void quite()
{
    printf("\nGoodbye!");
    exit(EXIT_SUCCESS);
}

void release(STUDENT *data)
{
    if (data == NULL)
    {
        return;
    }

    // free the nodes
    // because it can be use in memory
    // we need to clear it first
    // before we re-initailize the new data
    STUDENT *temp;
    while (data != NULL)
    {
        temp = data;
        data = data->next;
        free(temp);
    }
}
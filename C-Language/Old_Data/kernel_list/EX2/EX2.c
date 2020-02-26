#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LENGTH 100

typedef struct student {
    char first_name[MAX_LENGTH];
    char last_name[MAX_LENGTH];
    unsigned int age;
    struct list_head node_student;
} student_t;

student_t *make_student (const char *fn, const char *ln, unsigned int a) {
    student_t *stu = NULL;

    if ((stu = calloc(1, sizeof(struct student))) == NULL) {
        return NULL;
    }

    if (strlen(fn) > (MAX_LENGTH - 1)) {
        return NULL;
    }

    if (strlen(ln) > (MAX_LENGTH - 1)) {
        return NULL;
    }

    strcpy(stu->first_name, fn);
    strcpy(stu->last_name, ln);
    stu->age = a;

    return stu;
}


int main() {
    // Method 1
    struct list_head class;
    INIT_LIST_HEAD(&class);

    //Method 2
//    LIST_HEAD(class);

    student_t *stu = NULL;

    //////////////////create student
    if ((stu = make_student("ahao","mu",22)) == NULL) {
        fprintf(stderr,"Failed to create muahao\n");
        return -1;
    }

    list_add_tail(&stu->node_student, &class);

    if ((stu = make_student("lianjie", "li", 23)) == NULL) {
        fprintf(stderr,"Failed to create li tom\n");
        return -1;
    }

    list_add_tail(&stu->node_student, &class);

    if ((stu = make_student("xiaolong", "li", 12)) == NULL) {
        fprintf(stderr, "Failed to create lixuehua\n");
        return -1;
    }

    list_add_tail(&stu->node_student, &class);

    ///////////// Print all students

    printf("------print all students-----\n");

    list_for_each_entry(stu, &class, node_student) {
        printf("First name:%s\n", stu->first_name);
        printf("Last name:%s\n", stu->last_name);
        printf("Age:%d\n", stu->age);
    }


    //////////////// list bus

    LIST_HEAD(bus);
    printf("Moving all students to bus;\n");
    list_splice_init(&class, &bus);
    if (list_empty(&class)) {
        printf("No students in class\n");
    }

    printf("Print all bus students\n");
    list_for_each_entry(stu, &bus, node_student) {
        printf("%s %s %d", stu->first_name, stu->last_name, stu->age);
    }

    student_t *tmp = NULL;

    printf("muahao do not want go\n");
    list_for_each_entry_safe(stu, tmp, &bus, node_student) {
        if (strcmp(stu->first_name, "mu") == 0) {
            list_del(&stu->node_student);
            free(stu);
        }
    }

    printf("After muahao leave,students are: \n");
    list_for_each_entry(stu, &bus, node_student) {
        printf("%s %s %d\n",stu->first_name, stu->last_name, stu->age);
    }

    // end begin clean  all students;

    list_for_each_entry_safe(stu, tmp, &bus, node_student) {
        printf("Removing students: %s %s \n", stu->first_name, stu->last_name);
        list_del(&stu->node_student);
        free(stu);
    }

    return 0;
}

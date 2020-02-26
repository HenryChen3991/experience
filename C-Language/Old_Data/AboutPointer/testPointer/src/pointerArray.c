#include <stdio.h>
#include <stdlib.h>

int main() {
    int i=0;
    char *str[] = {"professor", "Justin", 
                      "teacher", "Momor", 
                      "student", "Caterpillar"}; 

    for(i = 0; i < 3; i++) {
	printf("%s\n",(char *)str[i]);
    }

    char **s = &str[0];

    printf("%s\n",(char *)*s);
    s++;
    printf("%s\n",(char *)*s);


    char *str2[] = {"professor2", "Justin2",
                      "teacher2", "Momor",
                      "student", "Caterpillar"};

    printf("---\n");
    printf("%s\n",(char *)str2[0]);
    printf("%s\n",(char *)str2[1]);

    return 0; 
}

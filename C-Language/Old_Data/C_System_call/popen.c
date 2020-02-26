#include <stdio.h>
#include <stdlib.h>

//FILE *popen(const char *command, const char *mode);
//int pclose(FILE *stream);

int main(void)
{
    FILE *cmd;
    char result[1024];

    cmd = popen("ps ax | grep \"henry\" | grep -v grep", "r");
    if (cmd == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }
    while (fgets(result, sizeof(result), cmd)) {
	printf("aa\n");
        printf("%s", result);
    }
    pclose(cmd);
    return 0;
}

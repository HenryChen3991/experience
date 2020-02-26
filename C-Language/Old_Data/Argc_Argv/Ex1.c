#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  int i=0;
  printf("We have %d arguments:\n", argc);
  for (i = 0; i < argc; ++i) {
    printf("[%d] %s\n", i, argv[i]);
    printf("argv[%d]'s len = %d\n",i,strlen(argv[i]));
    printf("argv[%d][0] = %c\n",i,argv[i][0]);
  }
  return 0;
}

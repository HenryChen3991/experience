#include <stdio.h>
#include <stdlib.h>

/* define simple structure */
struct {
   unsigned int widthValidated;
   unsigned int heightValidated;
} status1;
 
/* define a structure with bit fields */
struct {
   unsigned int widthValidated : 1;
   unsigned int heightValidated : 1;
} status2;
void main(void)
{
	printf( "Memory size occupied by status1 : %d\n", sizeof(status1));
        printf( "Memory size occupied by status2 : %d\n", sizeof(status2));
	return ;
}

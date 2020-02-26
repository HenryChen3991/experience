#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
	char cert_path[][]={ {"/data/X509/cert"},{"/data/X509/rsa"},{"/data/X509/host"}};
	int i=0;
	for(i=0;i<3;i++)
		printf("%s\n",cert_path[i]);
}

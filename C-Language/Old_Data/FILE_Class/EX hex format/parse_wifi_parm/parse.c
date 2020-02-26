#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct entry_struct {
    unsigned short offset;
    unsigned short value;
} Entry_struct;

typedef struct adapter_struct {
    unsigned char id[16];            /*format pci/0/1 */
    unsigned short entry_num;
    struct entry_struct entry[600];
} Adapter_struct;

typedef struct params_struct {
    unsigned short adapter_num;
    struct adapter_struct adapter[4];
} Params_struct;
#define NVRAM_WLAN_TAG "WLANDATA"
#define PCI2G "pci/1/1/0/"
#define PCI5G "pci/0/1/0/"

int mem_dbg = 1;
#define mem_printf(args...) { if(mem_dbg) printf(args); }

int main(int argc,char *argv[]){
	FILE *fp;
	char name[32], *data_array,nvram_array[1024];
	unsigned int offset, value;
	char line[64], *ptr;
	int cnt=0,al=0;

	struct params_struct params;

	struct adapter_struct *adapter_ptr=NULL;
	struct entry_struct *entry_ptr = NULL;
	int i=0, j=0;

	int size =0;
	unsigned short *data_ptr;

	int show_feature=0;
	
        memset(&params, 0, sizeof(params) );

	printf("argv[1] = %s\n",argv[1]);
	fp=fopen(argv[1],"r");
	if ( fp == NULL ) {
            printf("File %s could not be read\n", argv[1]);
            return -1;
        }
	while( !feof(fp) ) {
            cnt = fscanf(fp, "%s\n", line );

            if ( cnt != 1 ) /*Skip empty line*/
                continue;

            //printf("line=%s\n", line );
            ptr= strchr(line, '=');

            /*Comment line*/
            if ( ptr == NULL )
                continue;

            *ptr = '\0';

            strncpy( name, line, sizeof(name) );

            //printf("[%s]=", name);

            /*Start a new Adapter parameters*/
            if ( !strcmp(name, "ID") ) {
                adapter_ptr= (params.adapter + params.adapter_num );

                strcpy((char *)(adapter_ptr->id), (ptr+1) );
                //printf("[%s]\n", ptr+1);
                adapter_ptr->entry_num =0;

                entry_ptr = adapter_ptr->entry;
                params.adapter_num++;
                if ( params.adapter_num >=4) {
                    printf("Too many Adapters in the config file\n");
                    fclose(fp);
                    return 1;
                }
                continue;
            }

            if ( params.adapter_num == 0 ) {
                printf("The fist line should be ID=xxx to specify the adapter ID\n");
                fclose(fp);
                return -1;
            }

            /*Fetch a new entry and save it*/
            sscanf( name, "%d", &offset );
            sscanf( (ptr+1), "%x", &value );

            //printf("[%x]\n", value );

            entry_ptr->offset = (unsigned short)(offset&0xffff) ;
            entry_ptr->value = (unsigned short)(value&0xffff) ;
            adapter_ptr->entry_num++;
            if ( adapter_ptr->entry_num >= 600) {
                printf("Too many Entry in the config file\n");
                fclose(fp);
                return 1;
            }
            entry_ptr++;

        }

        fclose(fp);

        /*Transfer into a memory area, in order to save to flash*/
        ptr = malloc( sizeof(params) );
        size = 0;
        if ( ptr == NULL ) {
            printf("Memory Allocation Failure\n");
            return 1;
        }

        memset( ptr, 0, sizeof(params) );

	mem_printf("ptr = 0x%p\n",ptr);

        strncpy(ptr, NVRAM_WLAN_TAG, strlen(NVRAM_WLAN_TAG) );

        data_ptr = (unsigned short *)( ptr + strlen(NVRAM_WLAN_TAG) ) ;

	mem_printf("data_ptr = 0x%p\n",data_ptr);
        *data_ptr = params.adapter_num;
        data_ptr++;

	mem_printf("after *data_ptr = params.adapter_num ,and data_ptr++ , then data_ptr = 0x%p\n",data_ptr);

	for ( i=0; i< params.adapter_num; i++ ) {
            printf("\n\nadapter[%d] parames\n", i);
            printf("ID=%s\n", params.adapter[i].id );

	    if( !strncmp(PCI2G,params.adapter[i].id,strlen(PCI2G)) ){
		printf("is %s\n",PCI2G);
	    }
	    else if ( !strncmp(PCI5G,params.adapter[i].id,strlen(PCI5G)) ){
		printf("is %s\n",PCI5G);
	    }

            strcpy( (char *)data_ptr, (char *)(params.adapter[i].id) );
            data_ptr += 8;

            printf("entry_num=%d\n", params.adapter[i].entry_num);
            *data_ptr = params.adapter[i].entry_num;
            data_ptr++;

            for ( j=0; j<params.adapter[i].entry_num; j++ ) {
                printf("offset=%d value =0x%x\n",
                         params.adapter[i].entry[j].offset,
                         params.adapter[i].entry[j].value);
                if( !strncmp(PCI2G,params.adapter[i].id,strlen(PCI2G)) ){
                    printf("is %s\n",PCI2G);
                }
		char byteLen[2] = {0};
		byteLen[0] = (params.adapter[i].entry[j].value & 0xff);
		byteLen[1] = (params.adapter[i].entry[j].value >> 8) & 0xff;
	//	printf("0 : 0x%2x\n",byteLen[0]);
	//	printf("1 : 0x%2x\n",byteLen[1]);

                *data_ptr = params.adapter[i].entry[j].offset;
                data_ptr++;
                *data_ptr = params.adapter[i].entry[j].value;
                data_ptr++;
            }
        }

}

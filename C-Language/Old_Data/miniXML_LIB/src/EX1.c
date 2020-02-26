#include <stdio.h>
#include "mxml.h"
 
int main(void)
{
    FILE* fp = fopen("prac.xml","r");
    //从prac.xml文件中加载xml
    mxml_node_t *xml = mxmlLoadFile(NULL,fp,MXML_NO_CALLBACK);
    //定义两个空节点
    mxml_node_t *book = NULL;
    mxml_node_t *title = NULL;
    //从xml开始向下查找 name=book attrr=category
    book = mxmlFindElement(xml,xml,"book","category",NULL,MXML_DESCEND);
    
    while(book)
    {
        //获取title子元素的文本 book元素的属性
        title = mxmlFindElement(book,xml,"title",NULL,NULL,MXML_DESCEND);
        if(title == NULL)
        {
            printf("title not found\n");
            continue;
        }
        else
        {
            printf("book'titele is %s\n",mxmlGetText(title,NULL));
            printf("book'category:%s\n",mxmlElementGetAttr(book,"category"));
            book = mxmlFindElement(title,xml,"book","category",NULL,MXML_DESCEND);
        }
    }
    mxmlDelete(xml);
    fclose(fp);
    return 0;
}


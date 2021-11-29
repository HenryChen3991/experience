#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/list.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("XIYOU");
#define N 10
struct numlist{

        int num;
        struct list_head list;
};
struct numlist numhead;
static int __init doublelist_init(void)
{
    struct numlist *listnode;
    struct list_head *pos;
    struct numlist *p;
    int i;
    printk("doublelist is starting..\n");
    INIT_LIST_HEAD(&numhead.list);
    for(i=0;i<N;i++)
    {
        listnode=(struct numlist *)kmalloc(sizeof(struct numlist),GFP_KERNEL);
        listnode->num=i+1;
        list_add_tail(&listnode->list,&numhead.list);
        printk("node %d has added to the doublelist\n",i+1);
    }
    //遍歷鏈表
    i=1;
    list_for_each(pos,&numhead.list){
        p=list_entry(pos,struct numlist,list);
        printk("node %d's data :%d\n",i,p->num);
        i++;
    }
    return 0;
}
static void __exit doublelist_exit(void)
{
    struct list_head *pos,*n;
    struct numlist *p;
    //依次刪除N個節點
    int i;
    list_for_each_safe(pos,n,&numhead.list)
    {
        list_del(pos);
        p=list_entry(pos,struct numlist,list);
        kfree(p);
        printk("node %d has removed from the doublelist\n",i++);
    }
    printk("doublelist is exiting\n");

}
module_init(doublelist_init);
module_exit(doublelist_exit);

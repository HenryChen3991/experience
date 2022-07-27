// ©.
// https://github.com/sizet/lkm_linklist

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>




#define FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DMSG(msg_fmt, msg_args...) \
    printk(KERN_INFO "%s(%04u): " msg_fmt "\n", FILE_NAME, __LINE__, ##msg_args)




// 要記錄的資料, 記錄每個產品的名稱和價格.
struct product_t
{
    // 串列節點.
    struct list_head lnode;
    // 產品名稱.
    char name[16];
    // 產品價格.
    unsigned int price;
};

// 鍊結串列頭.
struct list_head product_list;




#define PARAMETER_DATA_SPLIT_KEY  ' '
#define PARAMETER_VALUE_SPLIT_KEY '='

struct parameter_record_t
{
    char *data_name;
    char *data_value;
    unsigned int is_must;
};

enum PARA_RECORD_INDEX_LIST
{
    PR_OPERATE_INDEX = 0,
    PR_NAME_INDEX,
    PR_PRICE_INDEX,
};
struct parameter_record_t para_record_list[] =
{
    {"operate", NULL, 1},
    {"name",    NULL, 0},
    {"price",   NULL, 0},
    {NULL, NULL, 0}
};

static ssize_t node_read(
    struct file *file,
    char __user *buffer,
    size_t count,
    loff_t *pos);

static ssize_t node_write(
    struct file *file,
    const char __user *buffer,
    size_t count,
    loff_t *pos);

static char *node_name = "linklist";
static struct proc_dir_entry *node_entry;
static struct file_operations node_fops =
{
    .read  = node_read,
    .write = node_write,
};




// 找到資料所在的位置.
static int linklist_search_product_by_name(
    char *product_name,
    struct product_t **product_data_buf)
{
    struct product_t *each_product;


    // 比對串列上的節點.
    list_for_each_entry(each_product, &product_list, lnode)
    {
        if(strcmp(product_name, each_product->name) == 0)
            break;
    }

    *product_data_buf = each_product;

    // 完整走訪完鍊結串列後, 串列節點最後會指向串列的頭而不是 NULL.
    return &(each_product->lnode) == &product_list ? -1 : 0;
}

// 將資料加入鍊結串列.
static int linklist_add(
    struct product_t *product_data)
{
    struct product_t *each_product;


    // 檢查此資料是否已經存在.
    if(linklist_search_product_by_name(product_data->name, &each_product) == 0)
    {
        DMSG("product already exist [%s/%u]", each_product->name, each_product->price);
        return -1;
    }

    // 取得空間.
    each_product = (struct product_t *) kmalloc(sizeof(struct product_t), GFP_KERNEL);
    if(each_product == NULL)
    {
        DMSG("call kmalloc() fail");
        return -1;
    }

    // 複製資料.
    memcpy(each_product, product_data, sizeof(struct product_t));

    // 加到鍊結串列的尾端.
    list_add_tail(&(each_product->lnode), &product_list);
    DMSG("add [%s/%u]", each_product->name, each_product->price);

    return 0;
}

// 將資料從鍊結串列刪除.
static int linklist_del(
    char *product_name)
{
    struct product_t *each_product;


    // 檢查資料是否存在.
    if(linklist_search_product_by_name(product_name, &each_product) < 0)
    {
        DMSG("not find product [%s]", product_name);
        return -1;
    }

    // 從鍊結串列刪除.
    DMSG("del [%s/%u]", each_product->name, each_product->price);
    list_del(&(each_product->lnode));

    // 釋放.
    kfree(each_product);

    return 0;
}

// 透過產品名稱從鍊結串列取出產品資料.
static int linklist_get(
    char *product_name)
{
    struct product_t *each_product;


    // 檢查資料是否存在.
    if(linklist_search_product_by_name(product_name, &each_product) < 0)
    {
        DMSG("not find product [%s]", each_product->name);
        return -1;
    }

    DMSG("product [%s/%u]", each_product->name, each_product->price);

    return 0;
}

// 顯示鍊結串列的所有資料.
static int linklist_dump(
    void)
{
    struct product_t *each_product;


    list_for_each_entry(each_product, &product_list, lnode)
    {
        DMSG("product [%s/%u]", each_product->name, each_product->price);
    }

    return 0;
}

// 刪除鍊結串列所有的資料.
static int linklist_clear(
    void)
{
    struct product_t *each_product, *tmp_product;


    list_for_each_entry_safe(each_product, tmp_product, &product_list, lnode)
    {
        DMSG("del [%s/%u]", each_product->name, each_product->price);
        list_del(&(each_product->lnode));
        kfree(each_product);
    }

    return 0;
}

static int split_parameter(
    char **para_con_buf,
    size_t *para_len_buf,
    char **data_name_buf,
    char **data_value_buf)
{
    char *pcon;
    size_t plen, idx1, idx2, more_para = 0;


    pcon = *para_con_buf;
    plen = *para_len_buf;

    for(idx1 = 0; idx1 < plen; idx1++)
        if(pcon[idx1] != PARAMETER_DATA_SPLIT_KEY)
            break;
    if(idx1 > 0)
    {
        pcon += idx1;
        plen -= idx1;
    }

    if(plen == 0)
        return 0;

    for(idx1 = 0; idx1 < plen; idx1++)
        if(pcon[idx1] == PARAMETER_DATA_SPLIT_KEY)
        {
            pcon[idx1] = '\0';
            more_para = 1;
            break;
        }

    for(idx2 = 0; idx2 < idx1; idx2++)
        if(pcon[idx2] == PARAMETER_VALUE_SPLIT_KEY)
        {
            pcon[idx2] = '\0';
            break;
        }

    *data_name_buf = pcon;

    *data_value_buf = idx2 < idx1 ? pcon + idx2 + 1 : NULL;

    idx1 += more_para;
    *para_con_buf = pcon + idx1;
    *para_len_buf = plen - idx1;

    return 1;
}

static int parse_parameter(
    char *para_con,
    size_t para_len,
    struct parameter_record_t *target_list)
{
    struct parameter_record_t *each_pr;
    char *tmp_name, *tmp_value;


    for(each_pr = target_list; each_pr->data_name != NULL; each_pr++)
        each_pr->data_value = NULL;

    while(1)
    {
        if(split_parameter(&para_con, &para_len, &tmp_name, &tmp_value) == 0)
            break;

        for(each_pr = target_list; each_pr->data_name != NULL; each_pr++)
            if(strcmp(each_pr->data_name, tmp_name) == 0)
            {
                if(tmp_value == NULL)
                {
                    DMSG("miss value [%s]", each_pr->data_name);
                    return -1;
                }

                if(each_pr->data_value != NULL)
                {
                    DMSG("duplic data [%s]", each_pr->data_name);
                    return -1;
                }

                each_pr->data_value = tmp_value;
                break;
            }

        if(each_pr->data_name == NULL)
        {
            DMSG("unknown parameter [%s]", tmp_name);
            return -1;
        }
    }

    for(each_pr = target_list; each_pr->data_name != NULL; each_pr++)
        if(each_pr->data_value == NULL)
            if(each_pr->is_must != 0)
            {
                DMSG("miss data [%s]", each_pr->data_name);
                return -1;
            }

    return 0;
}

static int process_parameter(
    char *para_con,
    size_t para_len)
{
    struct parameter_record_t *pr_name = NULL, *pr_price = NULL, *pr_operate;
    struct product_t product_data;


    if(parse_parameter(para_con, para_len, para_record_list) < 0)
    {
        DMSG("call parse_parameter() fail");
        return -1;
    }

    memset(&product_data, 0, sizeof(product_data));

    pr_name = para_record_list + PR_NAME_INDEX;
    if(pr_name->data_value != NULL)
    {
        snprintf(product_data.name, sizeof(product_data.name), "%s", pr_name->data_value);
        DMSG("name  = %s", product_data.name);
    }

    pr_price = para_record_list + PR_PRICE_INDEX;
    if(pr_price->data_value != NULL)
    {
        product_data.price = simple_strtoul(pr_price->data_value, NULL, 10);
        DMSG("price = %u", product_data.price);
    }

    pr_operate =   para_record_list + PR_OPERATE_INDEX;
    if(strcmp(pr_operate->data_value, "add") == 0)
    {
        if(pr_name->data_value == NULL)
        {
            DMSG("name can not be empty");
            return -1;
        }
        if(strlen(product_data.name) == 0)
        {
            DMSG("name can not be empty");
            return -1;
        }
        if(pr_price->data_value == NULL)
        {
            DMSG("price can not be empty");
            return -1;
        }

        if(linklist_add(&product_data) < 0)
        {
            DMSG("call linklist_add() fail");
            return -1;
        }
    }
    else
    if(strcmp(pr_operate->data_value, "del") == 0)
    {
        if(pr_name->data_value == NULL)
        {
            DMSG("name can not be empty");
            return -1;
        }
        if(strlen(product_data.name) == 0)
        {
            DMSG("name can not be empty");
            return -1;
        }

        if(linklist_del(product_data.name) < 0)
        {
            DMSG("call linklist_del() fail");
            return -1;
        }
    }
    else
    if(strcmp(pr_operate->data_value, "get") == 0)
    {
        if(pr_name->data_value == NULL)
        {
            DMSG("name can not be empty");
            return -1;
        }
        if(strlen(product_data.name) == 0)
        {
            DMSG("name can not be empty");
            return -1;
        }

        if(linklist_get(product_data.name) < 0)
        {
            DMSG("call linklist_get() fail");
            return -1;
        }
    }
    else
    if(strcmp(pr_operate->data_value, "dump") == 0)
    {
        if(linklist_dump() < 0)
        {
            DMSG("call linklist_dump() fail");
            return -1;
        }
    }
    else
    if(strcmp(pr_operate->data_value, "clear") == 0)
    {
        if(linklist_clear() < 0)
        {
            DMSG("call linklist_clear() fail");
            return -1;
        }
    }

    return 0;
}

static ssize_t node_read(
    struct file *file,
    char __user *buffer,
    size_t count,
    loff_t *pos)
{
    // 使用方式 : echo "command" > /proc/linklist
    DMSG("usage :");
    DMSG("  echo \"<command>\" > /proc/%s", node_name);

    // 增加產品資料, 例如 :
    // echo "operate=add name=pen price=25" > /proc/linklist
    DMSG("add product :");
    DMSG("  operate=add name=<name> price=<price>");

    // 刪除某個產品資料, 例如 :
    // echo "operate=del name=pen" > /proc/linklist
    DMSG("del product :");
    DMSG("  operate=del name=<name>");

    // 取得某個產品的資料, 例如 :
    // echo "operate=get name=pen" > /proc/linklist
    DMSG("get product data :");
    DMSG("  operate=get name=<name>");

    // 顯示全部的產品資料, 例如 :
    // echo "operate=dump" > /proc/linklist
    DMSG("dump all product :");
    DMSG("  operate=dump");

    // 刪除全部的產品資料, 例如 :
    // echo "operate=clear" > /proc/linklist
    DMSG("del all product :");
    DMSG("  operate=clear");

    return 0;
}

static ssize_t node_write(
    struct file *file,
    const char __user *buffer,
    size_t count,
    loff_t *pos)
{
    char read_buf[256];
    size_t rlen = sizeof(read_buf) - 1;


    memset(read_buf, 0, sizeof(read_buf));
    rlen = count >= rlen ? rlen : count;
    copy_from_user(read_buf, buffer, rlen);
    if(rlen > 0)
        if(read_buf[rlen - 1] == '\n')
        {
            rlen--;
            read_buf[rlen] = '\0';
        }

    if(process_parameter(read_buf, rlen) < 0)
    {
        DMSG("call process_parameter() fail");
    }

    return count;
}

static int __init main_init(
    void)
{
    // 初始化鍊結串列.
    INIT_LIST_HEAD(&product_list);

    if((node_entry = proc_create(node_name, S_IFREG | S_IRUGO | S_IWUGO, NULL, &node_fops)) == NULL)
    {
        DMSG("call proc_create(%s) fail", node_name);
        return 0;
    }

    return 0;
}

static void __exit main_exit(
    void)
{
    remove_proc_entry(node_name, NULL);

    linklist_clear();

    return;
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Che-Wei Hsu");
MODULE_DESCRIPTION("Link List");

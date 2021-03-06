#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define MAX_BUCKETS 1000     //根据作业要求，已将从100改成了1000
#define MULTIPLIER 31

#define LENGTH 30
typedef struct User_login {
    char name[LENGTH];
    int totalcount;
}ELementType;                 //链表的节点

typedef struct SListNode
{
    ELementType data;
    struct SListNode* next;
}Node, * PNode, * List;              //封装链表节点和next指针


static PNode table[MAX_BUCKETS];

static unsigned long hashstring(const char* str);
static void cleartable();
static PNode walloc(const char* str);
static PNode lookup(const char* str);
static PNode find(PNode wp, const char* str);

/*创建一个节点*/
static PNode walloc(const char* str)
{
    PNode p = (PNode)malloc(sizeof(Node));
    if (p != NULL) {
        strcpy(p->data.name, str);
        p->data.totalcount = 0;
        p->next = NULL;
    }
    return p;
}

/*计算哈希值*/
static unsigned long hashstring(const char* str)
{
    unsigned long hash = 0;                 //hash保存哈希值
    while (*str)                            //循环计算hash值        
    {
        hash = hash * MULTIPLIER + *str;    //
        str++;
    }
    hash = hash % MAX_BUCKETS;              //对MAX_BUCKETS求余得到最终hash值
    return hash;
}

/*在一个链表中查找人名，找到返回指针，找不到返回NULL*/
static PNode find(PNode wp, const char* str)
{
    PNode cur = wp;
    while (cur) {
        if (strcmp(cur->data.name, str) == 0)   
            return cur;                         //如果找到则返回节点
        cur = cur->next;                        //继续遍历下一个节点
    }
    return NULL;
}

/*将在散列表中查找相应节点，并进行相应操作，找到返回指针，没找到则创建节点并加入散列表,并返回指针*/
static PNode lookup(const char* str)
{
    unsigned long hash = hashstring(str);
    PNode cur = find(table[hash], str);             
    if (cur) {
        return cur;             //如果find()找到目标节点则直接返回该节点地址
    }
    else {
        cur = table[hash];
        if (cur == NULL) {
            table[hash] = walloc("/head");      //创建头节点
            table[hash]->next = walloc(str);    //头节点后新建数据节点
            return table[hash]->next;
        }
        while (cur->next) {
            cur = cur->next;
        }
        if (strcmp(cur->data.name, str) == 0)
            return cur;                         //如果最后一个节点匹配则返回该节点地址
        else
            return cur->next = walloc(str);     //如果没有节点匹配则新建数据节点并将其地址返回
    }
}

/*删除散列表*/
static void cleartable()
{
    PNode wp = NULL, p = NULL;
    int i = 0;

    for (i = 0; i < MAX_BUCKETS; i++) {
        wp = table[i];
        while (wp) {
            p = wp;
            wp = wp->next;
            free(p);
        }
    }
}

/*读取文件，创建散列表*/
void file_read_ht()
{
    FILE* fp = fopen("user_login.txt", "r");
    char word[1024];        //存储姓名
    char* name;             //结构体
    PNode wp = NULL;

    memset(table, 0, sizeof(table));

    int count = 0;
    while (1) {
        if (fscanf(fp, "%s", word) != 1)
            break;
        name = strtok(word, ",");
        ////begin
        //加入散列表，2条语句实现
        //wp = table[hashstring(name)];
        wp = lookup(name);          //临时节点wp接收lookup返回值
        wp->data.totalcount++;      //将wp指向的数据中的登录次数加一
        ////end

        count++;
    }
    printf("%d \n", count);
    fclose(fp);
}

/*将散列表写入文件*/
void file_write_ht()
{
    FILE* fp;
    int count = 0;

    if ((fp = fopen("output.txt", "wt")) == NULL) {
        printf("Fail to open file!\n");
        exit(0);
    }

    ////begin
    int i = 0;
    PNode cur;
    while (i < MAX_BUCKETS) {
        if (table[i] == NULL) {
            i++;
            continue;               //如果当前散列表元素指向空则跳过此次循环，i自增1
        }
        cur = table[i]->next;
        while (cur) {
            fprintf(fp, "%s, %d\n", cur->data.name, cur->data.totalcount);      //将节点数据写入output.txt
            count++;                //统计元素count自增1
            cur = cur->next;
        }
        i++;
    }
    ////end
    printf("%d\n", count);
}

/*搜索功能*/
void search_ht()
{
    char name[LENGTH];
    printf("Enter name, 'q' to exit: \n");
    scanf("%s", name);

    while (strcmp(name, "q")) {
        unsigned long hash = hashstring(name);
        PNode wp = table[hash];
        PNode curr = NULL;

        ////begin
        curr = find(wp, name);
        if (curr) {
            printf("name:%s\ntotalcount = %d\n", name, curr->data.totalcount);      //如果找到该人名的记录则输出其登录信息
        }
        else {
            printf("Not found!\n");             ////如果没找到该人名的记录则提示没找到
        }
        ////end

        scanf("%s", name);
    }

    cleartable();
}





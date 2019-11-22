#include<stdio.h>     
#include<string.h>    
#include<stdlib.h>   
#define N 1000   
#define M 4   
       
int amount=0;               //全局变量，保存书的总量  

 /*************书的结构体***********/      
typedef struct              
{   
    int num;				//书号 
    char name[20];		    //书名
    char writer[20];        //书作者
    int left;               //图书现存量
    int storage;            //该图书总量
           
}DataType;   
       
/*************B-树的存储结构***********/      
typedef struct node         
{    
    struct node *parent;    //指向父结点的指针
    int keynum;             //结点关键值
    DataType key[M + 1];    //一个结点中最多存放的书的个数
    struct node *ptr[M + 1];    //指向孩子结点的指针
}node;   

node *root; //根节点指针 
       
/*************借阅者信息（用单链表方式存储）***********/
typedef struct { 
	char number[20];				//借阅者编号
	char limtdata[20];				//归还时间
}Reader;
     
   

/*************查找结果的存储结构体***********/      
typedef struct   
{    
    node *ptr;    //结点指针，指向查找结果的结点 
    int pos;      //具体位置标示
    int find;     //一类书专用编号，判断该书是否录入，进而判断是否找到
}result; 
   					
 
DataType b[N];//书的结构体数组   


/*************函数声明***********/      
void assist();   
result SearchBT(node *root, int key);   //搜索树，用于各函数中查找相应结点位置 
void Insert(node *root, int pos, node *child, DataType key);   // 直接插入结点
void Split(node *p, int *key, int pos, node **child);   //分裂结点
void output(node *root, int floor);   //以凹入表的形式输出树 
void assist(node *root);   //查询函数
void borrow(node *root);   //借书函数
void dele(node *root);   //消除库存  
void welcome();   //欢迎函数，方便回到选择界面
void input();   //采编入库
       
result SearchBT(node *root,int key)    //搜索树   
{    
    if (root == NULL)   
    {    
        printf("SearchBT Error...\n"); //树不存在   
        exit(0);    
    }    
           
    int pos = 0, find = 0;    
    node *just = NULL;           
    while (root && !find)              //如果没有找到，则循环查找   
    {    
        pos = 0;    
        while (pos<root->keynum && root->key[pos+1].num<=key)    
        {    
            pos++;    
        }    
        if (pos && root->key[pos].num == key)   
        {    
            find = 1;    
        }    
        else {    
            just = root;    
            root = root->ptr[pos];    
            }    
    }    
           
        if (find)                          //如果找到了，则输出结果   
        {    
            result val = { root, pos, 1 };    
            return val;    
        }           
        else    
        {    
            result val = { just, pos, 0 };    
            return val;    
        }    
}    

      
void Insert(node *root, int pos, node *child, DataType key) //直接插入结点   
{    
    int i;    
           
    for (i=root->keynum; i>=pos+1; i--) {   //一个结点一个结点的向后移
        root->key[i + 1] = root->key[i];    
        root->ptr[i + 1] = root->ptr[i];    
    }    
    root->key[pos + 1] = key;               //在空出来的位置插入结点
    root->ptr[pos + 1] = child;    
    root->keynum++;    
}    
       
       
void Split(node *p, DataType *key, int pos, node **child) //分裂结点        
{    
    DataType tempkey[M + 2];    
    node *temptr[M + 2];    
    int i, j;    
    i = j = 0;    
    for (i=0; i<=M+1; i++) {    
        if (i == pos + 1) {    
            tempkey[i] = *key;    
            temptr[i] = *child;    
        }    
        else {    
            tempkey[i] = p->key[j];    
            temptr[i] = p->ptr[j++];    
        }    
    }                                 //将分裂结点之前的结点安置好
                   
    *child = (node *)malloc(sizeof(node));    //为新的子结点分配空间

    (*child)->parent = p->parent;    
    int mid = (M + 1) / 2 + 1;    
    p->keynum = mid - 1;    
    (*child)->keynum = M - (mid - 1);    //计算各分支结点的书类数

    for (i=0; i<=M+1; i++) {    
        if (i < mid) {    
            p->key[i] = tempkey[i];    
            p->ptr[i] = temptr[i];    
        }    
        else if (i == mid) {    
            (*child)->ptr[0] = temptr[i];    
            *key = tempkey[i];    
        }    
        else {    
            (*child)->key[i - mid] = tempkey[i];    
            (*child)->ptr[i - mid] = temptr[i];    
        }    
    }    
}    
       
       
//插入结点的时候，先直接插入到叶结点，   
//再判断是否要分裂结点   
void InsertBT(node **T, DataType key, node *p, int pos)   
{    
    node *child = NULL;                      //建立新的结点指针，为后面的结点分裂做准备
    node *just;    
    while (p)    
    {    
        if (p->keynum < M)                   //结点p中的书的种类〈M
        {    
            Insert(p, pos, child, key);      //直接插入结点
            return;    
        }   
        else {                               //超过该结点的最大承受书类型数目
            Split(p, &key, pos, &child);     //产生新的key, child...处于位置pos的新结点信息指针key放入结点指针p中，再进行分裂，用child指向结点的指针指向分裂后的另一个结点
            just = p;    
            p = p->parent;                   //p指向他的父结点
            if (p)    
            {    
                pos = 0;    
                while (pos<p->keynum && p->key[pos+1].num<=key.num)    //符合一条树分支的限额设置，pos相应加1
                {    
                    pos++;    
                }    
            }    
        }    
    }    
    node *root = (node *)malloc(sizeof(node));    
    root->keynum = 1;    
    root->key[1] = key;    
    root->ptr[0] = just;    
    root->ptr[1] = child;    
    root->parent = NULL;    
           
    (*T)->parent = child->parent = root;    
    *T = root;    
}    
       
       
void SearchInsert2(node **root, DataType key)    
{    
    if (*root == NULL)    
    {    
        *root = (node *)malloc(sizeof(node));    
        (*root)->key[1] = key;    
        (*root)->keynum = 1;    
        (*root)->ptr[0] = (*root)->ptr[1] = NULL;    
        (*root)->parent = NULL;    
        return;    
    }    
           
    result val=SearchBT(*root,key.num);    
    InsertBT(root, key, val.ptr, val.pos);    
           
}    
       
       
void SearchInsert(node **root, DataType key)    //搜索并且插入结点，调用函数找到插入结点的位置   key-包括一本书的所有信息
{    
    if (*root == NULL)                          //root指针空时，直接添加信息
    {    
        *root = (node *)malloc(sizeof(node));    
        (*root)->key[1] = key;                  //一本书的信息放入树中
        (*root)->keynum = 1;                    //相应书的数目加1
        (*root)->ptr[0] = (*root)->ptr[1] = NULL;    //子结点放空 
        (*root)->parent = NULL;                      //父结点放空
        return;    
    }    
            
    result val=SearchBT(*root,key.num);         //当root指针非空时，搜索树中是否有相同指针
    if (val.find)    
    {    
        printf("already exist...\n");           //有相同指针，发出信息提示
    }    
    else    
    {    
        InsertBT(root, key, val.ptr, val.pos);   //否则插入结点
    }    
}    
       
       


void output(node *root, int floor)               //以凹入表的形式输出树   
{     
    int t=1;
    if (root == NULL) return;          
    output(root->ptr[0], floor + 1);    
    for (int i=1; i<=root->keynum; i++)    
    {    
        for (int j=0; j<5; j++)    
        {    
            if (j == floor)    
            {    
                printf("\t[%d]%3d", t++, root->key[i].num);    
            }    
            else if (j < floor)   
            {    
                printf("      ");    
            }    
            else    
            {    
                printf("------");    
            }    
        }    
        printf("\t%s  %s  %d\n", root->key[i].name,root->key[i].writer,root->key[i].left);    
        output(root->ptr[i], floor + 1);    
    }    
}   
       
       
void assist(node *root)//查询函数   
{   
    int num,i,m;  
    DataType b2;
	char  mingzi[20];
    result d;   
    char a[3];   
    do
	{
	    printf("================================================================================\n");    
        puts("\t\t*****************查询*********************\n\n");   
        puts("\t\t\t\t1.书号查询\n");   
        puts("\t\t\t\t2.书名查询\n");   
        puts("\t\t\t\t3.作者查询\n");   
        puts("================================================================================\n");   
        printf("请在1-3中选择以回车键结束：\n\n");   
		scanf("%d",&m);
		switch(m)   
        {   
        case 1: printf("请输入你要查询的书的书号：\n\n");   
                scanf("%d",&num);   
                d=SearchBT(root,num);   
                if(d.find&&d.ptr->key[d.pos].storage!=0)   
                {   
                    printf("书号\t书名\t作者\t剩余量\t总库存\n");   
                    printf("%d\t%s\t%s\t%d\t%d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);         
                }   
                else    
                printf("你要找的书不存在!\n\n");   
                printf("你还要继续查找吗?选择yes或者no.\n");   
                scanf("%s",a);  
                break;   
                
        case 2:printf("请输入你要查询的书的书名：\n\n");   
                scanf("%s",&b2.name);   
                for(i=0;i<amount;i++)
                {
                	if(strcmp(b2.name,b[i].name)==0)
                	num=b[i].num;
				}
                d=SearchBT(root,num);   
                if(d.find&&d.ptr->key[d.pos].storage!=0)   
                {   
                    printf("书号\t书名\t作者\t剩余量\t总库存\n");   
                    printf("%d\t%s\t%s\t%d\t%d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);         
                }   
                else    
                printf("你要找的书不存在!\n\n");   
                printf("你还要继续查找吗?选择yes或者no.\n");   
                scanf("%s",a);  
                break;    
                
		case 3:printf("请输入你要查询的作者名：\n\n");   
                scanf("%s",&b2.writer);  
			    printf("书号\t书名\t作者\t剩余量\t总库存\n");
                for(i=0;i<amount;i++)
                {
                	if(strcmp(b2.writer,b[i].writer)==0)
                	{
					    num=b[i].num;
                        d=SearchBT(root,num);   
                        if(d.find&&d.ptr->key[d.pos].storage!=0)   
                        {   
                            printf("%d\t%s\t%s\t%d\t%d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);         
                        } 
					} 
				} 
                if(i>amount)    
                printf("你要找的作者暂时没有书储存于该系统!\n\n");   
                
                printf("你还要继续查找吗?选择yes或者no.\n");   
                scanf("%s",a);  
                break;   
        
        }         
    } while(strcmp(a,"yes")==0);   
    welcome();   
}   
       
       
  void print(node *root)   
{   
           
    root=NULL;   
           
    for(int x=0;x<amount;x++)   
    {   
               
        SearchInsert(&root,b[x]);   
    }   
	//SearchInsert(&root, b[i].num); 
	printf("\t序列 书号------------------------书名   作者  剩余量 \n");    
    output(root, 0);    
	getchar();    
    printf("\n");    
}   
       
void borrow(node *root)//借书函数   
{   
    int num,count,i,id,date;   
    int tag;
	Reader a;
    result d;   
    if(root==NULL)   
    {   
        printf("书库里一本书都没有，您不能借书!\n");   
        welcome();   
    }  
    printf("请输入这本书的书号:");   
    scanf("%d",&num);   
    d=SearchBT(root,num);   
    if(d.find==0||d.ptr->key[d.pos].storage==0)   
    {   
        printf("没有您要借的书!\n");   
        welcome();   
    }   
    else   
    {   
        printf("书号 \t书名 \t作者 \t剩余量 \t总库存\n");   
        printf("%5d \t%5s \t%5s \t%5d \t%5d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);   
    }      
    if(d.ptr->key[d.pos].left==0)   
        printf("此书已经借完!\n");   
           
    else    
    {   
        printf("请输入您要借这本书的数量(小与等于%d)\n",d.ptr->key[d.pos].left);   
        scanf("%d",&count);   
        while(count>d.ptr->key[d.pos].left)    
        {   
            printf("输入有误，请重新输入\n");   
            scanf("%d",&count);   
        }   
        printf("借书成功\n");   
        d.ptr->key[d.pos].left=d.ptr->key[d.pos].left-count;   
        printf("请输入您的图书证号：");
		scanf("%s",&a.number);
		printf("请输入您的还书日期：");
		scanf("%s",&a.limtdata);
    }   
    welcome();    
}   
       
void returnback()//还书函数   
{   
    int num,count,i;   
    int tag;   
    result d;   
    printf("请输入这本书的书号:");   
    scanf("%d",&num);   
    d=SearchBT(root,num);   
    if(d.find==0||d.ptr->key[d.pos].storage==0)   
    {   
        printf("这本书不属于图书馆!\n");   
        welcome();   
    }   
    else   
    {   
        printf("书号     书名     作者      剩余量        总库存\n");   
        printf("%d%7s%7s%8d%12d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);   
    }      
           
    printf("请输入您要还书的数量:\n");   
    scanf("%d",&count);   
    while(count>d.ptr->key[d.pos].storage-d.ptr->key[d.pos].left)    
    {   
        printf("输入有误，请重新输入\n");   
        scanf("%d",&count);   
    }   
    printf("还书成功\n");   
    d.ptr->key[d.pos].left=d.ptr->key[d.pos].left+count;   
    welcome();    
}   
       
       
void welcome()//欢迎函数，方便回到选择界面   
{   
    int  j;   
	    printf("=================================图书管理系统===================================\n");   
    do   
    {   printf("=================================B16070404陈慧===================================\n"); 
        puts("\t\t*****************MENU*********************\n\n");   
        puts("\t\t\t\t1.采编入库\n");   
        puts("\t\t\t\t2.消除库存\n");   
        puts("\t\t\t\t3.借阅\n");   
        puts("\t\t\t\t4.归还\n");   
        puts("\t\t\t\t5.显示\n");   
        puts("\t\t\t\t6.查询\n");   
        puts("\t\t\t\t0.退出\n");   
        puts("================================================================================\n");   
        printf("请在0-6中选择以回车键结束：\n\n");   
        scanf("%d",&j);   
        switch(j)   
        {   
        case 1: input();   
            break;   
        case 2:dele(root);   
            break;   
        case 3:borrow(root);   
            break;   
        case 4:returnback();   
            break;   
        case 5:print(root);    
            break;   
        case 6:assist(root);     
        }   
    }while(j!=0);     //判断结束   
    printf("谢谢使用，再见！\n"); 
	exit(0);  
}//主函数结束   
       
void input()   //采编入库
{   
    int y;   
           
    printf("请依次输入图书信息:\n");        //打印提示信息   
    printf("--------------------------------------------------------------------------\n");   
    do{   
        for(int i=amount;i<N;i++)   
           {   
            printf("请输入图书编号:");     
            scanf("%d",&b[i].num);            //b[N]书的结构体数组
            printf("请输入图书名:");   
            scanf("%s",b[i].name);   
            printf("请输入作者:");   
            scanf("%s",b[i].writer);   
            printf("请输入总库存量:");   
            scanf("%d",&b[i].storage);   
            b[i].left=b[i].storage;           //目前剩余量=总量
            amount++;				          //储存总量+1 
            SearchInsert(&root,b[i]);         //搜索并且插入结点，调用函数找到插入结点的位置 
            printf("第%d个信息已经输完是否继续?按任意数字键继续，按 0结束\n",amount);   
            scanf("%d",&y);   
            break;   
        }   
    }while(y!=0);   
}   
       
void dele(node *root)//删除函数   
{   
    result d;   
    int num,n,tag;   
    char t[3];   
    do{   
        printf("确定要消除库存吗?：\n\n");//打印删除方式菜单   
        printf("1.确定\n\n");   
        printf("0.返回\n\n");   
        printf("请选择：\n");   
        scanf("%d",&n);   
        if(n==0)   
            welcome();       
        else   
        {   
            printf("请输入你要删除的书的书号");   
            scanf("%d",&num);   
            d=SearchBT(root,num);   
        }   
        if(!d.find)printf("你要找的书不存在!");   
        else   
        {   
            printf("书号     书名     作者      剩余量        总库存\n");   
            printf("%d%7s%7s%8d%12d\n\n",d.ptr->key[d.pos].num,d.ptr->key[d.pos].name,d.ptr->key[d.pos].writer,d.ptr->key[d.pos].left,d.ptr->key[d.pos].storage);   
            printf("确定要删除吗?请输入yes或者no.\n");   
            scanf("%s",t);   
            if(!strcmp(t,"yes"))   
            {   
                d.ptr->key[d.pos].storage=0;   
                for(int i=0;i<amount;i++)   
                {   
                    if(b[i].num==num)   
                        tag=i;   
				}   
                b[tag]=b[amount-1];   
                amount--;   
                for(int x=0;x<amount;x++)   
                {
				    SearchInsert2(&root,b[x]);   
                }            
            }   
        }   
        printf("是否继续删除\n");        
        printf("请键入yes或no以回车键结束\n");   
        scanf("%s",t);       //读取是否继续信息   
        if(!strcmp(t,"yes")) //判断是否继续删除       
            printf("请按任意键以回车键结束返回上层菜单:\n");   
        break;   
    }while(!strcmp(t,"yes"));   
}   
       
       
       
int main()   
{      
   
    node *root=NULL;   
    welcome();   
}   
       
       
       
       
       



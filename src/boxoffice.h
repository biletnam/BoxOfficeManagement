#include<stdio.h>
#include<stdlib.h>
#include<string.h>
FILE *fp;
typedef struct node
{
    char name[25];
    char address[50];
    char city[10];
    int tickets;
    int status_tic[4];
    int tic_id[4];
    int del;
    int id;
    struct node *next;
}NODE;
typedef struct hnode
{
    int max_tic;
    NODE *start;

}HNODE;
typedef struct q
{

    int arr[1000];
    int qfront;
    int qrear;
}Q;
void qinit(Q *q)
{
    q->qfront=0;
    q->qrear=-1;
}
void enqueue(Q *q,int id)
{
    q->arr[++(q->qrear)]=id;
}
int dequeue(Q *q)
{
    return q->arr[(q->qfront)++];
}
int emptyq(Q *q)
{
    return ((q->qrear)<(q->qfront));
}
int searchinq(Q *q,int id)
{
    int i;
    for(i=q->qfront;i<=q->qrear;i++)
    {
        if(q->arr[i]==id)
            break;
    }
    return (i<=q->qrear);
}
//Create node
NODE * createnode()
{
    NODE *a=(NODE *)malloc(sizeof(NODE));
    a->next=NULL;
    a->del=1;
    return a;
}
//copy content of structures variable to node from file
NODE * updaterecord(NODE *a)
{
    int i=0;
    NODE *c=createnode();
    strcpy(c->name,a->name);
   strcpy(c->city,a->city);
    strcpy(c->address,a->address);
    c->tickets=a->tickets;
    c->id=a->id;
    c->del=a->del;
    c->next=NULL;
    for(i=0;i<4;i++)
    {
        c->tic_id[i]=a->tic_id[i];
        c->status_tic[i]=a->status_tic[i];
    }
    return c;
}
//update Queue with deleted ticket id
void updateq(HNODE **p,Q *q)
{
    int i=0;
    NODE *b=(*p)->start;
    while(b!=NULL)
    {

    for(i=0;i<4&&b->tic_id[i]!=0;i++)
    {

        if(b->status_tic[i]==0&&(!searchinq(q,b->tic_id[i])))
        {
            if(checklist(p,q,b->tic_id[i]))
                enqueue(q,b->tic_id[i]);
        }

    }
    b=b->next;
    }

}
//Retrieve linked list from file
int updatelist(HNODE **root,char *str,Q *q)
{
    static int flg=-1;
	int cnt=1,res;
	char str1[100];
    NODE*a,*b;
	NODE node_var;
	fflush(stdin);
	*root=(HNODE *)malloc(sizeof(HNODE));
	 if(fp==NULL)
        {

                fp=fopen(str,"wb");
                fwrite(&flg,sizeof(int),1,fp);
                fclose(fp);
                fp=fopen(str,"rb+");

        }

	fread(&flg,sizeof(int),1,fp);
        if(flg==-1)
        {
            rewind(fp);
            while(1)
            {
                do
                {
                printf("\n\nENTER TOTAL AVILABLE TICKETS FOR MATCH : ");
                scanf("%s",str1);
                res=checkscan(str1,1);
                }while(res==-1);
                (*root)->max_tic=res;
                if((*root)->max_tic<=0)
                {
                    printf("\n\nREENTER MAX. AVAILABLE TICKETS : ");
                    continue;
                }
                else
                    break;
            }

            (*root)->start=NULL;
            flg=(*root)->max_tic;
            fwrite(&flg,sizeof(flg),1,fp);
            flg=0;
            return 1;
     	}
        fseek(fp,0,SEEK_SET);
        fread(&flg,sizeof(int),1,fp);

        (*root)->max_tic=flg;

        (*root)->start=NULL;

        while(1)
        {

            fread(&node_var,sizeof(NODE),1,fp);
            if(feof(fp))
                break;
            cnt+=node_var.tickets;
            b=updaterecord(&node_var);

            if((*root)->start==NULL)
                (*root)->start=b;
            else
            {
                a=(*root)->start;
                while(a->next!=NULL)
                    a=a->next;
                a->next=b;
            }
        }
        return cnt;

}

void qdisplay(Q *q)
{
    int i;
    for(i=q->qfront;i<=q->qrear;i++)
        printf("\n%4d",q->arr[i]);
}
//check whether deleted id is given later to another customer throughout list
int checklist(HNODE **p,Q *q,int id)
{
    int i,j;
    NODE *b;



                b=(*p)->start;
                while(b!=NULL)
                {

                    for(j=0;j<4;j++)
                    {
                        if(b->tic_id[j]==id)
                        {
                            if(b->status_tic[j]==1)
                            {
                                return 0;
                            }

                        }

                    }
                    b=b->next;
                }
                return 1;
}

//add new record to list and file
void addrecord(HNODE **p,int *ref_id,Q *q)
{
    int uid,res;
    char str[100];
    NODE *a,*b;
	NODE var;
    int tic,i=0,flg;
    fflush(stdin);
	if((*p)->max_tic==(*ref_id)-(q->qrear-q->qfront)-2)
	{
		printf("\nNO TICKETS AVAILABLE...!!!\n");
		return;
	}
        do
        {
            printf("\nEnter unique id : ");
            gets(str);
            res=checkscan(str,1);
        }while(res==-1);
        uid=res;
        if(searchid(p,uid)==1)
        {
            printf("\nTICKETS ALREADY BOOKED ON THIS DATA...!!!\n");
            return;
        }
    while(1)
    {
         do
        {
            printf("\nEnter no of tickets : ");
            gets(str);
            res=checkscan(str,1);
        }while(res==-1);
        tic=res;
        if(tic>4||tic<=0)
        {
            printf("\nInvalid request..!!\nYou can get tickets from 1 to 4 for championship...!!!\n");
            continue ;
        }
        else if((*ref_id+tic-(q->qrear-q->qfront)-1)>(((*p)->max_tic)+1))
        {
            printf("\nOnly %d tickets avilable...!!!\n",(((*p)->max_tic)-*ref_id)+2+(q->qrear-q->qfront));
            continue;
        }
        else
            break;
    }

    a=createnode();
    a->id=uid;
    a->tickets=tic;
    fflush(stdin);
      do
        {
            printf("\nEnter Name : ");
            gets(str);
            res=checkscan(str,2);
        }while(res==-1);
        strcpy(a->name,str);
       do
        {
            printf("\nEnter city : ");
            gets(str);
            res=checkscan(str,2);
        }while(res==-1);
        strcpy(a->city,str);
       do
        {
            printf("\nEnter address : ");
            gets(str);
            res=checkscan(str,2);
        }while(res==-1);
        strcpy(a->address,str);
    i=0;
    while(tic)
    {
        if(emptyq(q))
         {
             a->tic_id[i]=(*ref_id)++;
             a->status_tic[i]=1;
             i++;
         }
        else
        {
            a->tic_id[i]=dequeue(q);
            a->status_tic[i]=1;
            i++;
        }
        tic--;
    }
    if((*p)->start==NULL)
    {
        (*p)->start=a;
    }
    else
    {
        b=(*p)->start;
        while(b->next!=NULL)
            b=b->next;
        b->next=a;
    }

    fseek(fp,0,SEEK_END);
	var=*a;
    fwrite(&var,sizeof(NODE),1,fp);

}
//search record id-wise
int searchid(HNODE **p,int id)
{
    NODE *a=(*p)->start;
    while(a!=NULL)
    {
        if(a->id==id)
            break;
        a=a->next;
    }
    return (a!=NULL);
}
//Validtion of input
int checkscan(char str[],int flg)
{
    int opt,i;
    for(i=0;str[i]!='\0';i++)
    {
        if(flg==1&&(str[i]<48||str[i]>57))
            return -1;
        else if(flg==2&&((str[i]<65||str[i]>90)&&(str[i]<97||str[i]>122)&&str[i]!=32))
            return -1;
    }
    if(flg==1&&str[i]=='\0')
    {
         sscanf(str,"%d",&opt);
         return opt;
    }
    if(flg==2&&str[i]=='\0')
        return 1;
}
//search record namewise
int searchname(char *name,NODE **p)
{
    NODE *a=*p;
    while(a!=NULL)
    {
        if(strcmp(name,a->name)==0)
            break;
        else
            a=a->next;
    }
    return (a!=NULL);
}
//search record citywise
int searchcity(char *city,NODE **p)
{
    NODE *a=*p;
    while(a!=NULL)
    {
        if(strcmp(city,a->city)==0)
            break;
        else
            a=a->next;
    }
    return (a!=NULL);
}
//display mailing label of a ticket
void displayrecord(NODE *a)
{
    int i=0;
    printf("\n ------------------------------------------------------------------------------\n|");
    printf("\n|             INTERNATIONAL CRICKET CHAMPIONSHIP 2015-16");
    printf("\n|             ------------------------------------------");
    printf("\n|  * CUSTOMER ID:-%d\n|\n|  * NAME:-%s\n|\n|  * ADDRESS:-%-32s   * CITY:-%s\n|\n|  * NO.OF TICKETS:-%4d\t\t         * TICKET ID:-",a->id,a->name,a->address,a->city,a->tickets);
    for(i=0;i<(a->tickets)&&a->status_tic[i]==1;i++)
        printf("%4d",a->tic_id[i]);
    printf("\n|\n|              \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1");
    printf("\n ------------------------------------------------------------------------------");
}
//display total list
void display(HNODE **p)
{
    NODE *a=(*p)->start;
    while(a!=NULL)
    {
        if(a->del==1)
            displayrecord(a);
        a=a->next;
    }
}
//delete record and update that record in list and file
void deleterecord(HNODE **p,Q *q,int uid)
{
    NODE var;
     int deltic,i=0,tmp;
     NODE *a=(*p)->start;
    while(a!=NULL)
    {

        if(a->id==uid)
            break;
        a=a->next;
    }
    if(a->del==0)
    {
        printf("\nALREADY DELETED:");
        return;
    }
    do
    {
        printf("\n Enter no of tickets to deleted:");
        scanf("%d",&deltic);
    }while(deltic>a->tickets);
    if(deltic==a->tickets)
    {
        a->del=0;
    }
    for(i=0;i<a->tickets;i++)
    {
        if(i>=(a->tickets-deltic))
        {
            enqueue(q,a->tic_id[i]);
            a->status_tic[i]=0;
        }
    }
    a->tickets-=deltic;
    fseek(fp,0,SEEK_SET);
    fread(&tmp,sizeof(int),1,fp);
    while(!feof(fp))
    {
        fread(&var,sizeof(NODE),1,fp);
        if(var.id==uid)
            break;

    }

    fseek(fp,-(sizeof(NODE)),SEEK_CUR);
    var=*a;
    fwrite(&var,sizeof(NODE),1,fp);

}
//count no of tickets in particular city
int citycount(HNODE **p,char *city)
{
    int sum=0;
    NODE *a=(*p)->start;
    while(a!=NULL)
    {
        if(strcmp(a->city,city)==0)
                sum+=a->tickets;
        a=a->next;
    }
    return sum;
}
//delete list and release memory
void deletelist(HNODE **p)
{
    NODE *a,*b;
    b=a=(*p)->start;
    while(a!=NULL)
    {
        b=a;
        a=a->next;
        free(b);
    }

    free(*p);
}
int menu()
{
    int opt;
    char str[10];
    fflush(stdin);
    printf("\n \tINTERNATIONAL CRICKET CHAMPIONSHIP 2015-16\n");
    printf("\n\t\t1. New  booking\n\t\t2. Display all bookings\n\t\t3. Bookings for particular city\n\t\t4. Deleterecord\n\t\t5. Exit\n");
    do
    {
        printf("\nYour option Please : ");
        gets(str);
        opt=checkscan(str,1);
    }while(opt==-1);
    return opt;
}


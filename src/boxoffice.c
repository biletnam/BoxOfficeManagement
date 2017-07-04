#include"boxoffice.h"
int main()
{
    int opt,ref_id,res,id,rres;
    char filename[20],cityname[20],str[100];
    HNODE *p;
    Q qvar;
    qinit(&qvar);
    printf("\nENTER FILE NAME : ");
    gets(filename);
    fp=fopen(filename,"rb+");
    ref_id=updatelist(&p,filename,&qvar);
    updateq(&p,&qvar);
    ref_id+=qvar.qrear-qvar.qfront+1;

    res=emptyq(&qvar);

    printf("\n       ***************************************");
    printf("\n              TOTAL TICKETS:%4d\n",p->max_tic);
    printf("\n              TOTAL AVAILABLE BOOKINGS:%4d",(p->max_tic)-(ref_id)+2+(qvar.qrear-qvar.qfront));
    printf("\n       ***************************************\n");
    while(1)
    {
        opt=menu();
        if(opt>=5)
            break;
        switch(opt)
        {
        case 1:
            addrecord(&p,&ref_id,&qvar);break;
        case 2:
            display(&p);break;
        case  3:
            do
            {
            printf("\nENTER CITY TO BE COUNT NO OF TICKETS : ");
            gets(str);
            rres=checkscan(str,2);
            }while(rres==-1);
            strcpy(cityname,str);
            res=citycount(&p,cityname);
            printf("\nNO OF TICKETS BOOKED IN THIS CITY : %d",res);break;
        case 4:
            do
            {
            printf("\nEnter customer id who want deletion : ");
            gets(str);
            rres=checkscan(str,1);
            }while(rres==-1);
            id=rres;
            deleterecord(&p,&qvar,id);break;
        }
    }
	fclose(fp);
	deletelist(&p);
	return 0;
}


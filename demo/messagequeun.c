/*
˵����
1.�����̴���������Ϣ���У�֮������ӽ��̣�ͨ����Ϣ�������ӽ��̽���ͨ�š�
2.Ҫ���Ӧ��linux�źŲ�����ƣ���Ҫ������SIGINT(ctrl+c)����ֹ�����˳���
���ǲ�û��ɾ����Ϣ���У�linuxϵͳ����Ϣ���еĸ����������Ƶġ�����Ҫ����
��SIGINT�źŲ���֮����Ҫɾ����Ϣ���У���ͨ��kill��������ӽ���Ҫ�˳���
֮��ͨ��waitpid����ȷ���ӽ���ȫ���˳���������̲��˳���

Ŀ�ģ�

*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/select.h>
#include <iconv.h>
#include <locale.h>


#define FATHER_TYPE 1000

#define EXCEPTION -1		//�쳣
#define NORMAL 0		//����
#define MAX_CHILD_NUM 1	//�����ӽ�������
#define UNUSED 10		//δռ��
#define USED	-10		//ռ��
#define IDLE	0		//����
#define BUSY 1		//��æ
#define DEFAULT -1

/*�ӽ�����Ϣ�ṹ��*/
typedef struct{
    int nChildPid;		//�ӽ���pid��
    int nPos;			//����λ��
    int nFlag;		//����λ���Ƿ����
    int nStatus;		//��ǰ״̬�������ǻ
}StruChildProcInfo;

/*���̹���ṹ��*/
typedef struct{
	int nCurrentProcNum;					//��ǰ�ӽ�������
	StruChildProcInfo struChild[MAX_CHILD_NUM];	//�ӽ�����Ϣ�ṹ������
}StruChildProcCtrl;

/*��Ϣ�ṹ��*/
typedef struct{
    int nPid;
    int nPos;
    char szContext[128];
}StruChildMsg;

/*��ǽṹ��*/
typedef struct{
    long MsgType;			//ָ����Ϣ����
    StruChildMsg struMyMsg;		//������Ϣ�������ڵĵط�
}StruMsg;

/*ȫ�ֱ���*/
static int gnF2CMsgQueue;	//�����̵��ӽ�����Ϣ����
static int gnC2FMsgQueue;	//�ӽ��̵���������Ϣ����
static int gnMainPid;	//�����̵�pid
static StruMsg struFMsg;		//��������Ϣ�ṹ��
static StruMsg struCMsg;		//�ӽ�����Ϣ�ṹ��
static struct sigaction struAct;		//�ź�����
static struct sigaction struOldAct;	//ԭ�ź�����

/*��������*/
void Init(StruChildProcCtrl *struChildCtrl);
int CreateMsgQueue(int *pnResult);
void DeleteMsgQueue();
void StartChildTask();
void CaptureQuitSignal(int nSignal);

int main(int argc,char **argv)
{
    /*����*/

    int i;
    int nPid;		//pidֵ
    int nRet;		//��������ֵ
    int nStatus;    //״̬
    int nInitFlag=0;
    StruChildProcCtrl struChildCtrl;	//�����̵Ĺ���ṹ��

    /*��ʼ��*/
    i=0;
    nPid=0;
    nRet=0;
    nStatus=0;
    gnMainPid=getpid();

    /*����ṹ���ʼ��*/
    Init(&struChildCtrl);

    /*�����źŴ���*/
    struAct.sa_handler=CaptureQuitSignal;
    struAct.sa_flags=0;
    sigfillset(&struAct.sa_mask);
    if(sigaction(SIGINT,&struAct,&struOldAct)==-1)
    {
	printf("[ERRORS] sigaction is failed!\n");
	return EXCEPTION;
    }

    /*������Ϣ����*/
    nRet=CreateMsgQueue(&nStatus);
    if(nRet!=NORMAL&&nStatus!=2)
    {
	printf("[ERRORS] CreateMsgQueue is failed!\n");
	return EXCEPTION;
    }
    else
    {
        printf("[INFO] ������Ϣ���гɹ�!\n");
    }

    /*�����ӽ���*/
    while(1)
    {
        if(nInitFlag)
        {
            usleep(50000);
	    /*�����ӽ�����Ϣ*/
	    memset(&struFMsg,0,sizeof(StruMsg));
	    if(msgrcv(gnC2FMsgQueue,(struct msgbuf*)&struFMsg,sizeof(StruChildMsg),FATHER_TYPE+1,IPC_NOWAIT)==-1)
	    {
	        if(errno!=ENOMSG)
	        {
		     printf("[ERRORS] msgrcv is failed!\n");
		     /*������Ҫ���źţ������˳�֮ǰҪ����һЩ����*/
		     continue;
	        }
	    }

            usleep(50000);
            /*�����ӽ�����Ϣ������ --�ı����״̬*/
	    for(i=0;i<MAX_CHILD_NUM;i++)
	    {
	        if(struFMsg.struMyMsg.nPid==struChildCtrl.struChild[i].nChildPid)
	        {
		     struChildCtrl.struChild[i].nStatus=IDLE;
		     break;
	        }
	    } 
         }
	
	/*�ӽ�����Ϣ*/
	printf("[INFO] father recv:%s\n",struFMsg.struMyMsg.szContext);
        usleep(50000);

	while(struChildCtrl.nCurrentProcNum<MAX_CHILD_NUM)
	{
            printf("[INFO] ��Ҫ�����ӽ���!\n");
	    nPid=fork();

	    switch(nPid)
	    {
		/*fork ����*/
		case -1:
			printf("[ERRORS] fork is failed!\n");
		break;

		/*�����ӽ���*/
		case 0:
                        usleep(50000);
                        printf("[INFO] �ӽ��̽��빤������!\n");
			StartChildTask();
		break;

		/*���븸����*/
	 	default:
                        printf("[INFO] ����������!\n");
                        if(!nInitFlag)
                        {
                            nInitFlag=1;
                            printf("[INFO] ��ʼ�����óɹ�!\n");
                        }

			for(i=0;i<MAX_CHILD_NUM;i++)
			{
			    if(struChildCtrl.struChild[i].nFlag==UNUSED\
				&&struChildCtrl.struChild[i].nChildPid<0)
			    {
			        struChildCtrl.nCurrentProcNum++;
			        struChildCtrl.struChild[i].nFlag=USED;
				 struChildCtrl.struChild[i].nChildPid=nPid;
				 struChildCtrl.struChild[i].nPos=i;
				 struChildCtrl.struChild[i].nStatus=IDLE;
                                 printf("[INFO] %d[%d] �ӽ������óɹ�!\n",struChildCtrl.struChild[i].nChildPid,nPid);
				 break;
			    }
			}
		 break;
	    }//end switch(nPid)
	 }//end while

	/*�ӽ���״̬���*/
        usleep(5000);
        printf("[INFO] �����̽����ӽ���״̬���׶�!\n");
	for(i=0;i<MAX_CHILD_NUM;i++)
	{
	    if(struChildCtrl.struChild[i].nChildPid>0)
	    {
		 if(kill(struChildCtrl.struChild[i].nChildPid,0)==-1&&errno==ESRCH)
		 {
			struChildCtrl.nCurrentProcNum--;
			struChildCtrl.struChild[i].nFlag=UNUSED;
			struChildCtrl.struChild[i].nChildPid=-1;
			struChildCtrl.struChild[i].nPos=-1;
			struChildCtrl.struChild[i].nStatus=DEFAULT;
		 }
                 else
                 {
                     printf("[INFO] %d �ӽ�������!\n",struChildCtrl.struChild[i].nChildPid);
                 }
	    }
	}

	/*����Ϣ���ӽ���*/
	for(i=0;i<MAX_CHILD_NUM;i++)
	{
	    if(struChildCtrl.struChild[i].nChildPid>0&&struChildCtrl.struChild[i].nStatus==IDLE)
	    {
                 printf("[INFO] �ҵ������ӽ���!\n");
		 struChildCtrl.struChild[i].nStatus=BUSY;
		 break;
	    }
	}

	struFMsg.MsgType=struChildCtrl.struChild[i].nChildPid;
	struFMsg.struMyMsg.nPid=struChildCtrl.struChild[i].nChildPid;
	struFMsg.struMyMsg.nPos=i;
	memset(struFMsg.struMyMsg.szContext,0,sizeof(struFMsg.struMyMsg.szContext));
	strcpy(struFMsg.struMyMsg.szContext,"һ�����������ӣ���ʶ�����徭�����ҽ��߰˾��ӣ�ʮ�ִ󵨣�");

	if(msgsnd(gnF2CMsgQueue,(struct msgbuf*)&struFMsg,sizeof(StruChildMsg),IPC_NOWAIT)==-1)
	{
	    printf("[ERRORS] msgsnd is failed!\n");
            usleep(50000);
	}
        else
        {
            printf("[INFO] ���ӽ��̷�����Ϣ�ɹ�!\n");
            usleep(50000);
        }
     }//end while(1)
    
    return 0;
}

/*��������*/

void Init(StruChildProcCtrl *struChildCtrl)
{
    int i=0;

    struChildCtrl->nCurrentProcNum=0;
    for(i=0;i<MAX_CHILD_NUM;i++)
    {
	struChildCtrl->struChild[i].nPos=-1;
	struChildCtrl->struChild[i].nChildPid=-1;
	struChildCtrl->struChild[i].nFlag=UNUSED;
	struChildCtrl->struChild[i].nStatus=DEFAULT;
    }
}

int CreateMsgQueue(int *pnResult)
{
	/*�����̵��ӽ�����Ϣ����*/
	gnF2CMsgQueue = msgget(IPC_PRIVATE, IPC_CREAT|0777);
	if(gnF2CMsgQueue == -1)
	{
		printf("[ERRORS][MsgFunc.c]����msgget������������![%s]\n",strerror(errno));
		return EXCEPTION;
	}
	
	*pnResult++;

	/*�ӽ��̵���������Ϣ����*/
	gnC2FMsgQueue = msgget(IPC_PRIVATE,IPC_CREAT|0777);
	if(gnC2FMsgQueue == -1)
	{
		printf("[ERRORS][MsgFunc.c]����msgget������������![%s]\n",strerror(errno));
		return EXCEPTION;
	}

	*pnResult++;

	return NORMAL;
}

void StartChildTask()
{
    int nSelfPid;

    nSelfPid=getpid();

    while(1)
    {
	/*�����ȴ���������Ϣ*/
        printf("[INFO] �ȴ���������Ϣ!\n");
        struCMsg.MsgType=nSelfPid;
	if(msgrcv(gnF2CMsgQueue,(struct msgbuf*)&struCMsg,sizeof(StruChildMsg),nSelfPid,0)==-1)
	{
	    printf("[ERRORS]���ո�������Ϣ  msgrcv is failed!\n");
            usleep(5000);
	    continue;
	}

	/*��������Ϣ*/
	printf("[INFO] �ӽ����յ�recv:%s\n",struCMsg.struMyMsg.szContext);

	/*������Ϣ��������*/
	struCMsg.MsgType=FATHER_TYPE+1;
	memset(struCMsg.struMyMsg.szContext,0,sizeof(StruChildMsg));
	strcpy(struCMsg.struMyMsg.szContext,"ʮ�Ҿ�ƶ���յð�����ë����������壬�������Ķ��⣬һ��������");
	if(msgsnd(gnC2FMsgQueue,(struct msgbuf*)&struCMsg,sizeof(StruChildMsg),IPC_NOWAIT)==-1)
	{
	    printf("[ERRORS] �򸸽��� msgsnd is failed!\n");
            usleep(5000);
	    continue;
	}
    }

}

void DeleteMsgQueue()
{
    msgctl(gnF2CMsgQueue,IPC_RMID,NULL);	
    msgctl(gnC2FMsgQueue,IPC_RMID,NULL);
}

void CaptureQuitSignal(int nSignal)
{
    kill(0,SIGTERM);

    /*ɾ����Ϣ����*/
    DeleteMsgQueue();    

    /*�ָ��ź�����*/
    if(sigaction(SIGINT,&struOldAct,NULL)==-1)
    {
	 printf("[ERRORS] sigaction is failed!\n");
    }

    exit(0);
}

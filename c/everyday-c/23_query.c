/* ѧ���ɼ���ѯϵͳ */
# include <stdio.h>
# include <stdlib.h>

void main( )
{
	int select;
	int i, j;
	int score[5][7];
	int average = 0;
	int sum = 0;
    do{
		//printf("��������4��ܣ�\n");
		printf("1.benchengxu you  sixiang  gongneng\n");
		//printf(" 1. ����ѧ�Ų�ѯѧ���ɼ�\n");
		printf("1.genju xuehao chaxun  xuesheng   chengji\n");
		//printf(" 2. ���ݿ��Ժ�ͳ�Ƴɼ�\n") ;
		printf("2.genju kaohao chaxun  chengji\n");
		//printf(" 3. ���ݿ��Ժź�ѧ�Ų�ѯ�ɼ�\n");
		printf("3. genju xuehao chaxun chengji\n");
		//printf(" 4. �ɼ�¼��\n");
		printf("4.chengji  luru\n");
		//printf(" 0. �˳�\n");
		printf("0.tuichu \n");
		//printf(" ������ѡ��(0 - 4): ");
		printf("qing shuru  xuanze\n");
		scanf("%d", &select);
		switch(select)
		{
		case 0:
			printf("OK\n");
			exit(0);
			break;
		case 1:
			//printf("����ѧ�ţ�");
			printf("shuru xuehao\n");
			scanf("%d\n", &i);
			for(j=1; j<7; j++)
			{
				printf("di %d ke chengji shi %d\n", j, score[i][j]);
				sum += score[i][j];
			}
			average = sum/6;
			printf("average  is %d\n", average);
			break;
		case 2:
			printf("shuru kaohao��");
			scanf("%d\n", &j);
			for(i=1; i<5; i++)
			{
				printf("di %d  hao  xuesheng chejishi %d\n", i, score[i][j]);
				sum += score[i][j];
			}
			average = sum/4;
			printf("benke pingjunchengjishi %d\n", average);
			break;
		case 3:
			printf("shuru xuehao he  kaohao ");
			scanf("%d %d\n", &i, &j);
			printf("di %d hao  xuesheng de di %d ke kaoshi chengji shi%d\n", i, j, score[i][j]);
			break;
		case 4:
			printf("qing shuru chengji \n");
			for(i=1; i<5; i++)
				for(j=1; j<7; j++)
					scanf("%d\n", &score[i][j]);
			break;
		default:
			break;
		}
	}while(1);
}
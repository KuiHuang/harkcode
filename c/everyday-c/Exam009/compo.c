# include <stdio.h>
	
void main()
{
		int x1, x2, x3, mid;
		printf("��������������������\n");
		scanf("%d %d %d", &x1, &x2, &x3);

		if(x1 < x2)
		{
			mid = x1; x1 = x2; x2 = mid; 
		}//��x y����λ��
		if(x1 < x3)
		{
			mid = x1; x1 = x3; x3 = mid; 
		}//�� x  z  ����λ��
		if(x2 < x3)
		{
			mid = x2; x2 = x3; x3 = mid; 
		}//��  y  z ����λ��
		printf("%d %d %d\n",x1,x2,x3);//�ɴ�С����   ���x1��С��x2��x3��ʱ��Ͳ���λ��
		printf("������һ����������������������ж������\n");
		//scanf("%d", &dec);
		//if(dec >= 0)	
		printf("�������Ϊ��%d\n", x1);
		//else	
		printf("��С����Ϊ��%d\n", x3);
}

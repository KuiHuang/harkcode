# include <stdio.h>

void main()
{
	/* ����һ���������͵ı�����������ź�����ʽ��ֵ */
	int logic;    

	int a = 1;
	int b = 2;
	int c = 3;

	logic = a+b>c&&b<=c;//0
	printf("logic = %d\n", logic);

	logic = a>=b+c||b==c;//0
	printf("logic = %d\n", logic);

	logic = !(a<c)+b!=1&&(a+c)/2;//1
}
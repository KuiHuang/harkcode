# include <stdio.h>

void main()
{
	/* ������һ���޷����ַ��ͱ������˱���ֻ�������洢�޷����� */
	unsigned char result;
    
	int a, b, c, d;
	a = 2;//0010
	b = 4;//0100
	c = 6;//0110
	d = 8;//1000

	/* �Ա������С���λ�롱���� */
	result = a & c;// 2
	printf("result = %d\n", result);

	/* �Ա������С���λ�򡱲��� */
	result = b | d;//12
	printf("result = %d\n", result);

	/* �Ա������С���λ��򡱲��� */
	result = a ^ d;//10
	printf("result = %d\n", result);

	/* �Ա������С�ȡ�������� */
	result = ~a;    //255-2
	printf("result = %d\n", result);
}
# include <stdio.h>

void main()
{
	int i, j, k;
	int m, n, p;

	i = 8;
	j = 10;
	k = 12;
    
	/* �����ڲ�����֮ǰ */
	m = ++i;
    printf("i = %d\n", i);//9
	printf("m = %d\n", m);//9

	/* �Լ��ڲ�����֮�� */
	n = j--;
	printf("j = %d\n", j);	//9
	printf("n = %d\n", n);//  �Ƚ�jԭ����ֵ���븳ֵ����   ����  n��10  ����9

	/* �������Լ��Ļ������ */
	p = (++m)*(n++)+(--k);
	printf("k = %d\n", k);	//11
	printf("p = %d\n", p);	//102       (9+1)*(10)+11
}

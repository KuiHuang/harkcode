# include <stdio.h>

void main()
{
	/* �гߴ� */
	/* һά���������ʼ�� */
	int  array1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	/* һά�ַ��������ʼ�� */
	char array2[13] = "How are you!";    /* ��ʽһ */
	char array3[13] = {'H','o','w',' ','a','r','e',' ','y','o','u','!'};    /* ��ʽ�� */

	/* ��ά���������ʼ�� */
	int array4[4][4] = 
	{
		12, 18,  6, 25,
	    23, 10, 32, 16,
		25, 63,  1, 63,
		 0,  0, 27, 98
	};    /* �������ʽ */

	/* �޳ߴ� */
	/* һά�ַ��������ʼ�� */
	char array5[] = "How are you!";
	int array6[][2] = {
		{ 1,50},
		{45, 2},
		{ 2, 0},
		{12,32},
		{42,33},
		{15,18}
	};
	
	
	printf("1:%d\n2:%d\n3:%d\n4:%d\n5:%d\n6:%d\n",sizeof(array1),sizeof(array2),sizeof(array3),sizeof(array4),sizeof(array5),sizeof(array6));
}
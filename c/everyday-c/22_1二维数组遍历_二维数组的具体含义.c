# include <stdio.h>

void main()
{
	/* 有尺寸 */
	/* 一维整形数组初始化 */
	int  array1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	/* 一维字符型数组初始化 */
	char array2[] = "How are you!";    /* 方式一 */
	char array3[] = {'H','o','w',' ','a','r','e',' ','y','o','u','!'};    /* 方式二 */

	/* 二维整形数组初始化 */
	int array4[4][4] = 
	{
		12, 18,  6, 25,
	    23, 10, 32, 16,
		25, 63,  1, 63,
		 0,  0, 27, 98
	};    /* 矩阵的形式 */

	/* 无尺寸 */
	/* 一维字符型数组初始化 */
	char array5[] = "How are you!";
	int i;
	int array6[][3] = {
		 {1,50},
		 {2, 0},
		{12,32},
		{42,33},
		{15,18,3}
	};
	int *array=array6[0];
	for(i=0;i<13;i++)
	{
		//printf("array[%d]: %d\n",i,(*array6)[i]);
		//printf("array[%d]: %d\n",i,*(array+i));
		printf("array[%d]: %d\n",i,*((int*)(array)+i));
	}
	printf("数组的名字  array6:  %p" , array6);
	printf("数组的名字  array6:  %p" , &array6[0]);
	
	printf("1:%d\n2:%d\n3:%d\n4:%d\n5:%d\n6:%d\n",sizeof(array1),sizeof(array2),sizeof(array3),sizeof(array4),sizeof(array5),sizeof(array));
}
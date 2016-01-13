//
// 来自: http://www.slyar.com/blog/kruskal-disjoint-sets-c.html
//
#include <stdio.h>
#include <stdlib.h>
 
#define MAX 100
 
/* 定义边(x,y)，权为w */
typedef struct
{
	int x, y;
	int w;
}edge;
 
edge e[MAX];
int rank[MAX]; /* rank[x]表示x的秩 */
int father[MAX]; /* father[x]表示x的父节点 */
int sum;

int cmp(const void *a, const void *b)/* 比较函数，按权值(相同则按x坐标)非降序排序 */
{
	if ((*(edge *)a).w == (*(edge *)b).w)
	{
		return (*(edge *)a).x - (*(edge *)b).x;
	}
	return (*(edge *)a).w - (*(edge *)b).w;
}
 
void Make_Set(int x)/* 初始化集合 */
{
	father[x] = x;
	rank[x] = 0;
}
 
/* 查找x元素所在的集合,回溯时压缩路径 */
int Find_Set(int x)
{
	if (x != father[x]){	
		father[x] = Find_Set(father[x]);
	}
	return father[x];
}
 
/* 合并x,y所在的集合 */
void Union(int x, int y, int w)
{
 
	if (x == y) return;
	
	if (rank[x] > rank[y]){ /* 将秩较小的树连接到秩较大的树后 */
	
		father[y] = x;
	}
	else{	
		if (rank[x] == rank[y]){		
			rank[y]++;
		}
		father[x] = y;
	}
	sum += w;
}
 
/* 主函数 */
int main()
{
	int i, n;
	int x, y;
	char chx, chy;

	scanf("%d", &n); /* 读取边的数目 */	
	getchar();
 
	/* 读取边信息并初始化集合 */
	for (i = 0; i < n; i++)
	{
		scanf("%c %c %d", &chx, &chy, &e[i].w);
		getchar();
		e[i].x = chx - 'A';
		e[i].y = chy - 'A';
		Make_Set(i);
	}
 
	/* 将边排序 */
	qsort(e, n, sizeof(edge), cmp);
 
	sum = 0;
 
	for (i = 0; i < n; i++)
	{
		x = Find_Set(e[i].x);
		y = Find_Set(e[i].y);
		if (x != y)
		{
			printf("%c - %c : %d\n", e[i].x + 'A', e[i].y + 'A', e[i].w);
			Union(x, y, e[i].w);
		}
	}
 
	printf("Total:%d\n", sum);
	//system("pause");
	return 0;	
}
#include <stdio.h>
#include <stdlib.h>

#define VNUM 5
#define MV 65536

int PeerNode[VNUM]; // 保存了最短路径对应的对等节点 记录寻找最短路径走过的节点
int Dist[VNUM];     // 最短路径
int Mark[VNUM];     // 访问过的节点

int Matrix[VNUM][VNUM] =
{
    {0, 10, MV, 30, 100},
    {MV, 0, 50, MV, MV},
    {MV, MV, 0, MV, 10},
    {MV, MV, 20, 0, 60},
    {MV, MV, MV, MV, 0},
};

void Dijkstra(int sv) // O(n*n)
{
    int i = 0;
    int j = 0;
    
    if( (0 <= sv) && (sv < VNUM) ){    
        for(i=0; i<VNUM; i++){       // 初始化 
            Dist[i] = Matrix[sv][i];
            PeerNode[i] = sv;
            Mark[i] = 0;
        }
        
        Mark[sv] = 1;
        
        for(i=0; i<VNUM; i++) {       
            int min = MV;
            int peer = -1;
            
            for(j=0; j<VNUM; j++)  { // 1.找到sv顶点对应的最短边和对等的顶点         
                if( !Mark[j] && (Dist[j] < min) ) {               
                    min = Dist[j];
                    peer = j;
                }
            }
            
            if( peer > -1 ) {           
                Mark[peer] = 1;
            }
            
            for(j=0; j<VNUM; j++){  // 2.在对等节点peer的边集合里面比较 比较sv到某顶点的最小值          
                if( !Mark[j] && (min + Matrix[peer][j] < Dist[j]) ) {               
                    Dist[j] = min + Matrix[peer][j];
                    PeerNode[j] = peer;
                }
            }
        }
        
        // 这里可以看出是贪心算法的查找
        for(i=0; i<VNUM; i++){        
            int p = i;            
            printf("%d -> %d: %d\n", sv, p, Dist[p]);
            
            do
            {
                printf("%d <- ", p);
                p = PeerNode[p];
            } while( p != sv );
            
            printf("%d\n", p);
        }
    }
}

int main(int argc, char *argv[]) 
{
    Dijkstra(0);
	return 0;
}
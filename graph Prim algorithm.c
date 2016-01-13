#include <stdio.h>
#include <stdlib.h>

#define VNUM 9
#define MV 65536

int PeerNode[VNUM];
int Cost[VNUM];
int Mark[VNUM];

int Matrix[VNUM][VNUM] =
{
    {0, 10, MV, MV, MV, 11, MV, MV, MV},
    {10, 0, 18, MV, MV, MV, 16, MV, 12},
    {MV, 18, 0, 22, MV, MV, MV, MV, 8},
    {MV, MV, 22, 0, 20, MV, MV, 16, 21},
    {MV, MV, MV, 20, 0, 26, MV, 7, MV},
    {11, MV, MV, MV, 26, 0, 17, MV, MV},
    {MV, 16, MV, MV, MV, 17, 0, 19, MV},
    {MV, MV, MV, 16, 7, MV, 19, 0, MV},
    {MV, 12, 8, 21, MV, MV, MV, MV, 0},
};

void PeerNoderim(int sv) // O(n*n)
{
    int i = 0;
    int j = 0;
    
    if( (0 <= sv) && (sv < VNUM) ) {   
        for(i=0; i<VNUM; i++){        // 初始化几个数组   
            Cost[i] = Matrix[sv][i];  // 某个顶点各边的权值 用来记录最小边
            PeerNode[i] = sv;         // 标记了某个顶点对应最小边的另外一个顶点  
            Mark[i] = 0;              // 标记的节点
        }
        
        Mark[sv] = 1;
        
        for(i=0; i<VNUM; i++) {       
            int min = MV;
            int peer = -1;
            
            // 1.遍历某个节点的各个权值 找到最小权值的边和对等节点peer      
            for(j=0; j<VNUM; j++){    
                if( !Mark[j] && (Cost[j] < min) ) {               
                    min = Cost[j];
                    peer = j; // peer标记了这个节点权值最小的边 这条边的另外一个节点
                }
            }
            
            if( peer > -1 ) {           
                Mark[peer] = 1;  
                // PeerNode[]记录了最小边的对等节点 将权值最小边的另外一个定点标记为访问过了 可以连通  打印出来这条路径            
                printf("(%d, %d, %d)\n", PeerNode[peer], peer, Cost[peer]);
            }
            
            // 2.遍历对等节点peer 找到权值最小的边
            for(j=0; j<VNUM; j++){   
                if( !Mark[j] && (Matrix[peer][j] < Cost[j]) ) {              
                    Cost[j]  = Matrix[peer][j]; // Cost[]数组记录了 顶点peer到各个顶点的权值
                    PeerNode[j] = peer; // 对等节点
                }
            }
        }
    }
}

int main(int argc, char *argv[]) 
{
    PeerNoderim(0);    
	return 0;
}
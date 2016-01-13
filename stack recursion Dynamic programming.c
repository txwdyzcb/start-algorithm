#include <iostream>
#include <map>
using namespace std;

map<int, int> known;

bool known_find(int n){
    map<int, int>::iterator it = known.find(n);

    return ((it != known.end()) ? true : false);
}

int fibonacci(int n){ // O(N)
    int t;
    if(known_find(n)) return known[n]; // 从子问题的解中得到问题的解

    if(0 == n) t = 0;
    if(1 == n) t = 1;
    if(n > 1)  t = fibonacci(n-1) + fibonacci(n-2);

    return known[n] = t; // 将子问题的解保存起来
}

int main()
{
    int i = 0;

    for(i=1; i<=10; i++)
    {
        printf("fibonacci(%d) = %d\n", i, fibonacci(i));
    }

    return 0;
}

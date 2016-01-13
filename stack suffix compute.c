#include <stdio.h>
#include <stdlib.h>
#include "stack/LinkStack.h"

int isNumber(char c)
{
    return ('0' <= c) && (c <= '9');
}

int isOperator(char c)
{
    return (c == '+') || (c == '-') || (c == '*') || (c == '/');
}
int value(char c)
{
    return (c - '0');
}
int express(int left, int right, char op)
{
    int ret = 0;

    switch(op)
    {
        case '+':
            ret = left + right;
            break;
        case '-':
            ret = left - right;
            break;
        case '*':
            ret = left * right;
            break;
        case '/':
            ret = left / right;
            break;
        default:
            break;
    }

    return ret;
}
//计算机对后缀表达式的计算是基于栈的
int compute(const char* exp)
{
  LinkStack* stack = LinkStack_Create();
  int ret = 0;
  int i = 0;
//1.遍历后缀表达式的数字和符号
  while( exp[i] != '\0')
  {
      if( isNumber(exp[i]) )//1.1对于数字:进栈
      {
          LinkStack_Push(stack, (void*)value(exp[i]));
      }
      else if( isOperator(exp[i]))//1.2对于符号
      {
          //1.从栈中弹出右操作数
          //2.从栈中弹出做操作数
          //3.根据符号进行运算
          //4.将原始结果压入栈中
          int right = (int)LinkStack_Pop(stack);
          int left = (int)LinkStack_Pop(stack);
          int result = express(left, right, exp[i]);

          LinkStack_Push(stack, (void*)result);
      }
      else
      {
          printf("invalid expression!\n");
          break;
      }
      ++i;
  }
  //2.遍历结束:栈中的唯一数字为计算结果
  if( (1 == LinkStack_Size(stack)) && ('\0' == exp[i]))
  {
      ret = (int)LinkStack_Pop(stack);
  }
  else
  {
      printf("Invalid expression!");
  }

  LinkStack_Destroy(stack);

  return ret;
}
int main()
{
    printf("9 + (3 - 1) * 5 + 8 / 2 = %d\n", compute("931-5*+82/+"));

    return 0;
}
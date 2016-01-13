#include <stdio.h>
#include <stdlib.h>
#include "stack/LinkStack.h"

int isNumber(char c)
{
  return ('0' <= c) && (c <= '9');
}
int isOperator(char c)
{
  return ('+' == c) || ('-' == c) || ('*' == c) || ('/' == c);
}
int isRight(char c)
{
  return ( ')' == c);
}
int isLeft(char c)
{
  return ( '(' == c);
}
int priority(char c)
{
  int ret = 0;

  if( ('+' == c) || ('-' == c))
  {
    ret = 1;
  }
  if( ('*' == c) || ('/' == c))
  {
    ret = 2;
  }

  return ret;
}
void output(char c)
{
  if(c != '\0')
  {
      printf("%c", c);
  }
}

void transform(const char* exp)
{
  LinkStack* stack = LinkStack_Create();
  int i = 0;

  while( exp[i] != '\0')
  {
      if( isNumber(exp[i]))//1.如果是数字 直接输出
      {
          output(exp[i]);
      }
      else if(isOperator(exp[i]))//2.如果是运算符号 + - * / 入栈
      {
          //3.如果栈顶的运算符级别高 先出栈(将栈顶元素弹出并且输出) 再入栈
          while( priority(exp[i]) <= priority( (char)(int)LinkStack_Top(stack) ) )
          {
              output( (char)(int)LinkStack_Pop(stack));
          }
          LinkStack_Push(stack, (void*)(int)exp[i]);//栈顶元素运算级低 直接入栈
      }
      else if( isLeft(exp[i]) )//4.如果是左运算符( 则入栈
      {
          LinkStack_Push(stack, (void*)(int)exp[i]);
      }
      else if( isRight(exp[i]) )//5.如果是右运算符) 则出栈
      {
          while( !isLeft((char)(int)LinkStack_Top(stack)) )//如果栈顶元素不是左括号( 则出栈顶元素 直到为(
          {
              output((char)(int)LinkStack_Pop(stack));
          }

          LinkStack_Pop(stack);//栈顶元素为( 则出栈
      }
      else
      {
          printf("Invalid expression!");
          break;
      }

      i++;
  }
  //6.遍历结束将栈中的左右元素弹出并且输出
  while( (LinkStack_Size(stack) > 0) && (exp[i] == '\0') )
  {
      output((char)(int)LinkStack_Pop(stack));
  }

  LinkStack_Destroy(stack);
}

int main()
{
    transform("9+(3-1)*5+8/2");

    printf("\n");

    return 0;
}
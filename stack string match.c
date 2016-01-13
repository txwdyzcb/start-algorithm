#include <stdio.h>
#include <stdlib.h>
#include "stack/linkstack.h"


int isLeft(char ch);
int isRight(char ch);
int match(char left, char right);
int scanner(const char *code);

int main(void)
{
  const char* code = "int main(void) {const char* code = ""; scanner(code;  return 0);  }";

  scanner(code);

  return 0;
}

int isLeft(char ch)
{
  int ret = 0;

  switch(ch)
  {
    case '<':
    case '[':
    case '(':
    case '\'':
    case '\"':
    case '{':
      ret = 1;
      break;
    default:
      ret = 0;
      break;
   }

  return ret;
}

int isRight(char ch)
{
  int ret = 0;

  switch(ch)
  {
    case '>':
    case ']':
    case ')':
    case '\'':
    case '\"':
    case '}':
      ret = 1;
      break;
    default:
      ret = 0;
      break;
   }

  return ret;
}
int match(char left, char right)
{
  int ret = 0;

  switch(left)
  {
    case '<':
      ret = ('>' == right );
      break;
    case '[':
      ret = (']' == right );
      break;
    case '(':
      ret = (')' == right );
      break;
    case '\'':
      ret = ('\'' == right );
      break;
    case '\"':
      ret = ('\"' == right );
      break;
    case '{':
      ret = ('}' == right );
      break;
      ret = 1;
      break;
    default:
      ret = 0;
      break;
   }

  return ret;
}

//栈适用于成对出现的对象 比如对称等问题的解决
int scanner(const char* code)
{
  LinkStack* stack = LinkStack_Create();
  int ret = 0;
  int i = 0;

  while( code[i] != '\0')
  {
      if( isLeft(code[i]) )//1.如果是左括号则入栈
      {
          LinkStack_Push(stack, (void*)(code + i));
      }
      if( isRight(code[i]) )//2.如果是有括号
      {
          char* ch = (char*)LinkStack_Pop(stack);

          if( (ch == NULL) || !match(*ch, code[i]) )//3.将栈顶元素与其匹配 栈为空 或者不匹配
          {
              printf("%c does not match!\n", code[i]);
              ret = 0;
              break;
          }
      }
      ++i;
  }
  if( (0 == (LinkStack_Size(stack))) && ('\0' == code[i]))
  {
      printf("succeed\n");
  }
  else
  {
      printf("Invalid code!\n");
  }

  LinkStack_Destroy(stack);

  return ret;
}
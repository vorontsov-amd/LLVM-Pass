#include <stdio.h>

void print(int x) {
  if (x) printf("Hello, world!\n");
  else   printf("Hell   world!\n");
}

int func(int arg1, int arg2) {
  return arg1 + arg2 * 2;
}

int main() {
  print(1);
  return func(func(3, 4), func(5, 6)) + func(3, 4);
}

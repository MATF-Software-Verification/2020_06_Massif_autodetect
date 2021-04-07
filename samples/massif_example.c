#include <stdlib.h>

void g(void)
{
  malloc(4000);
}

void f(void)
{
  void* a = malloc(2000);
  g();
}

void huge(void)
{
  malloc(15000);
}

int main(void)
{
  int i;
  int* a[10];

  for (i = 0; i < 10; i++) {
    a[i] = malloc(1000);
  }

  f();
  g(); 

  for (i = 0; i < 10; i++) {
    free(a[i]);
  }

  void* x = malloc(1000);
  free(x);

  return 0;
}

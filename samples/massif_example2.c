#include <stdlib.h>

void g(void)
{
  malloc(4000);
}

void f(void)
{
  malloc(2000);
  g();
}

int main(void)
{
  void* a = malloc(1000);
  void* b = malloc(1000);
  void* c = malloc(1000);
  void* d = malloc(1000);
  void* e = malloc(1000);

  free(c);
  free(b);
  free(d);
  
  return 0;
}

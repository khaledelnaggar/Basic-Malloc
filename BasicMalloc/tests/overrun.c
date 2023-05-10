#include <stddef.h>
#include "assert.h"
#include <stdlib.h>

#ifdef assert
#undef assert
#endif
#define assert(val) do {\
 if ((val) == 0) exit(-1);\
} while (0);

#define SOME_CONSTANT 'l'
#define ANOTHER_CONSTANT 'm'

#define SOME_CONSTANT_2 0x9999999
#define ANOTHER_CONSTANT_3 0x666

int i = 0;

int main()
{


  char*a1 = (char *)malloc(2563623);
  for (size_t i = 0; i < 2563623; i++)
  {
    a1[i] = SOME_CONSTANT;
  }
  free(a1);
  char*a2 = (char *)malloc(787787);
  char* a3 = (char *)malloc(30);

  char*a4 = (char *)malloc(40);
  char* a5 = (char *)malloc(999);

  char* a6 = (char *)malloc(7833344);

  for (size_t j = 0; j < 40; ++j)
  {
    a4[j] = ANOTHER_CONSTANT;
  }
  
  for (size_t j = 0; j < 999; ++j)
  {
    a5[j] = ANOTHER_CONSTANT;
  }
  for (size_t j = 0; j < 7833344; ++j)
  {
    a6[j] = ANOTHER_CONSTANT;
  }

  free(a6);
  free(a5);
  free(a4);
  free(a3);

  free(a2);
  free(a1);

  a1 = (char *)malloc(10);
  a2 = (char *)malloc(20);
  a3 = (char *)malloc(30);

  a4 = (char *)malloc(40);
  a5 = (char *)malloc(50);

  a6 = (char *)malloc(60);
  free(a6);
  free(a5);
  free(a4);
  free(a3);

  free(a2);
  free(a1);



  return 0;
}

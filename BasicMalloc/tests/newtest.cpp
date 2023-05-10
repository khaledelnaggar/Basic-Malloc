#include <stddef.h>
#include <stdlib.h>
#include "assert.h"
#include "object.h"

#define SOME_CONSTANT 'l'
#define ANOTHER_CONSTANT 'm'

#define SOME_CONSTANT_2 0x9999999
#define ANOTHER_CONSTANT_3 0x666

int i = 0;
int main()
{

  for (size_t i = 0; i < 1000; i++)
  {
    /* code */
    object *x1 = new object(1, 2, 3, 4, 5);
    object *x2 = new object(1, 2, 3, 4, 5);
    object *x3 = new object(1, 2, 3, 4, 5);
    object *x4 = new object(1, 2, 3, 4, 5);
    object *x5 = new object(1, 2, 3, 4, 5);
    object *x6 = new object(1, 2, 3, 4, 5);
    object *x7 = new object(1, 2, 3, 4, 5);

    delete (x7);
    delete (x6);
    delete (x4);
    delete (x1);
    delete (x3);
    delete (x2);

    object *x8 = new object(1, 2, 3, 4, 5);
    object *x9 = new object(1, 2, 3, 4, 5);
    object *x10 = new object(1, 2, 3, 4, 5);
    object *x11 = new object(1, 2, 3, 4, 5);

    delete (x8);
    delete (x10);
    delete (x9);

    delete (x5);
    delete (x11);
  }

  return 0;
}

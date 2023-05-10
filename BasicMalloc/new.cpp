#include <new> 
#include <stdlib.h> 
#include "memory.h"

namespace snp
{




// TODO Student
void *Memory::_new(size_t size)
{
  try
  {
    void *return_ptr = malloc(size);
    if (return_ptr != nullptr)
      return return_ptr;
    else
    {
      throw std::bad_alloc{};
    }

  }
  catch(std::bad_alloc&)
  {

  }
  (void)size;
  return 0;
}

// TODO Student
void Memory::_delete(void * p) 
{ 

  free(p);
  (void) p;
} 





}

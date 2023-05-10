#ifndef SNP_MEMORY_H_
#define SNP_MEMORY_H_

#include "unistd.h"
#include "stdlib.h"

namespace snp {
  class Memory
  { 
  public:
    // DO NOT MODIFY
    static void *calloc(size_t num_memb, size_t size_each);
    static void *malloc(size_t size);
    static void free(void *ptr);

    // DO NOT MODIFY
    static void *_new(size_t size);
    static void _delete(void *ptr);
    static size_t malloc_called_count() noexcept;
    static size_t used_blocks_count() noexcept;

    // Feel free to add new members to this class.

    static void increase_malloc_called_counter();

    //private:
    static size_t malloc_called_counter;
    static void* start_heap_address;
    static void* end_seg_address;
    static void*meta_data_linker;
    static void check_buffer_overrun(void *left_node, void *curr_node, void *right_node);
    static void split_block(void*curr_block, size_t requested_size);
  };

  // DO NOT MODIFY
  void* sbrk(intptr_t __delta);
  int brk(void* __addr);
}




// DO NOT CHANGE ANYTHING BELOW THIS LINE
void *malloc(size_t size) noexcept;
void *calloc(size_t num_memb, size_t size_each) noexcept;
void free(void *ptr) noexcept;
void* operator new(size_t size);
void operator delete(void *address ) noexcept;
void* operator new[] ( size_t size );
void operator delete[] ( void* address ) noexcept;

#endif /* SNP_MEMORY_H_ */
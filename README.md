## Basic Dynamic Memory Implementation
This Repo contains some basic Implemenation of Dynamnic memory management all done in the user space, namely (malloc, calloc, free, operator new and operator delete)

## Malloc
The naive approcch was to have a linked list of Nodes which each of them is a struct namely the meta data to the Memory block. The struct holds data such as the size of the block a pointer to the next Memory block and a boolean flag to indicating if the block is in use or not. When allocating a block of memory we first check whether we have a free block in our the list or not. if not we expand the sbrk further and make new Memory block. in case we find a block that is bigger than the needed block we split this block into 2 blocks for the sake of minimizing the total number of syscalls.

## Free
Freeing a block of memory that we allocated using malloc, calloc on it. It is also considered the main part that manages our list and does the error-handling. When freeing a block of memory we always check the neighbour blocks if we can merge any with the others while looping over the list. We also do Take care of overflows in case Memory corruption detected at some point. In case of errors detected we exit the process with exit code -1.
 and delete
## Calloc
The calloc() function allocates memory for the required size and returns a pointer to the allocated memory.  The memory is set to zero.  If nmemb or size is 0,  then  calloc()  returns

## New (operator)
It allocates memory for an object on the heap py passing the size of the object to malloc that does the dynamic memory allocation and returns the pointer the memory block, if it fails then we return a nullptr back or we throw and exception std::bad_alloc.

## Delete (operator)
It frees the memory block we allocated using the new operator, so basically in this implementation it is simplly a call to free(ptr) on the pointer.

### Note
This implementation is not optimal and might not be covering all edge cases. It is also not meant to be thread safe.

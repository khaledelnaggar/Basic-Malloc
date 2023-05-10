## Basic Dynamic Memory Implementation

This Repo contains some basic Implemenation of Dynamnic memory management all done in the user space, namely (malloc, calloc, free, operator new and operator delete)

##Malloc
The naive approcch was to have a liked list of Nodes which each of them is a struct namely the meta data to the Memory block. The struct holds data such as the size of the block a pointer to the next Memory block and if the block in use or not. When allocating a block of memory we first check wethere we have a free block in our the list or not. if not we expand the sbrk further and make new Memory block. in case we find a block that is smaller than the needed block we split this block into 2 blocks for the sake of minimizing the total number of syscalls.

##Free
the main part of managing and error-checking the Implementation. When freeing a block of memory we always check the neighbour blocks if we can merge any of with the others while looping over the list.
we also do Take care of overflows in case Memory corruption detected at some point. In case of errors detected we exit the process with exit code -1.

#calloc, new and delete
To be ADDED

// This should have all the imports you need. Please don't import iostream.
#include <pthread.h>
#include "memory.h"
#include <new>
#include "string.h"

namespace snp
{

  typedef struct Node
  {
    size_t node_size;
    bool occupied_node;
    Node *next;
  } Node;

  size_t Memory::malloc_called_counter = 0;
  void *Memory::start_heap_address = nullptr;
  void *Memory::end_seg_address = nullptr;
  void *Memory::meta_data_linker = nullptr;
  bool initialized = false;

  void *Memory::malloc(size_t size)
  {
    increase_malloc_called_counter();
    if((size == 0 ))
      return nullptr;

    void *return_ptr = nullptr;
    Node* data_struct;
    size = size + sizeof(Node);

    if (!initialized)
    {
      start_heap_address = (char *)sbrk(0);

      if (sbrk((intptr_t)size) == ((void *)-1))
        return nullptr;

      end_seg_address = ((char *)start_heap_address + size);

      data_struct = (Node *)start_heap_address;

      ((Node *)data_struct)->node_size = size; 
      ((Node *)data_struct)->next = nullptr;
      ((Node *)data_struct)->occupied_node = true;

      meta_data_linker = data_struct;

      return_ptr = ((char *)start_heap_address + sizeof(Node));
      initialized = true;

      return return_ptr;
    }

    Node* left_node = nullptr;
    Node *curr_node = (Node *)start_heap_address;
    while((curr_node != nullptr) && (curr_node < end_seg_address))
    {
      if ((left_node != nullptr) && (curr_node->next != nullptr))
        check_buffer_overrun(left_node, curr_node, curr_node->next);
      else if (curr_node->next != nullptr)
        check_buffer_overrun(nullptr, curr_node, curr_node->next);
      else if ((left_node != nullptr))
        check_buffer_overrun(left_node, curr_node, nullptr);

      if ((size <= curr_node->node_size) && !curr_node->occupied_node && (return_ptr==nullptr))
      {

        if((size < curr_node->node_size) && ((curr_node->node_size - size) >= 25))
        {
          split_block(curr_node, size);
          
        }
        curr_node->occupied_node = true;
        return_ptr = (void *)((char *)curr_node + sizeof(Node));
      }
      left_node = curr_node;
      curr_node = curr_node->next;
    }

    if(return_ptr != nullptr)
      return return_ptr;

  if (sbrk((intptr_t)size) == (void *)-1)
    return nullptr;

  data_struct = (Node *)(end_seg_address);


  end_seg_address = ((char *)data_struct) + size;   // move end address


  if(left_node != nullptr)
  {
    ((Node *)left_node)->next = ((Node *)data_struct);          // remember meta data are in the begining
  }

  // initializing the meta data
  data_struct->next = nullptr;
  data_struct->occupied_node = true;
  data_struct->node_size = size;

  return_ptr = ((char *)data_struct + sizeof(Node));

  (void)size;
  return return_ptr;

  }



  void Memory::free(void *ptr)
  {
    if(ptr == nullptr)
      return;

    if (((char *)ptr - sizeof(Node)) > end_seg_address ||
        (((char *)ptr - sizeof(Node)) < start_heap_address) ||
        (start_heap_address == end_seg_address))
      exit(-1);


    // this flag is to keep track of 
    // where the prev node should point to
    bool flag = false;             

    Node *curr_node = (Node *)start_heap_address;
    Node *left_node = curr_node;
    Node *right_node = nullptr;
    size_t counter = 0;

    if (curr_node->next != nullptr)
      right_node = curr_node->next;
    for (; curr_node != nullptr; curr_node = curr_node->next)
    {

      if ((left_node != curr_node) && (right_node != nullptr) && (right_node != curr_node)) // middle
        check_buffer_overrun(left_node, curr_node, right_node);
      else if ((left_node == curr_node) && (right_node != nullptr)) // top left and 2 elements list
        check_buffer_overrun(nullptr, curr_node, right_node);
      else if ((curr_node->next == nullptr) && (left_node != curr_node)) // top right node
        check_buffer_overrun(left_node, curr_node, nullptr);

      counter++;
      if (curr_node != ((Node *)start_heap_address))
      {
        flag = true;
      }

      if (((((char *)curr_node + sizeof(Node)) == ((ptr))) && curr_node->occupied_node))      // found the pointer to be freed?
      {
        if (right_node != nullptr)
        {

          if (flag && !left_node->occupied_node && !right_node->occupied_node)
          {

            if (right_node->next != nullptr) // merge right with left with current all together !!!
            {

              left_node->node_size += ((curr_node->node_size));
              Node *tmp1 = curr_node;
              left_node->next = right_node;
              tmp1->next = nullptr;

              left_node->node_size += ((right_node->node_size));
              Node *tmp2 = right_node;
              left_node->next = right_node->next;
              tmp2->next = nullptr;
              return;
            }
          }
          if (!right_node->occupied_node) // merge current with right
          {

            curr_node->node_size += (right_node->node_size);
            Node *tmp = right_node;
            curr_node->next = right_node->next;
            tmp->next = nullptr;
            curr_node->occupied_node = false;
            return;
          }
        }
        if (flag && !left_node->occupied_node) // merge left with current
        {

          if ((right_node != curr_node)) // make sure curr is not the last node
          {

            left_node->node_size += (curr_node->node_size);
            Node *tmp = curr_node;
            left_node->next = right_node;
            tmp->next = nullptr;
            return;
          }
          else // otherwise we risize the end of the heap
          {
            // check if not a single-element list loop to get node before the left
            // so we can elemenate the pointer to left_node

            if ((curr_node->next == nullptr) && (counter > 2))
            {
              Node *n;
              for (n = (Node *)start_heap_address; n->next != left_node; n = n->next);

              meta_data_linker = n;
              n->next = nullptr;
            }
            else
            {
              meta_data_linker = start_heap_address; // at this point it's an empty list
            }
            
            size_t dec_size = (curr_node->node_size + left_node->node_size);

            left_node->next = nullptr;
            end_seg_address = ((char *)end_seg_address) - (dec_size);

            if(sbrk(-dec_size) == ((void*)-1))
              exit(-1);

            return;
          }
        }

        if (curr_node->next != nullptr)
        {
          curr_node->occupied_node = false;
          return;
        }
        else
        {
          size_t dec_size = curr_node->node_size;

          if (curr_node != left_node)     // checking if the node is the first in the list
          { 
            left_node->next = nullptr;
            meta_data_linker = left_node;
          }
          end_seg_address = ((char *)end_seg_address) - (dec_size);

          if (sbrk(-dec_size) == (void *)-1)
            exit(-1);
            
          meta_data_linker = start_heap_address;

          return;
        }
      }
      else if((((char *)curr_node + sizeof(Node)) == ptr) && !curr_node->occupied_node) // check if it's a double free
        exit(-1);

      if (flag)
        left_node = left_node->next;
      if ((right_node != nullptr) && (right_node->next != nullptr))
        right_node = right_node->next;

    }
    //  (void) ptr;
    exit(-1);
  }

  void *Memory::calloc(size_t num_memb, size_t size_each)
  {

    if ((num_memb == 0) || (size_each == 0))
      return nullptr;
    void *return_ptr = (void *)malloc(num_memb * size_each);

    memset(return_ptr, 0 , num_memb*size_each);

    (void)num_memb;
    (void)size_each;
    return return_ptr;
  }

  size_t Memory::malloc_called_count() noexcept
  {
    return malloc_called_counter;
    // return 0;
  }

  size_t Memory::used_blocks_count() noexcept
  {
    size_t n_blocks = 0;
    for (Node *curr_node = (Node *)start_heap_address; curr_node != nullptr; curr_node = curr_node->next)
    {
      if (curr_node->occupied_node)
      {
        n_blocks++;
      }
    }

    return n_blocks;
  }

  void Memory::increase_malloc_called_counter()
  {
    malloc_called_counter++;
  }

  void Memory::check_buffer_overrun(void *left_node, void *curr_node, void *right_node)
  {

    if ((left_node != nullptr) && (right_node != nullptr))
    {
      if ((((char *)left_node + ((Node *)left_node)->node_size) != (char *)curr_node) 
      || (((char *)curr_node + ((Node *)curr_node)->node_size) != (char *)right_node))
        exit(-1);
    }

    if ((right_node == nullptr) && (((char *)left_node + ((Node *)left_node)->node_size) != (char *)curr_node))
      exit(-1);

    if ((left_node == nullptr) && (((char *)curr_node + ((Node *)curr_node)->node_size) != (char *)right_node))
      exit(-1);
    if((((Node*)curr_node)->next == nullptr) 
      && (((char*)curr_node +((Node*)curr_node)->node_size) != end_seg_address))
     exit(-1);

  }

  void Memory::split_block(void* node, size_t size)
  {
    Node* curr_node = ((Node*) node);
    Node *new_node;
    size_t prev_size = curr_node->node_size;
    curr_node->node_size = size;
    new_node = (Node *)((char *)curr_node + curr_node->node_size);
    new_node->next = curr_node->next;
    new_node->node_size = prev_size - size;
    new_node->occupied_node = false;
    curr_node->next = new_node;
  }


}


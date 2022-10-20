#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define SHARED_PTR shared_ptr __attribute__((cleanup(shared_ptr_cleanup))) 

typedef struct shared_ptr
{
    void  *ptr;

} shared_ptr;

struct _pointer
{
    size_t _cref;
    void (*_destructor) (void*);
    uint8_t _data[0];
};

shared_ptr new_int(int value)
{
    struct _pointer* pointer = malloc(sizeof(struct _pointer) + sizeof(value));
    shared_ptr sp = {0};

    if (pointer)
    {
        pointer->_cref = 1;
        pointer->_destructor = 0;

        sp.ptr = pointer + 1;
    }

    return sp; 
}

inline shared_ptr shared_ptr_copy(shared_ptr p)
{
    ++((struct _pointer*)(p.ptr) - 1)->_cref;
    return p;
}

inline size_t shared_ptr_cref(shared_ptr p)
{
    return ((struct _pointer*)(p.ptr) - 1)->_cref;
}

void shared_ptr_cleanup(shared_ptr *p)
{
    struct _pointer* pointer = ((struct _pointer*)(p->ptr) - 1);

    if (0 == --(pointer->_cref))
    {
        printf("ref count is zero! freeing\n");
        
        if (pointer->_destructor) 
            pointer->_destructor(p->ptr);
        
        bzero(pointer, sizeof(*pointer));
        free(pointer);
    } 
}

void func(shared_ptr p)
{
    SHARED_PTR resource = shared_ptr_copy(p);
    SHARED_PTR p2 = new_int(*(int*)resource.ptr);
    
    printf("func::(p) ref count: %lu \n",        shared_ptr_cref(p));       
    printf("func::(resource) ref count: %lu \n", shared_ptr_cref(p));       
    printf("func::(p2) ref count: %lu \n",      shared_ptr_cref(p2));       
}

int main (int argc, char *argv[])
{
    SHARED_PTR p = new_int(32);
    printf("main::p ref count: %lu \n", shared_ptr_cref(p));     
    
    func(p);
    
    printf("main::p ref count: %lu \n", shared_ptr_cref(p));       

    return 0;
}

#include <stdio.h>
#include <stdlib.h> 

typedef struct Object_inst Object;
typedef void (*mVi) (Object *this);
typedef Object* (*mIi) (Object *this);
typedef Object* (*mIv) (void);

struct Object_class
{
    mIv new;
    mIi init;
    mIi uninit;
    mVi delete;
    mVi method1;
};

struct Object_inst
{
    const struct Object_class *class;
};

static void method1(Object* this)
{
    printf("method1 on %p\n", this);
}

static Object* Object_alloc(void);
static Object* Object_ctor(Object* this);
static Object* Object_dtor(Object* this);
static void Object_free(Object* this);

struct Object_class Object_class = {
    .new = Object_alloc,
    .init = Object_ctor,
    .uninit = Object_dtor,
    .delete = Object_free,
    .method1 = method1
};

static Object* Object_alloc(void)
{
    return (Object*) malloc(sizeof(Object));
}

static Object* Object_ctor(Object* this)
{
    this->class = &Object_class;
    return this;
}

static Object* Object_dtor(Object* this)
{
    this->class = 0;
    return this;
}

static void Object_free(Object* this)
{
    free(this);
}

#define MCALL(obj, method) (obj).class->method(&obj)
#define DEL(ptr) ptr->class->delete(ptr->class->uninit(ptr))
#define NEW(type, args...) type##_class.init(type##_class.new(), ##args)

int main (int argc, char *argv[])
{
    Object inst;
    Object_class.init(&inst); // call to an Object constructor 
    Object *ref = &inst;
    
    MCALL(inst, method1);
    MCALL(*ref, method1);

    Object* ptr = NEW( Object );
    MCALL(* ptr, method1); 
    DEL(ptr);
    
    return 0;
}

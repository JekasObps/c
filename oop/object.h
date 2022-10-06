#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Object 
{
    const uint32_t id;
} Object;

Object * ObjectCreate(size_t data_sz);
void ObjectDestroy(Object *obj);
uint32_t ObjectGetId(Object *obj);
uint32_t getObjectsCount();







#endif


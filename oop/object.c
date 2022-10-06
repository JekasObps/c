#include "object.h"

static uint32_t objectsCreated = 0;
static uint32_t objectsDeleted = 0;

Object * ObjectCreate(size_t data_sz)
{
    Object * obj = (Object *) malloc(data_sz);
       
    if (!obj)
        return NULL;

    *(uint32_t*)&obj->id = objectsCreated++;
    
    return obj;
}

void ObjectDestroy(Object *obj)
{
    ++objectsDeleted;
    free(obj);
}

uint32_t ObjectGetId(Object *obj)
{
    return obj->id;
}

uint32_t getObjectsCount()
{
    return objectsCreated - objectsDeleted;
}



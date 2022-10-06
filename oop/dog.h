#ifndef __DOG_H__
#define __DOG_H__
#include "animal.h"

#include <stdio.h>
typedef struct Dog 
{
    Animal base;
    const char * name;
} Dog;

void DogMakeSound(const Dog * const dog);

const char* DogGetName(const Dog * const dog);

Dog * DogCreate(const char * color, const char *name);
void DogDestroy(Dog * dog);



#endif /*__DOG_H__*/

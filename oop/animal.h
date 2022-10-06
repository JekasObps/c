#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "object.h"

typedef struct Animal
{
    Object o;
    void * vtable; /* only two virtual functions */
    const char * color;   
} Animal;

/* destroy function calls to a dtor of an object */
void AnimalBaseDestroy(Animal *);

/* empty stub function */
void AnimalBaseMakeSound(Animal *);

/* ctor to create new instance of an animal */
Animal * AnimalCreate(const char * color);

void AnimalInit(Animal * anim, const char * color);

/* virtual destructor points to a vtable */
void AnimalDestroy(Animal * anim); 

const char * AnimalGetColor(Animal * anim);

/* virtual polimorphic function */
void AnimalMakeSound(Animal * anim); 


#endif /*__ANIMAL_H__*/

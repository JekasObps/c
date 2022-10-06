#include "animal.h"


/* global vtable */
struct animal_vtable_t
{
    void (*destroy)(Animal *);
    void (*make_sound)(Animal *);
} 
animal_vtable = {
    AnimalBaseDestroy, 
    AnimalBaseMakeSound
};

void AnimalBaseDestroy(Animal *anim)
{
    ObjectDestroy((Object *) anim);
}

void AnimalBaseMakeSound(Animal *anim)
{
    // not implemented
}

void AnimalDestroy(Animal *anim)
{
    ((struct animal_vtable_t*)anim->vtable)->destroy(anim);
}

void AnimalInit(Animal *anim, const char * color)
{
    anim->color = color;
    anim->vtable = (void*)&animal_vtable;
}

Animal * AnimalCreate(const char * color)
{
    Animal *anim = (Animal*) ObjectCreate(sizeof(*anim));
    
    if (!anim)
        return NULL;
    
    AnimalInit(anim, color);
    return anim;
}

const char * AnimalGetColor(Animal *anim)
{
    return anim->color;
}

void AnimalMakeSound(Animal *anim)
{
    ((struct animal_vtable_t*)anim->vtable)->make_sound(anim);
}

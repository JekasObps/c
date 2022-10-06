#include "dog.h"

struct dog_vtable_t
{
    void (*destroy)(Dog *);
    void (*make_sound)(const Dog *const);
}
dog_vtable = {
    DogDestroy,
    DogMakeSound
};

Dog * DogCreate(const char * color, const char *name)
{
    Dog * dog = (Dog*) ObjectCreate(sizeof(*dog));
    
    if (!dog)
        return NULL;

    AnimalInit((Animal*)dog, color);
    dog->name = name;
    (dog->base).vtable = (void*)&dog_vtable;
    return dog;
}

void DogDestroy(Dog * dog)
{
    AnimalBaseDestroy((Animal*)dog);
}

void DogMakeSound(const Dog * const dog)
{
    puts("Whoof whoof...\n    Whoooof whooofff rrrrrarrgh!");
}

const char* DogGetName(const Dog * const dog)
{
    return dog->name;
}




#include "animal.h"
#include "cat.h"
#include "dog.h"

#include <stdio.h>


int main(int argc, char **argv)
{
#if 0
    Object * o1 = ObjectCreate(sizeof(*o1));

    printf("Objects total: %u\n", getObjectsCount());
    
    destroyObject(o1);
    
    printf("Objects total: %u\n", getObjectsCount());

    o1 = ObjectCreate(sizeof(*o1));
    
    printf("Object's id: %u\n", getId(o1));
    
    destroyObject(o1);
#endif

#if 0
    Animal * a1 = AnimalCreate("red");
    printf("Animal.color = %s\n", AnimalGetColor(a1));
    printf("Objects total: %u\n", getObjectsCount());
    
    AnimalDestroy(a1);
    

    Animal * a2 = (Animal*)CatCreate("brown-white", 1);
        
    AnimalMakeSound(a2);
    CatSetTakingNap((Cat*)a2, 0);
    AnimalMakeSound(a2);
    AnimalDestroy(a2);
    
    
    printf("Objects total: %u\n", getObjectsCount());
    
#endif

#if 1
    Animal* array[3];
    array[0] = (Animal*) CatCreate("gingi", 1);
    array[1] = (Animal*) DogCreate("grey","Boby");
    array[2] = (Animal*) CatCreate("white", 0);
    
    for (int i = 0; i < 3; ++i)
    {
        AnimalMakeSound(array[i]);
    }
    
    for (int i = 0; i < 3; i++)
    {
        AnimalDestroy(array[i]);
        array[i] = NULL;
    }
#endif


    return 0;
}

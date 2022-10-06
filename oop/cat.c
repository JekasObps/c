#include "cat.h"

struct cat_vtable_t
{
    void (*destroy)(Cat *);
    void (*make_sound)(const Cat *const);
}
cat_vtable = {
    CatDestroy,
    CatMakeSound
};

Cat * CatCreate(const char * color, char tn)
{
    Cat * cat = (Cat*) ObjectCreate(sizeof(*cat));
    
    if (!cat)
        return NULL;

    AnimalInit((Animal*)cat, color);
    cat->takingNap = tn;
    (cat->base).vtable = (void*)&cat_vtable;
    return cat;
}

void CatDestroy(Cat * cat)
{
    AnimalBaseDestroy((Animal*)cat);
}

void CatMakeSound(const Cat * const cat)
{
    if (CatIsTakingNap(cat))
    {
        puts("frfrfrfr...");
        return;
    }
    
    puts("myau myau myauu...");
}

char CatIsTakingNap(const Cat * const cat)
{
    return cat->takingNap;
}

void CatSetTakingNap(Cat * const cat, char tn)
{
    cat->takingNap = tn;    
}



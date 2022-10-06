#ifndef __CAT_H__
#define __CAT_H__
#include "animal.h"

#include <stdio.h>
typedef struct Cat 
{
    Animal base;
    char takingNap;
} Cat;

void CatMakeSound(const Cat * const cat);

void CatSetTakingNap(Cat *const cat, char tn);
char CatIsTakingNap(const Cat * const cat);

Cat * CatCreate(const char * color, char tn);
void CatDestroy(Cat * cat);



#endif /*__CAT_H__*/

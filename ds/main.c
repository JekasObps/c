#include "slist.h"
#include "dlist.h"

#include <stdio.h>


int print(float* data, void* param)
{
    (void)param;
    printf("node(%f)\n", *data);
    return 0;
}

int main(int argc, char** argv)
{
	SList *list = slist_new_mm(sizeof(float));

    
    SListIterator it = slist_push_front(list);
    *(float*) slist_iter_data(it) = 2.0f; 
    
    it = slist_push_front(list);
    *(float*) slist_iter_data(it) = 3.5f;

    it = slist_push_front(list);
    *(float*) slist_iter_data(it) = 9.3f;
    
	printf("list size : %lu\n", slist_count(list));
    slist_foreach(list, (user_apply)print, 0);


	slist_free(list);

    
    DList *dl = dlist_new_mm(sizeof(float));
    
    DListIterator dit = dlist_push_front(dl);
    *(float*) dlist_iter_data(dit) = 2.0f; 
    
    dit = dlist_push_front(dl);
    *(float*) dlist_iter_data(dit) = 3.5f;

    dit = dlist_push_front(dl);
    *(float*) dlist_iter_data(dit) = 9.3f;
    
	printf("list size : %lu\n", dlist_count(dl));
    dlist_foreach(dl, (user_apply)print, 0);
    
    dit = dlist_begin(dl);
    dit = dlist_iter_next(dlist_iter_next(dit));
    
    while (dlist_iter_is_valid(dit))
    {
        printf("reverse %f\n", *(float*)dlist_iter_data(dit));
        dit = dlist_iter_prev(dit);
    }

	dlist_free(dl);

	return 0;
}

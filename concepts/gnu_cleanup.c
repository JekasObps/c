#include <stdlib.h>
#include <string.h>

typedef struct data {
    float* f;
    char s[16];
} Data;

Data* data_new(void)
{
    return (Data*) malloc(sizeof(Data));
}

void data_del(Data *d)
{
    free(d);
}

void data_init(Data* data, float f, const char* s)
{
    data->f = (float*) malloc(sizeof(f));
    *data->f = f; 
    strncpy(data->s, s, 16);
}

void Data_destroy(Data* data)
{
    free(data->f);
}

#define SCOPE(type) type __attribute__((cleanup(type##_destroy)))

int main(int argc, char *argv[])
{
    SCOPE(Data) data;
    data_init(&data, 12.0f, "mozamby");

    return 0;
}

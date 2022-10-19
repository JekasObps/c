#include <stdio.h>


struct interface {
    int    _sentinel;
    int    _1;
    char*  _2;
    double _3;
};

void foo(struct interface *args)
{
    printf("%d, %s, %f\n", args->_1, args->_2, args->_3);
}

#define foo(...) \
    foo(&(struct interface){._sentinel=0, __VA_ARGS__})


int main (int argc, char *argv[])
{
    foo(._1 = 1, ._2 = "hello", ._3 = 1.2);

    return 0;
}

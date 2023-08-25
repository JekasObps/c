#include  <stdio.h>
#include <unistd.h>

int main()
{
    char *argv[] = {"data", NULL};
    if (fork()!=0)
    {
        // parent code 
        printf("Hello\n");

    }
    else
    {
        // child process
        execv("/bin/date", argv);
    }
    return 0;
}

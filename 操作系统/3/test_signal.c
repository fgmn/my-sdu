#include <stdio.h>
#include <signal.h>

void int_handler(void);

int main(int argc, char ** argv)
{
    sigset( SIGINT, int_handler);
    printf( "Process_pid=[%d]\n", getpid() );
    while(1);
    return 0;
}

void int_handler(void)
{
    printf("Get a SIGINT signal!\n"); 
}

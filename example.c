#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "spinner.h"

int
main(int argc, char **argv)
{
    spinner_t *s = spinner_new(32);
    s->delay = 100000;
    s->prefix = "Running ";
    s->suffix = " I'm a suffix";
    s->final_msg = "Complete!\n";

    spinner_start(s);

    sleep(5); // simulate some work

    spinner_stop(s);
    spinner_free(s);

    return 0;
}

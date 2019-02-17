#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../spinner.h"

int 
main(int argc, char **argv) 
{
	spinner_t *s = spinner_new(9);
	s->delay = 100000; 
	s->prefix = "Running ";
	s->final_msg = "\nAll done\n";
	spinner_start(s);
	sleep(10);           // simulate some work
	spinner_stop(s);
	spinner_free(s);
	return 0;
}

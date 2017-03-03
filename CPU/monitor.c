#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>


int main(int argc, char **argv)
{
	int res;
	while(1)
	{
		res = kill(10758, 0);
		printf("res=%d\n",res);
	}
}
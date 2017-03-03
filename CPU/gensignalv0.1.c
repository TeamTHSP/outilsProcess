#include <stdio.h>
#include <stdlib.h>
#include <libproc.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach_time.h>


void handler_signal(int);

int switcher = 0;

void handler_signal_1(int inutilisee)
{
	struct itimerspec spec;
	if(switcher)
	{
		spec.it_interval.tv_sec 	= 0;
		spec.it_interval.tv_nsec 	= 0;
		spec.it_value.tv_sec 		= 6;
		spec.it_value.tv_nsec 		= 0;
		timer_settime(timer_1,0,&spec, NULL);
	}
	else
	{
		spec.it_interval.tv_sec 	= 0;
		spec.it_interval.tv_nsec 	= 0;
		spec.it_value.tv_sec 		= 2;
		spec.it_value.tv_nsec 		= 0;
		timer_settime(timer_1,0,&spec, NULL);
	}
	switcher = 1;
}

int main(int argc, char const *argv[])
{
	timer_t timer_1;
	struct sigevent event_1;
	struct itimerspec spec_1;
	signal(SIGUSR1, handler_signal_1);

	event_1.sigev_notify = SIGEV_SIGNAL;
	event_1.sigev_signo   = SIGUSR1;

	spec_1.it_interval.tv_sec 	= 0;
	spec_1.it_interval.tv_nsec 	= 0;
	spec_1.it_value.tv_sec 		= 2;
	spec_1.it_value.tv_nsec 	= 0;

	timercreate(CLOCK_REALTIME, &event_1, &timer_1);
	timer_settime(timer_1, 0, &spec_1, NULL);





	while(1)
	{
		pause();			
	}
	

	return 0;
}



void handler_signal(int inutilisee)
{
	//struct proc_taskallinfo tiDepart, tiFin;
	long tDepart,tFin;
	struct rusage r;

	switch(5)
	{
		case 0: //on a passé les 500ms
			//hasToSleep = 0;
			// getrusage(RUSAGE_SELF, &r);
			// tDepart = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			
			break;
		case 1 : // On est au bout de 100ms
			//hasToSleep = 1 ;
			// getrusage(RUSAGE_SELF, &r);
			// tFin = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			// timeToSleep = 400;
			// if(nb_of_timer%10000 == 1)
			// {
			// 	printf("temps passé sur 100us : %lu\n", tFin-tDepart );
			// }
			break;
		
		default : 
			break;
	} 
}
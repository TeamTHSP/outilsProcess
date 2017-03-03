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

// fill itimerval struct
void fillIS(struct itimerval *struc, int sec, int usec)
{
	struc->it_interval.tv_sec = sec;
	struc->it_interval.tv_usec = usec;

	struc->it_value = struc->it_interval ; 
}



int timerXOR = 0;

//struct itimerval* tabTimerVal[2];
struct itimerval timer1, timer2;

long tDepart, tFin;
struct timeval tD, tF;

struct rusage r;

clock_t ta, tb;

int main(int argc, char const *argv[])
{

	signal(SIGALRM, handler_signal);
	
	//struct itimerval timer1, timer2;
	fillIS(&timer1, 0, 100);
	//fillIS(&timer2, 4, 0);
	// timer1.it_interval.tv_sec = 0;
	// timer1.it_interval.tv_usec = 100;

	// timer1.it_value = timer1.it_interval ; 

	// timer2.it_interval.tv_sec = 0;
	// timer2.it_interval.tv_usec = 400;

	// timer2.it_value = timer2.it_interval ; 


	// tabTimerVal[0] = &timer1;
	// tabTimerVal[1] = &timer2;

	setitimer(ITIMER_REAL, &timer1, NULL);
	getrusage(RUSAGE_SELF, &r);
	tDepart = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
	struct timeval depart, fin ;

	while(1)
	{
		if(timerXOR)
		{
			usleep(400000);
		}	
	}
	

	return 0;
}



void handler_signal(int inutilisee)
{
	//printf("hell \n");
	int usec = 100;
	timerXOR ^= 1;
	switch(timerXOR)
	{
		case 0 : // va se mettre en activite
			timer1.it_interval.tv_usec = usec;
			timer1.it_value = timer1.it_interval ; 
			setitimer(ITIMER_REAL, &timer1, NULL);
			gettimeofday(&tD,NULL) ; 
			
			getrusage(RUSAGE_SELF, &r);
			tDepart = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;

			break;

		case 1 : // va se mettre en sommeil
			gettimeofday(&tF,NULL) ; 
			long differenceTemps = (tF.tv_sec - tD.tv_sec) * 1000000 + (tF.tv_usec - tD.tv_usec);
			getrusage(RUSAGE_SELF, &r);
			tFin = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			//printf("temps:%ld\n",tFin - tDepart );
			
			usec = 400 - (differenceTemps - (tFin - tDepart))*(1 + (tFin - tDepart)/(double)differenceTemps);
			//usec = 400 - (100 - (tFin - tDepart))*(1 + (tFin - tDepart)/100.0);

			timer1.it_interval.tv_usec = usec;

			timer1.it_value = timer1.it_interval ; 

			setitimer(ITIMER_REAL, &timer1, NULL);
			
			//printf("temps: %lu\n",differenceTemps );
			//printf("usec=%d\n", usec);
			break;

		default:
			break;
	}

	//printf("usec=%d\n", usec);
}
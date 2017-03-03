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


#define ORWL_NANO (+1.0E-9)
#define ORWL_GIGA UINT64_C(1000000000)

struct timespec clock_gettime(void) {
  // be more careful in a multithreaded environement
  struct timespec t;
  double time = mach_absolute_time();
  t.tv_sec = time * ORWL_NANO;
  t.tv_nsec = time - (t.tv_sec * ORWL_GIGA);
  return t;
}

void handler_signal(int);


double diffclock(struct timespec a,struct timespec b)
{
	return ((double)b.tv_sec + ORWL_NANO*b.tv_nsec - (double)a.tv_sec + ORWL_NANO*a.tv_nsec);

}

int hasToSleep  = -1;
int nb_of_timer = 0;
int timeToSleep = 0;
int timerXOR = 0;

struct itimerval tabTimerVal[10];

int main(int argc, char const *argv[])
{
	// while(1)
	// {
	// 	struct timespec tstart={0,0}, tend={0,0};
	// 	clock_gettime(CLOCK_REALTIME, &tstart);
	// 	clock_gettime(CLOCK_REALTIME,&tend);
	// 	//printf("on démarre\n\n");
	// 	while( diffclock(tstart, tend)/1000 <= 1 ){
	// 		//printf("%f\n",diffclock(tstart, tend)/1000);
	// 		//printf("difftime: %f\n",difftime(end_t, start_t));
	// 		clock_gettime(CLOCK_REALTIME,&tend);
	// 		//printf("%ld, %ld, %f \n",start_t, end_t, difftime(end_t, start_t)/1000.0 );
	// 	}
	// 	//printf("fin de la boucle while\n");
	// 	usleep(4000);
	// 	//printf("fin du sleep\n");

	// }
	// le temps sera exprimé en micro sec

/* Gere les parametres en entrée

	// double temps_actif, temps_sleep;
	// if(argc == 5 )
	// {
	// 	if(strcmp(argv[1], "-a") == 0)
	// 	{
	// 		temps_actif = atof(argv[2]);
	// 	}
	// 	if(strcmp(argv[3], "-s") == 0)
	// 	{
	// 		temps_sleep = atof(argv[4]);
	// 	}
	// }
	// else 
	// {
	// 	printf("Le nombre d'argument n'est pas correct\n");
	// 	exit(1);
	// }

*/

	//printf("%f, %f\n",temps_sleep, temps_actif );

	// while(1)
	// {
	// 	clock_t tstart, tend;
	// 	tstart = tend = clock();
	// 	//printf("on démarre\n\n");
	// 	while( (double)(tend - tstart)/CLOCKS_PER_SEC <= temps_actif/1000000.0 ){
	// 		//printf("%f\n",(double)(tend - tstart) );
	// 		//printf("%f\n",diffclock(tstart, tend)/1000);
	// 		//printf("difftime: %f\n",difftime(end_t, start_t));
	// 		tend = clock();
	// 		//printf("%ld, %ld, %f \n",start_t, end_t, difftime(end_t, start_t)/1000.0 );
	// 	}
	// 	//printf("fin de la boucle while\n");
	// 	usleep(temps_sleep);
	// 	//printf("fin du sleep\n");

	// }


	signal(SIGALRM, handler_signal);
	struct itimerval timer1, timer2;

	timer1.it_interval.tv_sec = 0;
	timer1.it_interval.tv_usec = 100;

	timer1.it_value = timer1.it_interval ; 

	timer2.it_interval.tv_sec = 0;
	timer2.it_interval.tv_usec = 400;

	timer2.it_value = timer2.it_interval ; 


	tabTimerVal[0] = timer1;
	tabTimerVal[1] = timer2;

	setitimer(ITIMER_REAL, &tabTimerVal[0], NULL);
	struct timeval depart, fin ;

	while(1)
	{
		if(timerXOR)
		{
			usleep(400000);
		}
		// if(hasToSleep){
		// 	hasToSleep = 0;
		// 	//gettimeofday(&depart,NULL) ; 
		// 	
		// 	printf("after sleep\n");
		// 	//gettimeofday(&fin,NULL) ; 
		// 	//long differenceTemps = (fin.tv_sec - depart.tv_sec) * 1000000 + (fin.tv_usec - depart.tv_usec);
		// 	//printf("temps du sleep: %lu\n",differenceTemps );


		// }	
	}
	

	return 0;
}



void handler_signal(int inutilisee)
{
	//struct proc_taskallinfo tiDepart, tiFin;
	//printf("going in handler\n");
	timerXOR ^= 1;
	setitimer(ITIMER_REAL, &tabTimerVal[timerXOR], NULL);


	
	long tDepart,tFin;
	struct rusage r;
/*
	switch(nb_of_timer%5)
	{
		case 0: //on a passé les 500ms
			hasToSleep = 0;
			// getrusage(RUSAGE_SELF, &r);
			// tDepart = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			
			break;
		case 1 : // On est au bout de 100ms
			hasToSleep = 1 ;
			// getrusage(RUSAGE_SELF, &r);
			// tFin = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			
			// if(nb_of_timer%10000 == 1)
			// {
			// 	printf("temps passé sur 100us : %lu\n", tFin-tDepart );
			// }
			break;
		
		default : 
			break;
	}
	nb_of_timer++ ; */
}
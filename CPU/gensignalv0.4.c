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

struct timeval tDepartH, tFinH ;
struct timeval tDepartP, tFinP ;
struct timeval tDepartSleep, tFinSleep ;

struct rusage r;
long differenceTempsCal, differenceTempsCPU;

long tHDepartCPU, tHFinCPU, tPDepartCPU, tPFinCPU;
long temps_sleep = 800;

int main(int argc, char const *argv[])
{
	gettimeofday(&tDepartH, NULL) ;
	gettimeofday(&tFinH, NULL) ; 
	getrusage(RUSAGE_SELF, &r);
	tHDepartCPU = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
	tHFinCPU = tHDepartCPU;
	while(1)
	{
		gettimeofday(&tDepartP, NULL) ;
		getrusage(RUSAGE_SELF, &r);
		tPDepartCPU = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
		differenceTempsCal = 0;
		while(differenceTempsCal < 200 )
		{
			gettimeofday(&tFinP,NULL) ; 

			getrusage(RUSAGE_SELF, &r);
			tPFinCPU = 1000000*r.ru_utime.tv_sec + r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			
			differenceTempsCal = (tFinP.tv_sec - tDepartP.tv_sec) * 1000000 + (tFinP.tv_usec - tDepartP.tv_usec);
		}

		usleep(MAX(temps_sleep,0));

		gettimeofday(&tFinH,NULL) ; 
		tHFinCPU = tPFinCPU;

		differenceTempsCal = (tFinH.tv_sec - tDepartH.tv_sec) * 1000000 + (tFinH.tv_usec - tDepartH.tv_usec);
		differenceTempsCPU = tHFinCPU - tHDepartCPU;

		temps_sleep = 800 + (0.8*(differenceTempsCal) - (differenceTempsCal - differenceTempsCPU)) ;
	}
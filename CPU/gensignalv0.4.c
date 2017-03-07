#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


#define MAX(A,B) (B > A ? B : A)

struct timeval tDepartH, tFinH, tDepartP, tFinP;
struct rusage r;
long differenceTempsCal, differenceTempsCPU, tHDepartCPU, tHFinCPU, tPDepartCPU, tPFinCPU;
int temps_sleep, temps_travail;
double ratio; 


int main(int argc, char const *argv[])
{

	int pct = atoi(argv[1]);
	temps_travail = pct * 10;
	temps_sleep	  = 1000 - temps_travail;
	ratio = temps_sleep / 1000.0;

	gettimeofday(&tDepartH, NULL) ;
	gettimeofday(&tFinH, NULL) ; 
	getrusage(RUSAGE_SELF, &r);
	tHDepartCPU = 1000000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
	tHFinCPU = tHDepartCPU;

	while(1)
	{
		gettimeofday(&tDepartP, NULL) ;
		getrusage(RUSAGE_SELF, &r);
		tPDepartCPU = 100000*r.ru_utime.tv_sec +  r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
		differenceTempsCal = 0;

		while(differenceTempsCal < temps_travail )
		{
			gettimeofday(&tFinP, NULL) ; 

			getrusage(RUSAGE_SELF, &r);
			tPFinCPU = 1000000*r.ru_utime.tv_sec + r.ru_utime.tv_usec + 1000000*r.ru_stime.tv_sec +  r.ru_stime.tv_usec;
			
			differenceTempsCal = (tFinP.tv_sec - tDepartP.tv_sec) * 1000000 + (tFinP.tv_usec - tDepartP.tv_usec);

		}

		usleep(MAX(temps_sleep, 0));

		gettimeofday(&tFinH, NULL) ; 
		tHFinCPU = tPFinCPU;

		differenceTempsCal = (tFinH.tv_sec - tDepartH.tv_sec) * 1000000 + (tFinH.tv_usec - tDepartH.tv_usec);
		differenceTempsCPU = tHFinCPU - tHDepartCPU;

		temps_sleep = temps_sleep + (ratio * (differenceTempsCal) - (differenceTempsCal - differenceTempsCPU)) ;
	}

	return 0;
}
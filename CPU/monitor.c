#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <libproc.h>
#include <unistd.h>

 #define ASCII_ESC 27

int main(int argc, char **argv)
{
	int res;
	struct proc_taskallinfo tiDepart, tiFin;
	struct timeval depart, fin ; 
	long totalTimeCPU, differenceTemps, totalUserTime, totalSystemTime = 0 ;
	double cpuUsage;
	char isRunning;
	unsigned char r;
	char posX[100];

	int pid = atoi(argv[1]);

	FILE* fd = fopen("/Volumes/Matt's Public Folder/flux.txt","w");

	while(1)
	{
		proc_pidinfo(pid, PROC_PIDTASKALLINFO, 0, &tiDepart, sizeof(tiDepart));
		gettimeofday(&depart,NULL) ; 
		usleep(10000);
		gettimeofday(&fin,NULL) ; 
		proc_pidinfo(pid, PROC_PIDTASKALLINFO, 0, &tiFin, sizeof(tiFin));

		differenceTemps = (fin.tv_sec - depart.tv_sec) * 1000000 + (fin.tv_usec - depart.tv_usec);

		totalUserTime   = (tiFin.ptinfo.pti_total_user - tiDepart.ptinfo.pti_total_user ) /1000; 
		totalSystemTime = (tiFin.ptinfo.pti_total_system  - tiDepart.ptinfo.pti_total_system ) /1000;

		totalTimeCPU = totalUserTime + totalSystemTime ; 

		cpuUsage = 100 * (double)(totalTimeCPU)/(double)(differenceTemps);

		//printf("%d\n", posX);
		res = (int)cpuUsage;
		r = (unsigned char)cpuUsage;
		//printf("%c, %d, %f, %d\n",r, r, cpuUsage, res);
		//printf("%d\n",r );
		fseek(fd, 0, SEEK_SET);
		fprintf(fd, "%c", r);
		


		//printf("%d", isRunning);
		
	}
	fclose(fd);
}
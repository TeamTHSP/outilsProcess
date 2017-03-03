#include <errno.h>
#include <stdio.h>
#include <libproc.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#define TEMPS_SLEEP  5000000
#define NICE 		 20

char **eclaterChaine(const char *s, char separateur);

struct sPidInfo {
	pid_t pid;
	struct proc_taskallinfo tiDepart;
	struct proc_taskallinfo tiFin;
	double cpuUsage;
	double pct;
};

void calcCPU(struct sPidInfo tPid[], int nbPids)
{
	//struct proc_taskallinfo **tiDepart, tiFin;
	struct timeval depart, fin ; 
	struct timespec tsleep ; 

	long differenceTemps, totalUserTime, totalSystemTime = 0 ;
	int bytes;

	tsleep.tv_sec = 0; 
	tsleep.tv_nsec = TEMPS_SLEEP;
	
	for (int i = 0; i<nbPids; i++)
	{
		bytes = proc_pidinfo(tPid[i].pid, PROC_PIDTASKALLINFO, 0, &tPid[i].tiDepart, sizeof(tPid[i].tiDepart));
	}
	gettimeofday(&depart,NULL) ; 	
	nanosleep(&tsleep, NULL) ; 
	gettimeofday(&fin, NULL) ;
	differenceTemps = (fin.tv_sec - depart.tv_sec) * 1000000 + (fin.tv_usec - depart.tv_usec);
	for (int j = 0; j<nbPids; j++)
	{
		bytes = proc_pidinfo(tPid[j].pid, PROC_PIDTASKALLINFO, 0, &tPid[j].tiFin, sizeof(tPid[j].tiFin));
		
		totalUserTime = (tPid[j].tiFin.ptinfo.pti_total_user  - tPid[j].tiDepart.ptinfo.pti_total_user ) /1000; 
		totalSystemTime = (tPid[j].tiFin.ptinfo.pti_total_system  - tPid[j].tiDepart.ptinfo.pti_total_system ) /1000;

		long totalTime = totalUserTime + totalSystemTime ; 
		tPid[j].cpuUsage += (double)(totalTime)/(double)(differenceTemps);
	}
}

void setNice(struct sPidInfo tPid[], nbPids)
{
	for(int i=0;i<nbPids;i++)
	{
		setpriority(tPid[i].pid,0,NICE);
	}
}





int main(int argc, char **argv)
{
	double i = 1;
	//double sum[100];
	double cpuUsage;
	struct sPidInfo tPid[1000];
	char **tPct;
	char **tempPid;


	if (argc > 1)
	{
		int index = 1;
		if (strcmp(argv[index], "-pid") == 0)
		{
			tempPid = eclaterChaine(argv[++index], ',');
		}
		else
		{
			tempPid = malloc(sizeof(tPid)*1);
			tempPid[0] = argv[index];
		}

		if (strcmp(argv[++index], "-pct") == 0)
		{
			tPct = eclaterChaine(argv[++index], ',');
		}
		else
		{
			tPct = malloc(sizeof(tPct)*1);
			tPct[0] = argv[index];
		}
	}
	
	for (int i = 1; i<atoi(tempPid[0]); i++)
	{
		tPid[i-1].pid = atoi(tempPid[i]);
		tPid[i-1].pct = atof(tPct[i])/100;
		tPid[i-1].cpuUsage = 0;
	}
	
	
	int nbPids = atoi(tempPid[0])-1;
	//printf("first pid: %d\n", tPid[0].pid);
	// long nbPids = sizeof(*tPid)/sizeof(char *);
	// printf("test\n");
	// printf("size tPid: %lu, size a *char: %lu\n", sizeof(*tPid), sizeof(char *));

	//setNice(tPid, nbPids);

	while(true)
	{
		int limit = 50;
		if (i > limit)
		{
			i = 1;
			for (int j = 0; j < nbPids; tPid[j++].cpuUsage = 0);
		}
		//printf("tempPid[0]: %s\n", tempPid[0]);
		calcCPU(tPid, nbPids);
		for (int k = 0; k<nbPids; k++)
		{
			//printf("pid: %d\n", atoi(tPid[k]));
			//printf("Usage: %f\n", tPid[k].cpuUsage);
			double cpuPct = tPid[k].cpuUsage/i;
			if( cpuPct >= tPid[k].pct)
			{
				kill(tPid[k].pid,SIGSTOP);
			}
			else 
			{
				kill(tPid[k].pid,SIGCONT);
			}

			if( (int)i%(limit/2) == 0 )
			{
				FILE* fic = NULL ;
				char filename[50];
				sprintf(filename, "5000000ms/cpuVals%d_%d.csv", tPid[k].pid, limit);
				fic = fopen(filename, "a");
				if(fic != NULL)
				{
					fprintf(fic,"%f\n",cpuPct);
					fclose(fic);
				}
				else 
				{
					printf("impossible d'ouvrir le fichier");
				}
			}
		}
		i++;
	}


	return 0 ; 
}

char **eclaterChaine(const char *s, char separateur)
{
    char **t    = NULL;                 //t est un tableau de chaine de car = tableau de tableau de car d'ou **
    void *temp  = NULL;
    int  deb = 0,fin = 0 , nbChaine = 2;
    char *idx   = (char *)s;            //idx est un pointeur sur la chaine a eclater,
                                        //il est repositionner à la position suivante de chaque séparateur

    t = malloc(sizeof(*t)*nbChaine);
    t[0] = malloc( sizeof(*t[0])*5);
    for (fin = 0; fin < strlen(s); fin++)
    {
        if (s[fin] == separateur)
        {
            nbChaine++;
            temp = realloc(t,sizeof(*t)*nbChaine);
            t = temp;
            t[nbChaine - 2] = malloc(fin - deb + 1);   // ALLOUE L'ESPACE POUR LE TOKEN
            strncpy(t[nbChaine - 2], idx,fin - deb);
            t[nbChaine - 2][fin - deb] = 0;
            idx += fin - deb + 1;
            deb = fin + 1;
        }
    }
    nbChaine++;
    temp = realloc(t,sizeof(*t)*nbChaine);
    t = temp;
    t[nbChaine - 2] = malloc(fin - deb + 1);
    strcpy(t[nbChaine - 2], idx);
    t[nbChaine - 2][fin - deb] = 0;
    t[nbChaine-1] = NULL;
    char str[5];
    sprintf(str, "%d", nbChaine-1);  //on passe le nombre de strings afin de pouvoir iterer dedans
    memset(t[0], '\0', 5);
    strcpy(t[0], str);
    return t;
}
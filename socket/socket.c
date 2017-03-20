#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define PORT 30500

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;


int main(int argc, char const *argv[])
{
	/* Socket et contexte d'adressage du serveur */
    SOCKADDR_IN sin;
    SOCKET sock;
    socklen_t recsize = sizeof(sin);
    
    /* Socket et contexte d'adressage du client */
    SOCKADDR_IN csin;
    SOCKET csock;
    socklen_t crecsize = sizeof(csin);

    int sock_err;
    char message[2000], fin[10] ;

    strcpy(fin, "FIN");


    /* Création d'une socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock != INVALID_SOCKET)
    {
    	printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
            
        /* Configuration */
        sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
        sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
        sin.sin_port = htons(PORT);               /* Listage du port */
        sock_err = bind(sock, (SOCKADDR*)&sin, recsize);

        /* Si la socket fonctionne */
        if(sock_err != SOCKET_ERROR)
        {
            /* Démarrage du listage (mode server) */
            sock_err = listen(sock, 5);
            printf("Listage du port %d...\n", PORT);


            /* Si la socket fonctionne */
            if(sock_err != SOCKET_ERROR)
            {	
        		/* Attente pendant laquelle le client se connecte */
                printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
                csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
                printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

                while(memcpy(message ,fin) != 0 )
                {
                	recv(csock, message, 2000, 0);
                	printf("vous avez envoyé ...\n");
                	printf("%s",message);
                }    
            }
            else
                perror("listen");

             /* Fermeture de la socket client et de la socket serveur */
            printf("Fermeture de la socket client\n");
            closesocket(csock);
            printf("Fermeture de la socket serveur\n");
            closesocket(sock);
            printf("Fermeture du serveur terminée\n");

        }
        else
            perror("socket");
    }

	return EXIT_SUCCESS;
}
/* receveur portReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

int somme=0;

void arretParControlC(int sig) 
{
printf("terminaison par un Control-C\nsomme=%d\n",somme);
fflush(stdout);
/* Actions a faire pour la partie 1.3*/
}

main (int argc, char **argv)
{
int sock,recu,envoye;
char confirmation[256], nomh[50];
struct sockaddr_in adresseLocale;
int lgadresseLocale;
struct sockaddr_in adresseEmetteur;
int lgadresseEmetteur;
struct hostent *hote;
struct sigaction action;


int unEntier;

if (argc!=2){
	printf("usage : %s [num port] \n",argv[0]);
	return -1;
}

/* handler de signal SIGINT */
action.sa_handler = arretParControlC;
sigaction(SIGINT, &action, NULL);

/* cr'eation de la socket */
if ((sock = socket( AF_INET,SOCK_DGRAM,0 )) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}

/* pr'eparation de l'adresse locale : port + toutes les @ IP */
adresseLocale.sin_family = AF_INET;
adresseLocale.sin_port = htons(atoi(argv[1]));
adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

/* attachement de la socket a` l'adresse locale */
lgadresseLocale = sizeof(adresseLocale);
if ((bind( sock, (struct sockaddr *)&adresseLocale, lgadresseLocale )) == -1) 
	{
  	perror("bind"); 
  	exit(1);
  	}

/* reception d'un entier */
while(1){
	if ((recu = recvfrom( sock, &unEntier, sizeof(int), 0, (struct sockaddr *) &adresseLocale, &lgadresseLocale )) != sizeof(int)) 
		{
		perror("recvfrom unEntier"); 
		close(sock); 
		exit(1);
		}
	somme++;
	printf("j'ai recu l'entier : %d\n", unEntier);
}
close(sock);
}

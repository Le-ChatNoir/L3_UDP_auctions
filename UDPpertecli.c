/* emetteur portReceveur machineReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

main(int argc, char **argv)
{
int sock, envoye, recu;
struct sockaddr_in adresseReceveur;
int lgadresseReceveur;
struct hostent *hote;

int unEntier = atoi(argv[3]);

if (argc!=4){
	printf("usage : %s [num port] [nom du receveur] [entier N]\n",argv[0]);
	return -1;
}


/* cr'eation de la socket */
if ((sock = socket( AF_INET,SOCK_DGRAM,0 )) == -1) 
	{
  	perror("socket"); 
  	exit(1);
  	}

/* recherche de l'@ IP de la machine distante */
if ((hote = gethostbyname(argv[2])) == NULL)
	{
  	perror("gethostbyname"); 
  	close(sock); 
  	exit(2);
  	}

/* pr'eparation de l'adresse distante : port + la premier @ IP */
adresseReceveur.sin_family = AF_INET;
adresseReceveur.sin_port = htons(atoi(argv[1]));
bcopy(hote->h_addr, &adresseReceveur.sin_addr, hote->h_length);
printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseReceveur.sin_addr));

/* envoi d'un entier */
lgadresseReceveur = sizeof(adresseReceveur);
while(unEntier>0){	
	if ((envoye = sendto( sock,&unEntier,sizeof(unEntier),0,(struct sockaddr *) &adresseReceveur,lgadresseReceveur )) != sizeof(int)) 
		{
	  	perror("sendto unEntier"); 
	  	close(sock); 
	  	exit(1);
	  	}
	  	
	printf("unEntier envoye\n");
	unEntier--;
}
close(sock);
}

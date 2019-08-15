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
#include <string.h>

void arretParControlC(int sig) 
{
printf("terminaison par un Control-C\n");
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
char descriptionVente[256];
int offreInitiale;
int valEnvoi = 1;


/* programme mal utilisé */

	if(argc != 3)
	{
		printf("Usage du programme: %s [Adresse IP cible] [Port]\n", argv[0]);
		exit(0);
	}


/* handler de signal SIGINT */
action.sa_handler = arretParControlC;
sigaction(SIGINT, &action, NULL);

/* cr'eation de la socket */
if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	
	printf("Socket créée...\n");
	
/* Recherche de l'adresse IP de la machine distante */
	if((hote = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(2);
	}
	
	printf("Hote distant trouvé!\n");

	/* Préparation de l'adresse distante : port + la 1ere adresse IP */
	adresseEmetteur.sin_family = AF_INET;
	adresseEmetteur.sin_port = htons(atoi(argv[2]));
	bcopy(hote->h_addr, &adresseEmetteur.sin_addr, hote->h_length);
	printf("L'adresse en notation pointée: %s\n\n", inet_ntoa(adresseEmetteur.sin_addr));
	
	
	/* Envoi de l'adresse IP client */
	lgadresseEmetteur = sizeof(adresseEmetteur);
	if((envoye = sendto(sock, &valEnvoi, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != sizeof(int))
	{
		perror("sendto adresseIP");
		exit(1);
	}
	printf("Adresse IP envoyée...\n\n");
	
  	
/* reception de la description de l'offre */
lgadresseEmetteur = sizeof(adresseEmetteur);
if ((recu = recvfrom( sock, descriptionVente, sizeof(descriptionVente), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur )) == -1) 
	{
	perror("recvfrom descriptionVente"); 
	close(sock); 
	exit(1);
	}

/* reception de la description */
lgadresseEmetteur = sizeof(adresseEmetteur);
if ((recu = recvfrom( sock, &offreInitiale, sizeof(offreInitiale), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur )) == -1) 
	{
	perror("recvfrom offreInitiale"); 
	close(sock); 
	exit(1);
	}
	
	
/* envoie de la confirmation de reception */
strcpy(confirmation, "condition initiales recues.\n");
if((envoye = sendto(sock, confirmation, strlen(confirmation)+1, 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != strlen(confirmation)+1)
	{
		perror("sendto offreInitiale");
		exit(1);
	}
	printf("Confirmation envoyée...\n\n");

	
printf("Les enchères sont ouvertes ! \"%s\" est en vente pour un prix de départ de %d.\n", descriptionVente, offreInitiale);

close(sock);
}

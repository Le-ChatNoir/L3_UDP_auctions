#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int arg, char **argv){

	if(arg != 2)
	{
		printf("Usage du programme: %s [Port]\n", argv[0]);
		exit(0);
	}
	
	int sockAcc, sockVente, envoye, recu;
	char confirmation[256];
	struct sockaddr_in adresseReceveur;
	int lgadresseReceveur;
	struct hostent *hote;
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	char nomh[50];

	char descriptionVente[80];
	int offreInitiale = 149;
	
	int contAcc;
	contAcc = 1;
	char accCont[256];
	int strcmpAcc = 1;
	
	int valIP;
	int nbClients = 0;
	struct sockaddr_in tabClients[100];
	int parcourCli;
	




	/*-----------------CODE DE LANCEMENT DE LA SOCKET D'ACCUEIL-----------------------*/
	

	strcpy(descriptionVente, "Les cendres de mamie");

	/* Création de la socket d'accueil */
	if((sockAcc = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socketAcc");
		exit(1);
	}
	
	printf("Socket d'accueil créée...\n");
	
	/* Récupération de l'adresse locale */

if (gethostname(nomh, 50) == -1){
	perror("gethostname"); exit(1);
}
	printf("Je m'execute sur %s\n", nomh);

	/* pr'eparation de l'adresse locale : port + toutes les @ IP */
	adresseLocale.sin_family = AF_INET;
	adresseLocale.sin_port = htons(atoi(argv[1]));
	adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

	/* attachement de la socket a` l'adresse locale */
	lgadresseLocale = sizeof(adresseLocale);
	if ((bind( sockAcc, (struct sockaddr *) &adresseLocale, lgadresseLocale )) == -1) 
	{
  		perror("bind"); 
  		exit(1);
  	}
  	
  	
  	
  	
  	/*----------------BOUCLE D'ACCUEIL-----------------------*/
  	
  	while(contAcc){
  		printf("\n\nEn attente...\n");
  	
  		
	  	/* En attente de réception de l'IP client */
	  	lgadresseReceveur = sizeof(adresseReceveur);
		if((recu = recvfrom(sockAcc, &valIP, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1)
		{
			perror("recvfrom IPcli");
			exit(2);
		}
		
		tabClients[nbClients] = adresseReceveur;
		printf("Adresse IP recue : %s\n", inet_ntoa(adresseReceveur.sin_addr));

	
		
  		nbClients++;

		printf("Nombre de clients actifs: %d\nSouhaitez vous attendre d'autres acheteurs ? (o)ui/(n)on/(v)oir les clients\n", nbClients);

		while(strcmpAcc){
			
			scanf("%s", accCont);
			if((strcmp(accCont, "o") != 0) && (strcmp(accCont, "n") != 0))
				{
				if(strcmp(accCont, "v") == 0)
					{
					for(parcourCli = 0; parcourCli < nbClients; parcourCli++)
						{
						printf("Adresse IPV4 du client numéro %d: %s\n", parcourCli+1, inet_ntoa(tabClients[parcourCli].sin_addr));
						}
					printf("\nFin de l'affichage des clients en attente.\n\n");
					} else 
						{
						printf("Carractère invalide\n");
						continue;
						}
				} else break;
		}
		
		if(strcmp(accCont, "o") == 0)
			{
			continue;
			} 
		if (strcmp(accCont, "n") == 0) 
			{
			printf("\nFin de l'accueil...\n\n");
			contAcc = 0;
			continue;
			}
		
	}
	
	
	
	/*--------CREATION DE LA SOCKET DE VENTE----------*/
	
	/* Création de la socket d'accueil */
	if((sockVente = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socketVente");
		exit(1);
	}
	
	printf("Socket de vente créée...\n\n\n");
	
	close(sockAcc);
	
	/* Récupération de l'adresse locale */

if (gethostname(nomh, 50) == -1){
	perror("gethostname"); exit(1);
}

	/* pr'eparation de l'adresse locale : port + toutes les @ IP */
	adresseLocale.sin_family = AF_INET;
	adresseLocale.sin_port = htons(atoi(argv[1]));
	adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

	/* attachement de la socket a` l'adresse locale */
	lgadresseLocale = sizeof(adresseLocale);
	if ((bind( sockVente, (struct sockaddr *) &adresseLocale, lgadresseLocale )) == -1) 
	{
  		perror("bind"); 
  		exit(1);
  	}
  	
  	
  	
	
	
	/*-----------BOUCLE D'ENVOI DES INFOS------------*/
	printf("La vente a commencée ! Envoie en cours des informations...\n\n");
	
	for(parcourCli = 0; parcourCli < nbClients; parcourCli++)
		{
		printf("Traitement du client n°%d...\n", parcourCli+1);
	/* Envoie de la description */
		lgadresseReceveur = sizeof(tabClients[parcourCli]);

		if((envoye = sendto(sockVente, descriptionVente, strlen(descriptionVente)+1, 0, (struct sockaddr *) &tabClients[parcourCli], lgadresseReceveur)) != strlen(descriptionVente)+1)
			{
			perror("sendto descriptionVente");
			exit(1);
			}
		printf("Description de la vente envoyée...\n");

		/* Envoie de l'offre initiale */
		if((envoye = sendto(sockVente, &offreInitiale, sizeof(offreInitiale), 0, (struct sockaddr *) &tabClients[parcourCli], lgadresseReceveur)) != sizeof(offreInitiale))
			{
			perror("sendto offreInitiale");
			exit(1);
			}
		printf("Offre initiale envoyée...\n");


		/* Confirmation de réception */
		if((recu = recvfrom(sockVente, confirmation, sizeof(confirmation), 0, (struct sockaddr *) &tabClients[parcourCli], &lgadresseReceveur)) == -1)
			{
			perror("recvfrom");
			exit(2);
			}
			
		printf("Client n°%d traité !\n\n", parcourCli+1);
		}
		printf("Tous les clients traités.\n\n");
		
		

	return 0;
}

/*
Cours : INFOF201 
Projet : Programmation Systèmes : Chatroom
Auteurs : BAS Alessandra, NIETO NAVARRETE Matias
Date de remise : 19 décembre 2021
*/

// Côté client. Utiliser la commande ./client <Username> 127.0.0.1 8080


#include <pthread.h>
#include "common.h"

// Variables globales
int sock = 0;
char name[32];


/**
 * Prépare le socket pour la connection en prenant le port et l'adresse ip donnés pas l'utilisateur.
 * 
 */
void setup_socket(int port, const char *ip_address){

  struct sockaddr_in serv_addr;
  sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port); 

  // Conversion de string vers IPv4 ou IPv6 en binaire
  checked(inet_pton(AF_INET, ip_address, &serv_addr.sin_addr)); 
  checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));

}

/**
 * Prend le message écrit par l'utilisateur, vérifie si le message contient au moins un caractère et 
 * envoie le message au server.
 */
void send_msg(){
  
	char buffer[LENGTH];

  while(1) {

    fgets(buffer, LENGTH, stdin);

    
    if (strlen(buffer) > 1){ // envoi message que si recois caracteres 
     
      buffer[strlen(buffer) - 1] = '\0';    
      char* timestamp = get_time();
      ssend(sock, timestamp, buffer, strlen(buffer));
    }

    // met buffer a 0 octet
    bzero(buffer, LENGTH);


  }
}

/**
 * Reçoit les messages envoyés par le serveur et vérifie qu'ils ont la bonne taille (en coupant la 
 * fin du message si ce n'est pas le cas). Vérifie également si le message reçu indique que le serveur 
 * s'est fermé. Si c'est le cas, le client se ferme.
 */

void recv_msg(){
    
  while (1) {

    char *message = NULL;

		char* timestamp = get_time();
    size_t recv = receive(sock, timestamp, (void *)&message);

    message[recv] = '\0'; // met le message a la bonne taille

    if (recv > 0) {

        printf(" > %s\n", message);
        
        // si serveur s'arrete le client se ferme proprment
        if (strcmp(message,"Le serveur s'est arreté") == 0){ 
          flag = true;
          break;
        }
        //libere la memoir utilise par message pour un prochain message
        free(message);
    } 
    else if (recv == 0) {
        break;
    } 
  }
}

/**
 * Envoie le username au server. Ensuite, créé une thread pour l'envoie de messages et un thread
 *  pour la réception de messages.
 */
int main(int argc, char const *argv[]) {
  if(argc != 4 ){
		printf("Format attendu: %s <username> <ip> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
  
  // nom utilisateur placer dans nom 
  strcpy(name,argv[1]);

  // Détecte un CTRL + C
  signal(SIGINT, catch_ctrl_c);

  int port = atoi(argv[3]); // 8080
  const char *ip_address = argv[2]; // 127.0.0.1
  setup_socket(port,ip_address);

  // envoi l'username au serveur
  char* timestamp = get_time();
  ssend(sock, timestamp, name,strlen(name));

  printf("\n=== Bienvenue dans le salon de dicussion ===\n\n");

  pthread_t send_msg_thread;
  if(pthread_create(&send_msg_thread, NULL, (void *) send_msg, NULL) != 0){
      printf("ERREUR: pthread\n");
      return EXIT_FAILURE;
	}

  pthread_t recv_msg_thread;
  if(pthread_create(&recv_msg_thread, NULL, (void *)recv_msg, NULL) != 0){
      printf("ERREUR: pthread\n");
		  return EXIT_FAILURE;
	}
  
  while (1){
      if(flag){
        printf("\n=== Déconnexion ===\n");
        exit(0);
        }
	}

  return 0; 
}
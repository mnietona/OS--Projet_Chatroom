/*
Cours : INFOF201 
Projet : Programmation Systèmes : Chatroom
Auteurs : BAS Alessandra, NIETO NAVARRETE Matias
Date de remise : 19 décembre 2021
*/

// Côté serveur. Utiliser la commande ./select_server 8080


#include "common.h"

int master_socket = 0;
int opt = 1;
struct sockaddr_in address;

struct User {
  char nom[32];
};

/**
 * Prépare le socket pour la connexion en prenant le port.
 */
void setup_socket(int port){
  
  master_socket = checked(socket(AF_INET, SOCK_STREAM, 0));
  address.sin_family = AF_INET;
  address.sin_port = htons(port); //8080
  address.sin_addr.s_addr = INADDR_ANY;

  checked(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)));
  checked(bind(master_socket, (struct sockaddr *)&address, sizeof(address)));
  checked(listen(master_socket, 3));

}

/**
 * Démarre le serveur, gère les différents clients grâce à l'appel système select. 
 * Le main se charge également d'avertir tous les clients si le serveur se ferme,
 * d'afficher les messages chez tous les clients avec les bons username et indique
 * les connexions et déconnexions.
 */
int main(int argc, char *argv[]) {
  if(argc != 2){
		printf("Format attendu : %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
  }


  int port = atoi(argv[1]); // 8080
  setup_socket(port);
  
  
  size_t addrlen = sizeof(address);
  fd_set readfds;
  int clients[1024];
  int nclients = 0;
  struct User nom_user[100]; // tableau des pseudos
  int count = 0;

  printf("\n==== Serveur ouvert ====\n\n");

  while (true) {

    FD_ZERO(&readfds);
    FD_SET(master_socket, &readfds);
    int max_fd = master_socket;
    
    for (int i = 0; i < nclients; i++) {
      FD_SET(clients[i], &readfds);
      if (clients[i] > max_fd) {
        max_fd = clients[i];
      }
    }

    // attent activité sur un des sockets
    select(max_fd + 1, &readfds, NULL, NULL, NULL);

    // Détecte un CTRL + C
    signal(SIGINT, catch_ctrl_c);

    // Previens les clients que le serveur s'arrete
    if (flag){
      for (int j = 0; j< nclients; j++){
        char msg[] = "Le serveur s'est arreté";
        char* timestamp = get_time();
        ssend(clients[j], timestamp, msg, strlen(msg));
      }
      printf("\n=== Déconnexion du serveur ===\n");
      exit(0);
    }
    

    if (FD_ISSET(master_socket, &readfds)) {
      // Si c'est le master socket qui a des donnees, c'est une nouvelle connexion.
      clients[nclients] = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen);
      nclients++;

      // permet d'activer l'enregistrement du nom a la connexion du client
      count = 1;

    } 
    else {
      // Sinon, c'est un message d'un client
      for (int i = 0; i < nclients; i++) {

        if (FD_ISSET(clients[i], &readfds)) {

          char *buffer = NULL;
          char* timestamp = get_time();
          size_t nbytes = receive(clients[i], timestamp, (void *)&buffer);
          
          if (nbytes > 0) {
            buffer[nbytes] = '\0'; // met le message a la bonne taille

            // enregistre nom client
            if (count == 1){

              // enregistre le pseudo du client i dans le tableau
              strcpy(nom_user[clients[i]].nom,buffer);

              // envoi le message de connexion aux clients
              char co[LENGTH+32] = {};
              strcat(co, nom_user[clients[i]].nom);
              strcat(co," est connecté");
              printf("%s\n", co); // affiche sur le serveur le nom du client connecté

              for (int j = 0; j< nclients; j++){
                char* timestamp = get_time();
                ssend(clients[j],timestamp, co, strlen(co));
              }
              count--; // permet d'attendre une nouvelle connexion
              bzero(co, LENGTH+32);  // met co a 0 octet
            }
            
            else{
            char message[LENGTH+32] = {};

            // met pseudo et le buffer du client i dans message 
            sprintf(message, "%s : %s", nom_user[clients[i]].nom, buffer);

            // envoi a tout les clients le pseudo et le message 
            for (int j = 0; j< nclients; j++){
              char* timestamp = get_time();
              ssend(clients[j], timestamp, message, strlen(message));
            }
            printf("%s\n",message); // l'ecrit sur le serveur
            bzero(message, LENGTH+32); // met message a 0 octet
            }
            free(buffer); //libere la memoir utilise par buffer pour un prochain message
           
          } 
          else {;
            // previens les clients qu'un clients s'est deconnecté
            char deco[LENGTH+32] = {};
            strcat(deco,nom_user[clients[i]].nom);
            strcat(deco," est déconnecté");

            close(clients[i]);
            // On deplace le dernier socket a la place de libre pur ne pas faire de trou.
            clients[i] = clients[nclients - 1];
            nclients--;

            printf("%s\n",deco); // affiche sur le serveur le nom du client déconnecté
            for (int j = 0; j< nclients; j++){
              char* timestamp = get_time();
              ssend(clients[j], timestamp, deco, strlen(deco));
            }
            bzero(deco, LENGTH); // met deco a 0 octet
          }
        }
      }
    }
  }

  return 0;
}
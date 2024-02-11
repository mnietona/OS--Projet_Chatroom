/*
Cours : INFOF201 
Projet : Programmation Systèmes : Chatroom
Auteurs : BAS Alessandra, NIETO NAVARRETE Matias
Date de remise : 19 décembre 2021
*/

#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define LENGTH 1024 

volatile bool flag = false;

/**
 * Permet d'arrêter proprement le programme et affiche un message à 
 * l'utilisateur après avoir fait un CTRL+C.
 */
void catch_ctrl_c(int sig) {
    flag = true;
}

/**
 * Vérifie si les fonctions du socket sont possible sinon ferme le programme avec message d'erreur.
 */
int _checked(int ret, char* calling_function) {
  if (ret < 0) {
    perror(calling_function);
    exit(EXIT_FAILURE);
  }
  return ret;
}

#define checked(call) _checked(call, #call)

/**
 * Permet d'écrire dans un socket et renvoi le nombre d'octets envoyés.
 */
int ssend(int sock, char* timestamp, void* data, size_t len) {
  checked(write(sock, &len, sizeof(len)));
  return checked(write(sock, data, len));
}

/**
 * Permet de lire dans un socket et renvoi le nombre d'octets lus.
 */
size_t receive(int sock, char* timestamp, void** dest) {
  size_t nbytes_to_receive;
  if (checked(read(sock, &nbytes_to_receive, sizeof(nbytes_to_receive))) == 0) {
    // Connexion fermée.
    return 0;
  };

  unsigned char* buffer = malloc(nbytes_to_receive);
  if (buffer == NULL) {
    fprintf(stderr, "malloc n'a pas pu allouer %zd bytes", nbytes_to_receive);
    perror("");
    exit(1);
  }
  size_t total_received = 0;
  while (nbytes_to_receive > 0) {
    size_t received = checked(read(sock, &buffer[total_received], nbytes_to_receive));
    if (received < 0) {
      return total_received;
    }
    total_received += received;
    nbytes_to_receive -= received;
  }
  *dest = buffer;

  return total_received;
}

/** 
 * Renvoie la date et l'heure.
 */
char* get_time(){

    time_t now = time(NULL);
    char * timestamp = ctime(&now);
    timestamp[strlen(timestamp)-1] = '\0';

    return timestamp;
}

#endif  // __COMMON_H

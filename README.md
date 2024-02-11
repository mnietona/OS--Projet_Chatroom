# Chatroom - Projet de Système d'Exploitation (OS)

## Cours : INFOF201 - Programmation Systèmes

## Description du Projet
Ce projet a été développé dans le cadre du cours de système d'exploitation (OS). Il s'agit d'une application chatroom qui permet à plusieurs utilisateurs de communiquer en temps réel. Le projet est divisé en trois composants principaux : le serveur, le client, et l'interface client. Ces composants facilitent la gestion des connexions, l'envoi et la réception de messages, ainsi que l'interaction utilisateur grâce à une interface conviviale.

## Installation et Compilation
Pour compiler le projet, assurez-vous d'avoir GCC installé sur votre système. Utilisez ensuite le `Makefile` fourni pour compiler les différents composants.

Exécutez la commande suivante dans votre terminal pour compiler le projet :

```bash
cd chatroom && make
```

Cette commande compile les trois composants du projet :
- `server` : le serveur de la chatroom.
- `client` : le client de la chatroom qui communique avec le serveur.
- `c_interface` : l'interface utilisateur du client, utilisant `ncurses` et `pthread` pour une expérience interactive.

## Utilisation
Après la compilation, vous pouvez démarrer le serveur, l'interface client, et le client en utilisant les exécutables générés.

Pour lancer le serveur :

```bash
./server [port]
```

Pour lancer le client (dans un nouveau terminal) :

```bash
./client [Username] [ip] [port]
```

Pour lancer un client avec une interface (dans un nouveau terminal) :

```bash
./c\_interface [Username] [ip] [port]
```

Assurez-vous de démarrer le serveur avant les clients pour permettre une connexion réussie.

## Nettoyage
Pour supprimer les exécutables et nettoyer votre répertoire de travail, utilisez la commande suivante :

```bash
make clean
```

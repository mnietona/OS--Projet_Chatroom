#Cours : INFOF201 
#Projet : Programmation Systèmes : Chatroom
#Auteurs : BAS Alessandra, NIETO NAVARRETE Matias
#Date de remise : 19 décembre 2021

all: chatroom

chatroom: server.c client.c client_interface.c
	gcc -o server server.c -Wall 
	gcc -o client client.c -lpthread -Wall 
	gcc -o c_interface client_interface.c -lncurses -lpthread -Wall

clean:
	rm -f server client c_interface
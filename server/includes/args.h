#ifndef		_ARGS_H_
# define	_ARGS_H_

#include	<stdio.h>
#include	<stdlib.h>
#include	<getopt.h>

# define USAGE "Usage:\n\t--rep-port [port utilisé pour recevoir les commandes des clients et répondre]\n\t--pub-port [port utilisé pour envoyer les notifications aux clients]\n\t--cycle [nombre en microsecondes correspondant à un cycle (> 0)]\n\t-v (active le mode verbose (loglevel INFO))\n\t--log [fichier de log]\n\t--size [taille de la map (minimum 5)]\nExemple: ./server -v --size 8 --log /tmp/soft_war.log --cycle 1000000 --rep-port 4242 --pub-port 4243"
# define BAD_ARGS "Mauvais argument pour l'une des options.\nMerci de vous référer à l'usage.\n"

typedef struct 	s_args
{
    int     	rep_port;
    int     	pub_port;
    int     	cycle;
    int     	v;
    char    	*log;
    int     	size;
}				t_args;

int         parse_args(int, char**, t_args*);

#endif		/* _ARGS_H_ */

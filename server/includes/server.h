#ifndef			_SERVER_H_
# define		_SERVER_H_

# include       <ctype.h>
# include       <stdio.h>
# include       <stdlib.h>
# include       <unistd.h>

typedef struct args ARGS;

typedef struct      args
{
    int             rep_port;
    int             pub_port;
    int             cycle;
    int             v;
    int             log;
    int             size;
}			        ARGS;

ARGS            *parse_args(int, char**);

#endif			/* _SERVER_H_ */

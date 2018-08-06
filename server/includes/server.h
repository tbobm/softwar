#ifndef		_SERVER_H_
# define	_SERVER_H_


# include       "args.h"
# include       "log.h"
# include	<zmq.h>
# include <czmq.h>
# include <stdio.h>
# include	<unistd.h>
# include	<string.h>
# include	<assert.h>

# define ERR_SERVER "Quelque chose s'est mal passé à la création du serveur."

typedef struct s_notif        t_notif;
typedef struct s_game_info    t_game_info;
typedef struct s_player       t_player;
typedef struct s_energy_cell  t_energy_cell;

enum e_notification_type
{
  CYCLE_INFO,
  GAME_STARTED,
  GAME_FINISHED,
  CLIENT_DEAD,
  CLIENT_WIN
};

struct s_notification
{
  int   e_notification_type;  // (ex: 0) entier représentant le type de notification
  char  *data;              // (ex: null) type variable en fonction de la notification
};

struct s_game_info
{
  uint      map_size;
  uint      game_status; // 0 => waiting, 1 => started, 2 => finished
  t_player  *players;
  // list<EnergyCell>    energy_cells;
};

struct s_player
{
  char      *name;  // identité unique du joueur
  uint      x;      // position x du joueur
  uint      y;      // position y du joueur
  uint      energy; // énergie restante du joueur
  uint      looking; // direction dans laquelle regarde le joueur (left = 0, up = 1, right = 2, down = 3)
  t_player  *next;
};

struct s_energy_cell
{
  uint	x;      // position x de la cellule
  uint	y;      // position y de la cellule
  uint	value;  // quantité d'énergie de la cellule
};

// Server core functions
int	manage_server(ARGS*);
int	start_server(ARGS*);

// Linked list functions for Player
t_player *create_player(char*, uint*, t_player*);
t_player *prepend(t_player*, char*, uint*);
t_player* search(t_player*, char*);
void display(t_player*);

// -----------------------------------------
// BELOW IS THE ARCHITECHTURE NEEDED FOR RFC
// -----------------------------------------

// Function Pointer Type for RFC
typedef zframe_t *(*t_cmd)(char*);

// Structure for binding cmd with function
typedef struct  s_bind_cmd
{
  t_cmd         cmd_function;
  char          cmd_name[];
}               t_bind_cmd;

// RFC Functions
zframe_t *identify(char*);
zframe_t *forward(char*);
zframe_t *backward(char*);
zframe_t *leftfwd(char*);
zframe_t *rightfwd(char*);
zframe_t *right(char*);
zframe_t *left(char*);
zframe_t *looking(char*);
zframe_t *gather(char*);
zframe_t *watch(char*);
zframe_t *attack(char*);
zframe_t *selfid(char*);
zframe_t *selfstats(char*);
zframe_t *inspect(char*);
zframe_t *next(char*);
zframe_t *jump(char*);

#endif    /* _SERVER_H_ */

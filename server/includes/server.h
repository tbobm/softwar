#ifndef		_SERVER_H_
# define	_SERVER_H_


# include       "args.h"
# include       "log.h"

# define ERR_SERVER "Quelque chose s'est mal passé à la création du serveur."

typedef struct s_notif          notif;
typedef struct s_game_info      game_info;
typedef struct s_player         player;
typedef struct s_energy_cell    energy_cell;

enum notification_type {
	cycle_info = 0,
	game_started = 1,
	game_finished = 2,
	client_dead = 3,
	client_win = 4
};

struct s_notification {
	int     notification_type;  // (ex: 0) entier représentant le type de notification
	char    *data;              // (ex: null) type variable en fonction de la notification
};

struct s_game_info {
    uint                map_size;
    uint                game_status; // 0 => waiting, 1 => started, 2 => finished
    // list<Player>        players;
    // list<EnergyCell>    energy_cells;
};

struct s_player {
    char        *name;  // identité unique du joueur
    uint        x;      // position x du joueur
    uint        y;      // position y du joueur
    uint        energy; // énergie restante du joueur
    uint        looking; // direction dans laquelle regarde le joueur (left = 0, up = 1, right = 2, down = 3)
};

struct s_energy_cell {
	uint    x;      // position x de la cellule
	uint    y;      // position y de la cellule
	uint    value;  // quantité d'énergie de la cellule
};

int             manage_server(ARGS*);

# include	<zmq.h>
# include	<unistd.h>
# include	<string.h>
# include	<assert.h>

# define ERR_SERVER "Quelque chose s'est mal passé à la création du serveur."

typedef struct s_notif		notif;
typedef struct s_game_info	game_info;
typedef struct s_player		player;
typedef struct s_energy_cell	energy_cell;

enum notification_type {
  cycle_info	= 0,
  game_started	= 1,
  game_finished	= 2,
  client_dead	= 3,
  client_win	= 4
};

struct s_notification {
  int	notification_type;  // (ex: 0) entier représentant le type de notification
  char	*data;              // (ex: null) type variable en fonction de la notification
};

struct s_game_info {
  uint	map_size;
  uint	game_status; // 0 => waiting, 1 => started, 2 => finished
  // list<Player>        players;
  // list<EnergyCell>    energy_cells;
};

struct s_player {
  char	*name;  // identité unique du joueur
  uint	x;      // position x du joueur
  uint	y;      // position y du joueur
  uint	energy; // énergie restante du joueur
  uint	looking; // direction dans laquelle regarde le joueur (left = 0, up = 1, right = 2, down = 3)
};

struct s_energy_cell {
  uint	x;      // position x de la cellule
  uint	y;      // position y de la cellule
  uint	value;  // quantité d'énergie de la cellule
};

int	manage_server(ARGS*);
int	start_server(ARGS*);

#endif		/* _SERVER_H_ */

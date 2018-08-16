#ifndef		_SERVER_H_
# define	_SERVER_H_


# include "args.h"
# include "log.h"
# include	<zmq.h>
# include <czmq.h>
# include <stdio.h>
# include	<unistd.h>
# include	<string.h>
# include	<assert.h>

# define ERR_SERVER "Quelque chose s'est mal passé à la création du serveur."

typedef enum
{
  CYCLE_INFO,
  GAME_STARTED,
  GAME_FINISHED,
  CLIENT_DEAD,
  CLIENT_WIN,
  NB_NOTIF
} e_notification_type;

typedef struct  s_notification
{
  int           e_notification_type;  // (ex: 0) entier représentant le type de notification
  char          *data;              // (ex: null) type variable en fonction de la notification
}               t_notification;

typedef struct  s_player t_player;
struct          s_player
{
  char          *name;  // identité unique du joueur
  uint          x;      // position x du joueur
  uint          y;      // position y du joueur
  uint          energy; // énergie restante du joueur
  uint          looking; // direction dans laquelle regarde le joueur (left = 0, up = 1, right = 2, down = 3)
  t_player      *next;
};

typedef struct  s_energy_cell t_energy_cell;
struct          s_energy_cell
{
  uint          x;      // position x de la cellule
  uint          y;      // position y de la cellule
  uint          value;  // quantité d'énergie de la cellule
  t_energy_cell *next;
};

typedef struct  s_game_info
{
  uint          map_size;
  uint          game_status; // 0 => waiting, 1 => started, 2 => finished
  t_player      *list_players;
  t_energy_cell *list_energy_cells;
}               t_game_info;

// -------------------------------------------
// BELOW IS THE WHAT IS NEEDED FOR GLOBAL INFO
// -------------------------------------------

// Structure taking everything into account to pass along with functions
typedef struct  s_server_info
{
  t_args        *args;
  t_game_info   game_info;
  char          *parsed_cmd;
  char          *parsed_param;
  char          *identity;
  int           nb_clients;
  uint          player_info[4];
}               t_server_info;

// -----------------------------------------
// BELOW IS THE ARCHITECHTURE NEEDED FOR RFC
// -----------------------------------------

// Function Pointer Type for RFC
typedef zframe_t *(*t_cmd)(t_server_info*);

// Structure for binding cmd with function
typedef struct  s_bind_cmd
{
  t_cmd         cmd_function;
  char          cmd_name[10];
}               t_bind_cmd;

// RFC Functions
zframe_t *identify(t_server_info*);
zframe_t *forward(t_server_info*);
zframe_t *backward(t_server_info*);
zframe_t *leftfwd(t_server_info*);
zframe_t *rightfwd(t_server_info*);
zframe_t *right(t_server_info*);
zframe_t *left(t_server_info*);
zframe_t *looking(t_server_info*);
zframe_t *gather(t_server_info*);
zframe_t *watch(t_server_info*);
zframe_t *attack(t_server_info*);
zframe_t *selfid(t_server_info*);
zframe_t *selfstats(t_server_info*);
zframe_t *inspect(t_server_info*);
zframe_t *next(t_server_info*);
zframe_t *jump(t_server_info*);

// --------------------------------------
// BELOW ARE THE FUNCTIONS FOR THE SERVER
// --------------------------------------

// Server core functions
int	message_client_server(t_args*);
int	start_server(t_args*);

// Linked list functions for Player
t_player  *create_player(char*, uint*, t_player*);
t_player  *prepend(t_player*, char*, uint*);
t_player  *search_by_name(t_player*, char*);
t_player  *search_by_pos(t_player*, uint, uint);
void      position_to_fill(t_server_info*);
void      display(t_player*);
int       count_players(t_player*);

// Linked list functions for Energy Cells
t_energy_cell   *create_energy_cell(t_energy_cell*, uint*);
t_energy_cell   *prepend_energy_cell(t_energy_cell*, uint*);
t_energy_cell   *search_energy_cell_by_pos(t_energy_cell*, uint, uint);
void            display_energy_cell(t_energy_cell*);
int             count_energy_cells(t_energy_cell*);

// Additionnal functions for RFC
zframe_t  *move_up(t_server_info*, t_player*);
zframe_t  *move_down(t_server_info*, t_player*);
zframe_t  *move_left(t_server_info*, t_player*);
zframe_t  *move_right(t_server_info*, t_player*);
zframe_t  *go_forward(t_server_info*, t_player*);
zframe_t  *go_backward(t_server_info*, t_player*);
void      rotate_left(t_player*);
void      rotate_right(t_player*);

#endif    /* _SERVER_H_ */

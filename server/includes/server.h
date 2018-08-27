#ifndef		_SERVER_H_
# define	_SERVER_H_


# include "args.h"
# include "log.h"
# include	<zmq.h>
# include <czmq.h>
# include <time.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <assert.h>
# include <pthread.h>

# define _GNU_SOURCE 
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

typedef struct  s_notif
{
  int           e_notif_type;  // (ex: 0) entier représentant le type de notification
  char          *data;         // (ex: null) type variable en fonction de la notification
}               t_notif;

typedef struct  s_player t_player;
struct          s_player
{
  char          *name;          // identité unique du joueur
  uint          x;              // position x du joueur
  uint          y;              // position y du joueur
  uint          energy;         // énergie restante du joueur
  uint          looking;        // direction dans laquelle regarde le joueur (left = 0, up = 1, right = 2, down = 3)
  uint          stun_duration;  // 0 = processus operationnel, 1 ou + stunned.
  float         action;
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
  t_notif       notif;
  t_game_info   game_info;
  char          *parsed_cmd;
  char          *parsed_param;
  char          *identity;
  int           nb_clients;
  int           nb_energy;
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
t_player  *remove_first(t_player*);
t_player  *remove_last(t_player*);
t_player  *remove_any(t_player*, t_player*);
void      position_to_fill(t_server_info*);
void      display(t_player*);
void      cycle_energy_loss(t_player*);
void      reset_action(t_player*);
int       count_players(t_player*);
int       count_players_alive(t_server_info*);

// Linked list functions for Energy Cells
t_energy_cell   *create_energy_cell(t_energy_cell*, uint*);
t_energy_cell   *prepend_energy_cell(t_energy_cell*, uint*);
t_energy_cell   *search_energy_cell_by_pos(t_energy_cell*, uint, uint);
t_energy_cell   *remove_first_energy_cell(t_energy_cell*);
t_energy_cell   *remove_last_energy_cell(t_energy_cell*);
t_energy_cell   *remove_any_energy_cell(t_energy_cell*, t_energy_cell*);
void            generate_energy_cell(t_server_info*);
void            display_energy_cell(t_energy_cell*);
int             count_energy_cells(t_energy_cell*);

// Additionnal functions for RFC
zframe_t  *move_up(t_server_info*, t_player*, int);
zframe_t  *move_down(t_server_info*, t_player*, int);
zframe_t  *move_left(t_server_info*, t_player*, int);
zframe_t  *move_right(t_server_info*, t_player*, int);
zframe_t  *go_forward(t_server_info*, t_player*, int);
zframe_t  *go_backward(t_server_info*, t_player*);
zframe_t  *watch_vision(t_server_info*, t_player*);
zframe_t  *cone_attack(t_server_info*, t_player*);
zframe_t  *gather_energy(t_server_info*, t_player*);
void      rotate_left(t_player*);
void      rotate_right(t_player*);

// Function for thread pub-sub
void      *pub_sub_worker(void*);

#endif    /* _SERVER_H_ */

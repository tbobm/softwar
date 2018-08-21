#include            "../includes/server.h"

t_player            *create_player(char *name, uint *x_y_energy_looking, t_player *next)
{
    t_player        *new_player = NULL;
    if ((new_player = (t_player*)malloc(sizeof(t_player))) == NULL)
    {
        printf("Error creating a new player.\n");
        exit(0);
    }
    new_player->name          = name;
    new_player->x             = x_y_energy_looking[0];
    new_player->y             = x_y_energy_looking[1];
    new_player->energy        = x_y_energy_looking[2];
    new_player->looking       = x_y_energy_looking[3];
    new_player->stun_duration = 0;
    new_player->next          = next;
    return new_player;
}

t_player            *prepend(t_player *list_player, char *name, uint *x_y_energy_looking)
{
    return create_player(name, x_y_energy_looking, list_player);
}

t_player            *remove_first(t_player *list_players)
{
    if (list_players == NULL)
        return NULL;

    t_player *first_player = list_players;
    list_players           = list_players->next;
    first_player->next     = NULL;

    // If this is the last player in the list
    if (first_player == list_players)
        list_players = NULL;

    free(first_player);
    return list_players;
}

t_player            *remove_last(t_player *list_players)
{
    if (list_players == NULL)
        return NULL;

    t_player *cursor      = list_players;
    t_player *last_player = NULL;

    while (cursor->next != NULL)
    {
        last_player = cursor;
        cursor      = cursor->next;
    }
    if (last_player != NULL)
        last_player->next = NULL;

    // If this is the last player in the list
    if (cursor == list_players)
        list_players = NULL;

    free(cursor);
    return list_players;
}

t_player            *remove_any(t_player *list_players, t_player *player)
{
    // If the player is the first in the list 
    if (player == list_players)
    {
        list_players = remove_first(list_players);
        return list_players;
    }

    /* If the player is the last in the list */
    if (player->next == NULL)
    {
        list_players = remove_last(list_players);
        return list_players;
    }

    // If the player is between other players in the list 
    t_player *cursor = list_players;
    while (cursor != NULL)
    {
        if (cursor->next == player)
            break;
        cursor = cursor->next;
    }

    if (cursor != NULL)
    {
        t_player *tmp = cursor->next;
        cursor->next       = tmp->next;
        tmp->next          = NULL;
        free(tmp);
    }
    return list_players;
}

void                display(t_player *list_player)
{
    t_player        *tmp = list_player;

    printf("Players list :\n");
    while (tmp != NULL)
    {
        printf(
            "  Name : %s, Positions x, y : %u, %u, Energy : %u, Looking : %u, Stun duration : %u\n",
            tmp->name, tmp->x, tmp->y, tmp->energy, tmp->looking, tmp->stun_duration
        );
        tmp = tmp->next;
    }
}

t_player            *search_by_name(t_player *list_player, char *name)
{
    t_player        *tmp = list_player;

    while (tmp != NULL)
    {
        if (strcmp(tmp->name, name) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

t_player            *search_by_pos(t_player *list_player, uint x, uint y)
{
    t_player        *tmp = list_player;

    while (tmp != NULL)
    {
        if (tmp->x == x && tmp->y == y)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int                 count_players(t_player *list_player)
{
    t_player        *tmp = list_player;
    int             c = 0;

    while(tmp != NULL)
    {
        c++;
        tmp = tmp->next;
    }
    return c;
}

void                position_to_fill(t_server_info *server_info)
{
    int             x = 0;
    int             y = 0;
    int             looking = 3;

    switch (count_players(server_info->game_info.list_players))
    {  
        case 1 :
            x = server_info->args->size - 1;
            y = 0;
            looking = 3;
            break;
        case 2 :
            x = 0;
            y = server_info->args->size - 1;
            looking = 1;
            break;
        case 3 :
            x = server_info->args->size - 1;
            y = server_info->args->size - 1;
            looking = 1;
            break;
    }  
    server_info->player_info[0] = x;
    server_info->player_info[1] = y;
    server_info->player_info[3] = looking;
}

zframe_t            *move_up(t_server_info *server_info, t_player *player, int nb_cells)
{
    if ((int)(player->y - nb_cells) < 0) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x, player->y - nb_cells) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->y -= nb_cells;
    return zframe_from("OK|you moved up");
}

zframe_t            *move_down(t_server_info *server_info, t_player *player, int nb_cells)
{
    if ((int)(player->y + nb_cells) >= server_info->args->size) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x, player->y + nb_cells) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->y += nb_cells;
    return zframe_from("OK|you moved down");
}

zframe_t            *move_left(t_server_info *server_info, t_player *player, int nb_cells)
{
    if ((int)(player->x - nb_cells) < 0) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x - nb_cells, player->y) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->x -= nb_cells;
    return zframe_from("OK|you moved left");
}

zframe_t            *move_right(t_server_info *server_info, t_player *player, int nb_cells)
{
    if ((int)(player->x + nb_cells) >= server_info->args->size) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x + nb_cells, player->y) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->x += nb_cells;
    return zframe_from("OK|you moved right");
}

zframe_t            *go_forward(t_server_info *server_info, t_player *player, int nb_cells)
{
    switch (player->looking)
    {
        case 0 :
            return move_left(server_info, player, nb_cells);
        case 1 :
            return move_up(server_info, player, nb_cells);
        case 2 :
            return move_right(server_info, player, nb_cells);
        case 3 :
            return move_down(server_info, player, nb_cells);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

zframe_t            *go_backward(t_server_info *server_info, t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            return move_right(server_info, player, 1);
        case 1 :
            return move_down(server_info, player, 1);
        case 2 :
            return move_left(server_info, player, 1);
        case 3 :
            return move_up(server_info, player, 1);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

void                rotate_left(t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            player->looking = 3;
            return;
        case 1 :
            player->looking = 0;
            return;
        case 2 :
            player->looking = 1;
            return;
        case 3 :
            player->looking = 2;
            return;
    }
}

void                rotate_right(t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            player->looking = 1;
            return;
        case 1 :
            player->looking = 2;
            return;
        case 2 :
            player->looking = 3;
            return;
        case 3 :
            player->looking = 0;
            return;
    }
}

// --------------------------------------
// BELOW ARE THE FUNCTIONS FOR THE VISION
// --------------------------------------

static char         *seek_on_cell(t_server_info *server_info, t_player *player, uint diff_x, uint diff_y)
{
    t_player        *found;

    if ((int)(player->y + diff_y) < 0) {
        return "";
    }
    if ((int)(player->y + diff_y) >= server_info->args->size) {
        return "";
    }
    if ((int)(player->x + diff_x) < 0) {
        return "";
    }
    if ((int)(player->x + diff_x) >= server_info->args->size) {
        return "";
    }
    if ((found = search_by_pos(server_info->game_info.list_players, player->x + diff_x, player->y + diff_y)) != NULL) {
        return found->name;
    }
    if (search_energy_cell_by_pos(server_info->game_info.list_energy_cells, player->x + diff_x, player->y + diff_y) != NULL) {
        return "energy";
    }
    return "empty";
}

static zframe_t     *seek_up(t_server_info *server_info, t_player *player)
{
    char            buff[50];

    sprintf(
        buff,
        "OK|[\"%s\",\"%s\",\"%s\",\"%s\"]",
        seek_on_cell(server_info, player, 0, -1),
        seek_on_cell(server_info, player, -1, -2),
        seek_on_cell(server_info, player, 0, -2),
        seek_on_cell(server_info, player, 1, -2)
    );
    return zframe_from(buff);
}

static zframe_t     *seek_down(t_server_info *server_info, t_player *player)
{
    char            buff[40];

    sprintf(
        buff,
        "OK|[%s,%s,%s,%s]",
        seek_on_cell(server_info, player, 0, 1),
        seek_on_cell(server_info, player, 1, 2),
        seek_on_cell(server_info, player, 0, 2),
        seek_on_cell(server_info, player, -1, 2)
    );
    return zframe_from(buff);
}

static zframe_t     *seek_left(t_server_info *server_info, t_player *player)
{
    char            buff[40];

    sprintf(
        buff,
        "OK|[%s,%s,%s,%s]",
        seek_on_cell(server_info, player, -1, 0),
        seek_on_cell(server_info, player, -2, 1),
        seek_on_cell(server_info, player, -2, 0),
        seek_on_cell(server_info, player, -2, -1)
    );
    return zframe_from(buff);
}

static zframe_t     *seek_right(t_server_info *server_info, t_player *player)
{
    char            buff[40];

    sprintf(
        buff,
        "OK|[%s,%s,%s,%s]",
        seek_on_cell(server_info, player, 1, 0),
        seek_on_cell(server_info, player, 2, -1),
        seek_on_cell(server_info, player, 2, 0),
        seek_on_cell(server_info, player, 2, 1)
    );
    return zframe_from(buff);
}

zframe_t            *watch_vision(t_server_info *server_info, t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            return seek_left(server_info, player);
        case 1 :
            return seek_up(server_info, player);
        case 2 :
            return seek_right(server_info, player);
        case 3 :
            return seek_down(server_info, player);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

// --------------------------------------
// BELOW ARE THE FUNCTIONS FOR THE ATTACK
// --------------------------------------

static char         *player_on_cell(t_server_info *server_info, t_player *player, uint diff_x, uint diff_y)
{
    t_player        *found;

    if ((int)(player->y + diff_y) < 0) {
        return "";
    }
    if ((int)(player->y + diff_y) >= server_info->args->size) {
        return "";
    }
    if ((int)(player->x + diff_x) < 0) {
        return "";
    }
    if ((int)(player->x + diff_x) >= server_info->args->size) {
        return "";
    }
    if ((found = search_by_pos(server_info->game_info.list_players, player->x + diff_x, player->y + diff_y)) != NULL) {
        found->stun_duration = 2;
        return found->name;
    }
    return "";
}

static zframe_t     *attack_up(t_server_info *server_info, t_player *player)
{
    char            buff[35];
    int             nb_attacked_player;

    nb_attacked_player = 0;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 0, -1), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -1, -2), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 0, -2), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 1, -2), "") == 0) ? 0 : 1;
    sprintf(buff, "OK|%d player(s) have been attacked", nb_attacked_player);
    return zframe_from(buff);
}

static zframe_t     *attack_down(t_server_info *server_info, t_player *player)
{
    char            buff[35];
    int             nb_attacked_player;

    nb_attacked_player = 0;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 0, 1), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 1, 2), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 0, 2), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -1, 2), "") == 0) ? 0 : 1;
    sprintf(buff, "OK|%d player(s) have been attacked", nb_attacked_player);
    return zframe_from(buff);
}

static zframe_t     *attack_left(t_server_info *server_info, t_player *player)
{
    char            buff[35];
    int             nb_attacked_player;

    nb_attacked_player = 0;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -1, 0), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -2, 1), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -2, 0), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, -2, -1), "") == 0) ? 0 : 1;
    sprintf(buff, "OK|%d player(s) have been attacked", nb_attacked_player);
    return zframe_from(buff);
}

static zframe_t     *attack_right(t_server_info *server_info, t_player *player)
{
    char            buff[35];
    int             nb_attacked_player;

    nb_attacked_player = 0;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 1, 0), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 2, -1), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 2, 0), "") == 0) ? 0 : 1;
    nb_attacked_player += (strcmp(player_on_cell(server_info, player, 2, 1), "") == 0) ? 0 : 1;
    sprintf(buff, "OK|%d player(s) have been attacked", nb_attacked_player);
    return zframe_from(buff);
}

zframe_t            *cone_attack(t_server_info *server_info, t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            return attack_left(server_info, player);
        case 1 :
            return attack_up(server_info, player);
        case 2 :
            return attack_right(server_info, player);
        case 3 :
            return attack_down(server_info, player);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

// -----------------------------------------
// BELOW ARE THE FUNCTIONS FOR THE GATHERING
// -----------------------------------------

zframe_t            *gather_energy(t_server_info *server_info, t_player *player)
{
    t_energy_cell   *energy;

    if ((energy = search_energy_cell_by_pos(server_info->game_info.list_energy_cells, player->x, player->y)) != NULL) {
        player->energy += energy->value;
        server_info->game_info.list_energy_cells = remove_any_energy_cell(server_info->game_info.list_energy_cells, energy);
        return zframe_from("OK|player gathered energy");
    }
    return zframe_from("KO|no energy to gather");
}

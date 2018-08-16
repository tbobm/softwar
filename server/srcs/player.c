#include        "../includes/server.h"

t_player        *create_player(char *name, uint *data, t_player *next)
{
    t_player    *new_player = NULL;
    if ((new_player = (t_player*)malloc(sizeof(t_player))) == NULL)
    {
        printf("Error creating a new player.\n");
        exit(0);
    }
    new_player->name = name;
    new_player->x = data[0];
    new_player->y = data[1];
    new_player->energy = data[2];
    new_player->looking = data[3];
    new_player->next = next;
    return new_player;
}

t_player        *prepend(t_player *list_player, char *name, uint *data)
{
    return create_player(name, data, list_player);
}


void            display(t_player *list_player)
{
    t_player    *tmp = list_player;

    while (tmp != NULL)
    {
        printf(
            "Name : %s, Positions x, y : %u, %u, Energy : %u, Looking : %u\n",
            tmp->name, tmp->x, tmp->y, tmp->energy, tmp->looking
        );
        tmp = tmp->next;
    }
}

t_player        *search_by_name(t_player *list_player, char *name)
{
    t_player    *tmp = list_player;

    while (tmp != NULL)
    {
        if (strcmp(tmp->name, name) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

t_player        *search_by_pos(t_player *list_player, uint x, uint y)
{
    t_player    *tmp = list_player;

    while (tmp != NULL)
    {
        if (tmp->x == x && tmp->y == y)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int             count_players(t_player *list_player)
{
    t_player    *tmp = list_player;
    int         c = 0;

    while(tmp != NULL)
    {
        c++;
        tmp = tmp->next;
    }
    return c;
}

void            position_to_fill(t_server_info *server_info)
{
    int         x = 0;
    int         y = 0;
    int         looking = 3;

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

zframe_t        *move_up(t_server_info *server_info, t_player *player)
{
    if ((int)(player->y - 1) < 0) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x, player->y - 1) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->y -= 1;
    return zframe_from("OK|you moved up");
}

zframe_t        *move_down(t_server_info *server_info, t_player *player)
{
    if ((int)(player->y + 1) >= server_info->args->size) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x, player->y + 1) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->y += 1;
    return zframe_from("OK|you moved down");
}

zframe_t        *move_left(t_server_info *server_info, t_player *player)
{
    if ((int)(player->x - 1) < 0) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x - 1, player->y) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->x -= 1;
    return zframe_from("OK|you moved left");
}

zframe_t        *move_right(t_server_info *server_info, t_player *player)
{
    if ((int)(player->x + 1) >= server_info->args->size) {
        return zframe_from("KO|out of map");
    }
    if (search_by_pos(server_info->game_info.list_players, player->x + 1, player->y) != NULL) {
        return zframe_from("KO|another player is on this cell");
    }
    player->x += 1;
    return zframe_from("OK|you moved right");
}

zframe_t        *go_forward(t_server_info *server_info, t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            return move_left(server_info, player);
        case 1 :
            return move_up(server_info, player);
        case 2 :
            return move_right(server_info, player);
        case 3 :
            return move_down(server_info, player);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

zframe_t        *go_backward(t_server_info *server_info, t_player *player)
{
    switch (player->looking)
    {
        case 0 :
            return move_right(server_info, player);
        case 1 :
            return move_down(server_info, player);
        case 2 :
            return move_left(server_info, player);
        case 3 :
            return move_up(server_info, player);
    }
    return zframe_from("KO|looking is not 0, 1, 2 or 3");
}

void            rotate_left(t_player *player)
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

void            rotate_right(t_player *player)
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

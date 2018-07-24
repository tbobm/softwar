#include "../includes/server.h"

player	*create_player(char *name, uint *data, player *next) {
	player *new_player = NULL;
	if ((new_player = (player*)malloc(sizeof(player))) == NULL)
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

player 	*prepend(player *list_player, char *name, uint *data)
{
    player* new_player = create_player(name, data, list_player);
    return new_player;
}


void 	display(player *p)
{
	player *tmp = p;

    while (tmp != NULL)
    {
        printf(
        	"Name : %s, Positions x, y : %u, %u, Energy : %u, Looking : %u\n",
        	tmp->name, tmp->x, tmp->y, tmp->energy, tmp->looking
        );
        tmp = tmp->next;
    }
}

player* search(player *list_player, char *name)
{
    player *player = list_player;
    while (player != NULL)
    {
        if (strcmp(player->name, name) == 0)
            return player;
        player = player->next;
    }
    return NULL;
}
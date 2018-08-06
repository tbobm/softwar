#include "../includes/server.h"

t_player	*create_player(char *name, uint *data, t_player *next) {
	t_player *new_player = NULL;
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

t_player 	*prepend(t_player *list_player, char *name, uint *data)
{
    t_player* new_player = create_player(name, data, list_player);
    return new_player;
}


void 	display(t_player *p)
{
	t_player *tmp = p;

    while (tmp != NULL)
    {
        printf(
        	"Name : %s, Positions x, y : %u, %u, Energy : %u, Looking : %u\n",
        	tmp->name, tmp->x, tmp->y, tmp->energy, tmp->looking
        );
        tmp = tmp->next;
    }
}

t_player* search(t_player *list_player, char *name)
{
    t_player *player = list_player;
    while (player != NULL)
    {
        if (strcmp(player->name, name) == 0)
            return player;
        player = player->next;
    }
    return NULL;
}
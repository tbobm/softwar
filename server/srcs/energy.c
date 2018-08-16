#include            "../includes/server.h"

t_energy_cell       *create_energy_cell(t_energy_cell *next, uint *data)
{
    t_energy_cell   *new_energy_cell = NULL;
    if ((new_energy_cell = (t_energy_cell*)malloc(sizeof(t_energy_cell))) == NULL)
    {
        printf("Error creating a new energy_cell.\n");
        exit(0);
    }
    new_energy_cell->x = data[0];
    new_energy_cell->y = data[1];
    new_energy_cell->value = data[2];
    new_energy_cell->next = next;
    return new_energy_cell;
}

t_energy_cell       *prepend_energy_cell(t_energy_cell *list_energy_cell, uint *data)
{
    return create_energy_cell(list_energy_cell, data);
}


void                display_energy_cell(t_energy_cell *list_energy_cell)
{
    t_energy_cell   *tmp = list_energy_cell;

    while (tmp != NULL)
    {
        printf(
            "Positions x, y : %u, %u, Value : %u\n",
            tmp->x, tmp->y, tmp->value
        );
        tmp = tmp->next;
    }
}

t_energy_cell       *search_energy_cell_by_pos(t_energy_cell *list_energy_cell, uint x, uint y)
{
    t_energy_cell   *tmp = list_energy_cell;

    while (tmp != NULL)
    {
        if (tmp->x == x && tmp->y == y)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int                 count_energy_cells(t_energy_cell *list_energy_cell)
{
    t_energy_cell   *tmp = list_energy_cell;
    int             c = 0;

    while(tmp != NULL)
    {
        c++;
        tmp = tmp->next;
    }
    return c;
}

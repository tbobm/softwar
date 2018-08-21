#include            "../includes/server.h"

t_energy_cell       *create_energy_cell(t_energy_cell *next, uint *pos_and_value)
{
    t_energy_cell   *new_energy_cell = NULL;
    if ((new_energy_cell = (t_energy_cell*)malloc(sizeof(t_energy_cell))) == NULL)
    {
        printf("Error creating a new energy_cell.\n");
        exit(0);
    }
    new_energy_cell->x     = pos_and_value[0];
    new_energy_cell->y     = pos_and_value[1];
    new_energy_cell->value = pos_and_value[2];
    new_energy_cell->next  = next;
    return new_energy_cell;
}

t_energy_cell       *prepend_energy_cell(t_energy_cell *list_energy_cell, uint *pos_and_value)
{
    return create_energy_cell(list_energy_cell, pos_and_value);
}

void                generate_energy_cell(t_server_info *server_info)
{
    uint            pos_and_value[3];

    pos_and_value[0] = (uint)rand() % server_info->args->size; // random between 0 and map size
    pos_and_value[1] = (uint)rand() % server_info->args->size; // random between 0 and map size
    pos_and_value[2] = (uint)((rand() % 11) + 5);              // random between 5 and 15
    while (search_energy_cell_by_pos(server_info->game_info.list_energy_cells, pos_and_value[0], pos_and_value[1]) != NULL) {
        pos_and_value[0] = (uint)rand() % server_info->args->size;
        pos_and_value[1] = (uint)rand() % server_info->args->size;
    }
    server_info->game_info.list_energy_cells = prepend_energy_cell(server_info->game_info.list_energy_cells, pos_and_value);
}

t_energy_cell      *remove_first_energy_cell(t_energy_cell *list_energy_cell)
{
    if (list_energy_cell == NULL)
        return NULL;

    t_energy_cell *first_cell = list_energy_cell;
    list_energy_cell          = list_energy_cell->next;
    first_cell->next          = NULL;

    // If this is the last energy cell in the list
    if (first_cell == list_energy_cell)
        list_energy_cell = NULL;

    free(first_cell);
    return list_energy_cell;
}

t_energy_cell      *remove_last_energy_cell(t_energy_cell *list_energy_cell)
{
    if (list_energy_cell == NULL)
        return NULL;

    t_energy_cell *cursor    = list_energy_cell;
    t_energy_cell *last_cell = NULL;

    while (cursor->next != NULL)
    {
        last_cell = cursor;
        cursor    = cursor->next;
    }
    if (last_cell != NULL)
        last_cell->next = NULL;

    // If this is the last energy cell in the list
    if (cursor == list_energy_cell)
        list_energy_cell = NULL;

    free(cursor);
    return list_energy_cell;
}

t_energy_cell       *remove_any_energy_cell(t_energy_cell *list_energy_cell, t_energy_cell *energy_cell)
{
    // If the cell is the first in the list 
    if (energy_cell == list_energy_cell)
    {
        list_energy_cell = remove_first_energy_cell(list_energy_cell);
        return list_energy_cell;
    }

    /* If the energy_cell is the last in the list */
    if (energy_cell->next == NULL)
    {
        list_energy_cell = remove_last_energy_cell(list_energy_cell);
        return list_energy_cell;
    }

    // If the energy_cell is between other cells in the list 
    t_energy_cell *cursor = list_energy_cell;
    while (cursor != NULL)
    {
        if (cursor->next == energy_cell)
            break;
        cursor = cursor->next;
    }

    if (cursor != NULL)
    {
        t_energy_cell *tmp = cursor->next;
        cursor->next       = tmp->next;
        tmp->next          = NULL;
        free(tmp);
    }
    return list_energy_cell;
}

void                display_energy_cell(t_energy_cell *list_energy_cell)
{
    t_energy_cell   *tmp = list_energy_cell;

    printf("Energy cells list :\n");
    while (tmp != NULL)
    {
        printf(
            "  Positions x, y : %u, %u, Value : %u\n",
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

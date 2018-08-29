#include    		"../includes/server.h"

static zframe_t 	*find_cmd(t_server_info *server_info) {
    int 			i = 0;
	static const t_bind_cmd my_cmds[] = {
		{&identify, "identify"},
		{&forward, "forward"},
		{&backward, "backward"},
		{&leftfwd, "leftfwd"},
		{&rightfwd, "rightfwd"},
		{&right, "right"},
		{&left, "left"},
		{&looking, "looking"},
		{&gather, "gather"},
		{&watch, "watch"},
		{&attack, "attack"},
		{&selfid, "selfid"},
		{&selfstats, "selfstats"},
		{&inspect, "inspect"},
		{&next, "next"},
		{&jump, "jump"},
		{NULL, "NULL"}
	};
	while (strcmp(my_cmds[i].cmd_name, "NULL") != 0) {
		if (strcmp(server_info->parsed_cmd, my_cmds[i].cmd_name) == 0)
			return my_cmds[i].cmd_function(server_info);
		i++;
	}
	return zframe_from("KO");
}

static zframe_t 	*parse_client_req(t_server_info *server_info, char *identity, char *content) {
	int 			i = 0;

	if ((server_info->parsed_cmd = malloc(sizeof(char) * (strlen(content) + 1))) == NULL)
		exit(-1);
	else if ((server_info->parsed_param = malloc(sizeof(char) * (strlen(content) + 1))) == NULL)
		exit(-1);
	if (strchr(content, '|') == NULL)
		return zframe_from("KO");
	while (content[i] != '|')
	{
		server_info->parsed_cmd[i] = content[i];
		i++;
	}
	server_info->parsed_cmd[i] = '\0';
	server_info->parsed_param = (strchr(content, '|') + 1);
	server_info->identity = identity;
	printf("Parsed_cmd : %s\n", server_info->parsed_cmd);
	printf("Parsed_param : %s\n", server_info->parsed_param);
	return find_cmd(server_info);
}

static void 		init_server_info(t_server_info *server_info, t_args *args) {
	server_info->args = args;
	server_info->game_info.map_size = 0;
	server_info->game_info.game_status = 0;
	server_info->game_info.list_players = NULL;
	server_info->game_info.list_energy_cells = NULL;
	server_info->parsed_cmd = NULL;
	server_info->parsed_param = NULL;
	server_info->identity = NULL;
	server_info->nb_clients = 0;
	server_info->nb_energy = 0;
	server_info->player_info[0] = 0;
	server_info->player_info[1] = 0;
	server_info->player_info[2] = 50;
	server_info->player_info[3] = 0;
}

int         		message_client_server(t_args *arguments) {
    t_server_info 	server_info;
    pthread_t		pub_serv;

    init_server_info(&server_info, arguments);

	if (pthread_create(&pub_serv, NULL, pub_sub_worker, &server_info)) {
		printf("pthread_create\n");
		return EXIT_FAILURE;
	}

	zsock_t *router = zsock_new(ZMQ_ROUTER);
	zsock_bind(router, "tcp://*:%d", server_info.args->rep_port);

	while (!zsys_interrupted) {
		zmsg_t *message = zmsg_recv(router);

		zframe_t *identity = zmsg_pop(message);
		zframe_t *empty = zmsg_pop(message);
		zframe_t *content = zmsg_pop(message);

		zmsg_destroy(&message);
		printf("Content of message is : \"%s\" from Client : \"%s\"\n", zframe_strdup(content), zframe_strdup(identity));

		content = parse_client_req(&server_info, zframe_strdup(identity), zframe_strdup(content));

		display(server_info.game_info.list_players);
		display_energy_cell(server_info.game_info.list_energy_cells);

		printf("Response message is : %s\n\n", zframe_strdup(content));

		zmsg_t *response = zmsg_new();

		zmsg_prepend(response, &identity);
		zmsg_append(response, &empty);
		zmsg_append(response, &content);

		zmsg_send(&response, router);
		zmsg_destroy(&response);

		zframe_destroy(&identity);
		zframe_destroy(&empty);
		zframe_destroy(&content);
	}
	zsock_destroy(&router);
    return 0;
}

// -------------------------------------------
// BELOW ARE THE FUNCTIONS FOR THE JSON FORMAT
// -------------------------------------------

static void			flush_buffer(char *buff)
{
	int 			i;

	i = 0;
	while (buff[i] != '\0')
	{
		buff[i] = '\0';
		i++;
	}
}

static char 		*energy_cell_to_json(t_energy_cell *energy_cell)
{
	static char 	buff[100];

	flush_buffer(buff);
	sprintf(
		buff,
		"{\"x\":%u,\"y\":%u,\"value\":%u}",
		energy_cell->x,
		energy_cell->y,
		energy_cell->value
	);
	return (char*)buff;
}

static char 		*list_energy_cells_to_json(t_energy_cell *list_energy_cells)
{
	static char 	buff[1024];
    t_energy_cell   *energy_cell = list_energy_cells;

	flush_buffer(buff);
    if (energy_cell != NULL) {
	    while (energy_cell != NULL)
	    {
	    	if (strlen(buff) == 0) {
	    		sprintf(
					buff,
					"%s",
					energy_cell_to_json(energy_cell)
				);
			} else {
	    		sprintf(
					buff,
					"%s,%s",
					buff,
					energy_cell_to_json(energy_cell)
				);
	    	}
	        energy_cell = energy_cell->next;
	    }
	} else {
		sprintf(
			buff,
			"%s",
			""
		);
	}
	return (char*)buff;
}

static char 		*player_to_json(t_player *player)
{
	static char 	buff[100];

	flush_buffer(buff);
	sprintf(
		buff,
		"{\"name\":\"%s\",\"x\":%u,\"y\":%u,\"energy\":%u,\"looking\":%u,\"stun_duration\":%u}",
		player->name,
		player->x,
		player->y,
		player->energy,
		player->looking,
		player->stun_duration
	);
	return (char*)buff;
}

static char 		*list_players_to_json(t_player *list_players)
{
	static char 	buff[400];
    t_player        *player = list_players;

	flush_buffer(buff);
    while (player != NULL)
    {
    	if (strlen(buff) == 0) {
    		sprintf(
				buff,
				"%s",
				player_to_json(player)
			);
		} else {
    		sprintf(
				buff,
				"%s,%s",
				buff,
				player_to_json(player)
			);
    	}
        player = player->next;
    }
	return (char*)buff;
}

static char 		*game_info_to_json(t_game_info *game_info)
{
	static char 	buff[1024];

	flush_buffer(buff);
	sprintf(
		buff,
		"\"map_size\":%u,\"game_status\":%u,\"list_players\":[%s],\"list_energy_cells\":[%s]",
		game_info->map_size,
		game_info->game_status,
		list_players_to_json(game_info->list_players),
		list_energy_cells_to_json(game_info->list_energy_cells)
	);
	return (char*)buff;
}

static char 		*server_info_to_json(t_server_info *server_info, int event)
{
	static char 	buff[1024];

	flush_buffer(buff);
	switch (event) {
		case 0 :
			sprintf(buff, "{\"notification_type\":%d,\"data\":{%s}}", event, game_info_to_json(&server_info->game_info));
			return (char*)buff;
		default :
			sprintf(buff, "{\"notification_type\":%d,\"data\":null}", event);
			return (char*)buff;
	}
}

void 				*pub_sub_worker(void *serv)
{
	t_server_info 	*server = (t_server_info*)serv;
	int 			nb_clients = 0;
	int 			game_start = 0;

	zsock_t *chat_server = zsock_new(ZMQ_PUB);
	zsock_bind(chat_server, "tcp://*:%d", server->args->pub_port);

	if (server->game_info.game_status == 0) {
	    printf("Publisher is waiting for game to start.\n");
	}

    while (1)
    {
    	usleep(server->args->cycle);
		// Waiting for game to start
		if (server->game_info.game_status == 1) {
			// Checking if game is over
			if (server->nb_clients <= 1) {
				server->game_info.game_status = 2;
			    zstr_sendf(chat_server, "#event:%s", server_info_to_json(server, 2));
			    zstr_sendf(chat_server, "#event:%s", server_info_to_json(server, 4));
				printf("Game Over\n");
				break;
			}

			// Doing stuff for each cycle
			nb_clients = server->nb_clients;
			generate_energy_cell(server);
			cycle_energy_loss(server->game_info.list_players);
			reset_action(server->game_info.list_players);
			decrement_stun_duration(server->game_info.list_players);
			server->nb_clients = count_players_alive(server);
			if (game_start == 0) {
			    zstr_sendf(chat_server, "#event:%s", server_info_to_json(server, 1));
			    game_start = 1;
			} else if (nb_clients != server->nb_clients) {
			    zstr_sendf(chat_server, "#event:%s", server_info_to_json(server, 3));
			}
			zstr_sendf(chat_server, "#all:%s", server_info_to_json(server, 0));
			printf("Sent to #all : \n%s\n\n", server_info_to_json(server, 0));

			// Display to see what has changed
			display(server->game_info.list_players);
			display_energy_cell(server->game_info.list_energy_cells);
		}
    }
    pthread_exit(NULL);
}

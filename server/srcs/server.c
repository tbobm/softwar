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
	server_info->player_info[0] = 0;
	server_info->player_info[1] = 0;
	server_info->player_info[2] = 50;
	server_info->player_info[3] = 0;
}

int         		message_client_server(t_args *arguments) {
    t_server_info 	server_info;

    init_server_info(&server_info, arguments);
	generate_energy_cell(&server_info);

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

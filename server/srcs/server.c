#include    "../includes/server.h"

// Function Pointer Array for RFC
static const t_cmd g_cmd[] = {
	&identify,
	&forward,
	&backward,
	&leftfwd,
	&rightfwd,
	&right,
	&left,
	&looking,
	&gather,
	&watch,
	&attack,
	&selfid,
	&selfstats,
	&inspect,
	&next,
	&jump
};

static zframe_t *find_cmd(char *cmd, char *param) {
    int i = 0;
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
		if (strcmp(cmd, my_cmds[i].cmd_name) == 0)
			return (my_cmds[i].cmd_function(param));
		i++;
	}
	return (zframe_from("KO"));
}

static zframe_t *parse_client_req(char *content) {
	int i = 0;
	char *cmd;

	if ((cmd = malloc(sizeof(char) * (strlen(content) + 1))) == NULL)
		exit(-1);
	printf("Content : %s\n", content);
	if (strchr(content, '|') == NULL)
		return (zframe_from("KO"));
	while (content[i] != '|')
	{
		cmd[i] = content[i];
		i++;
	}
	cmd[i] = '\0';
	return (find_cmd(cmd, (strchr(content, '|') + 1)));
}

int         manage_server(ARGS *arguments) {
    (void) 	arguments;
	t_player 	*list_player = NULL;
	uint	data[4] = {1, 2, 3, 4};
	int 	nb_client = 0;

	zsock_t *router = zsock_new(ZMQ_ROUTER);
	zsock_bind(router, "tcp://*:5555");

	while (!zsys_interrupted) {
		zmsg_t *message = zmsg_recv(router);

		zframe_t *identity = zmsg_pop(message);
		zframe_t *empty = zmsg_pop(message);
		zframe_t *content = zmsg_pop(message);

		zmsg_destroy(&message);
		printf("Content of message is : \"%s\" from : %s\n", zframe_strdup(content), zframe_strdup(identity));

		content = parse_client_req(zframe_strdup(content));

		// if ((search(list_player, zframe_strdup(identity)) == NULL) && nb_client >= 4) {
		// 	printf("'%s' Can't go in list_player, because list is full.\n", zframe_strdup(identity));
		// 	content = zframe_from("List is full, you're OUT !");
		// } else if (search(list_player, zframe_strdup(identity)) == NULL) {
		// 	printf("'%s' added to list_player.\n", zframe_strdup(identity));
		// 	list_player = prepend(list_player, zframe_strdup(identity), data);
		// 	content = zframe_from("You are in !");
		// 	nb_client++;
		// } else {
		// 	content = zframe_from("You are ALREADY in !");
		// }
		display(list_player);

		printf("Response message is : %s\n\n", zframe_strdup(content));

		sleep(1);

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
    // prepare_resources();
    return 0;
}

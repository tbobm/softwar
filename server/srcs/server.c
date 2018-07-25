#include    "../includes/server.h"

int         manage_server(ARGS *arguments) {
    (void) 	arguments;
	player 	*list_player = NULL;
	uint	data[4] = {1, 2, 3, 4};
	int 	nb_client = 0;

	zsock_t *router = zsock_new(ZMQ_ROUTER);
	zsock_bind(router, "tcp://*:5555");

	while (!zsys_interrupted) {
		zmsg_t *message = zmsg_recv(router);

		zframe_t *identity = zmsg_pop(message);
		zframe_t *identity2 = zframe_from("test");
		zframe_t *empty = zmsg_pop(message);
		zframe_t *content = zmsg_pop(message);

		zmsg_destroy(&message);
		printf("Content of message is : %s from : %s\n", zframe_strdup(content), zframe_strdup(identity));

		if ((search(list_player, zframe_strdup(identity)) == NULL) && nb_client >= 4) {
			printf("'%s' Can't go in list_player, because list is full.\n", zframe_strdup(identity));
			content = zframe_from("List is full, you're OUT !");
		} else if (search(list_player, zframe_strdup(identity)) == NULL) {
			printf("'%s' added to list_player.\n", zframe_strdup(identity));
			list_player = prepend(list_player, zframe_strdup(identity), data);
			content = zframe_from("You are in !");
			nb_client++;
		} else {
			content = zframe_from("You are ALREADY in !");
		}
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

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
		printf("Content of message is : %s\n %s\n", zframe_strdup(content), zframe_strdup(identity));

		printf("Message de %s on va ajouter son identité dans list_player.\n", zframe_strdup(identity));
		if (search(list_player, zframe_strdup(identity)) == NULL) {
			list_player = prepend(list_player, zframe_strdup(identity), data);
		}
		display(list_player);

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

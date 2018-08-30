#include <stdio.h>
#include <czmq.h>

// run with gcc -lczmq -o pub publisher.c
int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("Port number is mandatory\n");
    return 0;
  }

  zsock_t *chat_srv_socket = zsock_new(ZMQ_PUB);
  zsock_bind(chat_srv_socket, "tcp://*:%s", argv[1]);
  printf("Server listening on tcp://*:%s", argv[1]);

  while (!zsys_interrupted) {
    char message[1024];

    if (!fgets(message, 1024, stdin)) {
      break;
    }
    zstr_sendf(chat_srv_socket, "%s", message);
  }
  zsock_destroy(&chat_srv_socket);
  return 0;

}

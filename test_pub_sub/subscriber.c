#include <stdio.h>
#include <czmq.h>

// run with gcc -lczmq sub subscriber.c
int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("Port number and channel are mandatory\n");
    return 0;
  }

  zsock_t *client = zsock_new(ZMQ_SUB);
  zsock_connect(client, "tcp://127.0.0.1:%s", argv[1]);
  printf("Client connected to 127.0.0.1:%s\n", argv[1]);

  zsock_set_subscribe(client, argv[2]);
  zsock_set_subscribe(client, "#general:");

  while (!zsys_interrupted) {
    char *message = zstr_recv(client);
    printf("%s", message);
    zstr_free(&message);
  }

  zsock_destroy(&client);
  return 0;
}

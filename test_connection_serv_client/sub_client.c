#include <czmq.h>

int main(void)
{
  zsock_t *client = zsock_new(ZMQ_SUB);
  zsock_connect(client, "tcp://127.0.0.1:4243");
  printf("Client connected to 127.0.0.1:4243\n");

  zsock_set_subscribe(client, "#all:");

  while (!zsys_interrupted) {
    char *message = zstr_recv(client);
    printf("%s\n", message);
    zstr_free(&message);
  }

  zsock_destroy(&client);
  return 0;
}

#include <czmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
  printf("Connecting to echo...\n");
  zsock_t *req = zsock_new(ZMQ_REQ);
  zsock_set_identity(req, "client1");
  zsock_connect(req, "tcp://localhost:4242");

  int request_nbr;
  for (request_nbr = 0; request_nbr != 1; request_nbr++) {
    zstr_sendf(req, "selfstats|");

    char *message = zstr_recv(req);
    printf("Received : %s\n", message);
    zstr_free(&message);
  }
  zsock_destroy(&req);
  return 0;
}

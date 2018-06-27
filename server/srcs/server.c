#include	"../includes/server.h"

int		manage_server(ARGS *arguments) {
  (void) arguments;
  // prepare_resources();
  if (start_server(arguments) != 0)
    return 1;
  return 0;
}

int		start_server(ARGS *arguments) {
  (void) arguments;
  void *context = zmq_ctx_new();
  void *responder = zmq_socket (context, ZMQ_REP);
  int rc = zmq_bind (responder, "tcp://*:5555");
  assert (rc == 0);

  while (1) {
    char buffer [10];
    zmq_recv (responder, buffer, 10, 0);
    printf ("Received Hello\n");
    sleep (1);
    zmq_send (responder, "World", 5, 0);
  }
  return 0;
}

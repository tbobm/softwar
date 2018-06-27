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
  return 0;
}

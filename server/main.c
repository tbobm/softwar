#include    "includes/server.h"

int	        main(int argc, char **argv) {
    t_args  arguments;
    int	    check;
    
    if ((check = parse_args(argc, argv, &arguments)) == -1) {
        fprintf(stderr, "%s\n", USAGE);
        return 1;
    } else if (check == -2) {
        fprintf(stderr, "%s%s\n", BAD_ARGS, USAGE);
        return 1;
    }

    srand(time(NULL)); // For the generation of energy cells
    if (message_client_server(&arguments) != 0) {
        fprintf(stderr, "%s\n", ERR_SERVER);
        return 1;
    }
    return 0;
}

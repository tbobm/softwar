#include    "../includes/server.h"

int         main(int argc, char **argv) {
    ARGS    arguments;
    int     check;
    
    if ((check = parse_args(argc, argv, &arguments)) == -1) {
        fprintf(stderr, "%s\n", USAGE);
        return 1;
    } else if (check == -2) {
        fprintf(stderr, "%s%s\n", BAD_ARGS, USAGE);
        return 1;
    }
    return 0;
}

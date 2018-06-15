#include "includes/server.h"

int         main(int argc, char** argv) {
    ARGS    arguments;

    if (parse_args(argc, argv, &arguments) != 0)
    {
        fprintf(stderr, "Usage:%s\n", USAGE);
        return 1;
    }
    return 0;
}


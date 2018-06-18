#include        "../includes/args.h"

static void     init_args(ARGS *arguments) {
    arguments->rep_port = 4242;
    arguments->pub_port = 4243;
    arguments->cycle = 1000000;
    arguments->v = 0;
    arguments->log = "/tmp/softwar.log";
    arguments->size = 5;
}

static void     display_args(ARGS *arguments) {
    fprintf (stdout,
    "%srep_port = %d\npub_port = %d\ncycle\t = %d\nv\t = %d\nlog\t = %s\nsize\t = %d\n",
    "Voici la configuration du serveur, grâce à vos arguments et/ou aux paramètres par défaut :\n",
    arguments->rep_port,
    arguments->pub_port,
    arguments->cycle,
    arguments->v,
    arguments->log,
    arguments->size);
}

int             parse_args(int argc, char **argv, ARGS *arguments) {
    int         c;

    init_args(arguments);
    fprintf(stderr, "\nDébut du parsing des arguments...\n");
    while (1) {
        static struct option long_options[] = {
            {"rep-port",    required_argument,  0,  'r'},
            {"pub-port",    required_argument,  0,  'p'},
            {"cycle",       required_argument,  0,  'c'},
            {"v",           no_argument,        0,  'v'},
            {"log",         required_argument,  0,  'l'},
            {"size",        required_argument,  0,  's'},
            {"h",           no_argument,        0,  'h'},
            {0, 0, 0, 0}
        };
        int option_index = 0;
        c = getopt_long(argc, argv, "r:p:c:vl:s:h", long_options, &option_index);

        if (c == -1) {
            fprintf(stderr, "Parsing des arguments terminé.\n\n");
            break;
        }
        
        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
                fprintf(stdout, "Option %s", long_options[option_index].name);
                if (optarg)
                    fprintf(stdout, " with arg %s", optarg);
                fprintf(stdout, "\n");
                break;
            case 'r':
                fprintf(stdout, "Option --rep-port, valeur : %s\n", optarg);
                arguments->rep_port = atoi(optarg);
                break;
            case 'p':
                fprintf(stdout, "Option --pub-port, valeur : %s\n", optarg);
                arguments->pub_port = atoi(optarg);
                break;
            case 'c':
                fprintf(stdout, "Option --cycle,    valeur : %s\n", optarg);
                arguments->cycle = atoi(optarg);
                break;
            case 'v':
                fprintf(stdout, "Option -v enabled.\n");
                arguments->v = 1;
                break;
            case 'l':
                fprintf(stdout, "Option --log,\t   valeur : %s\n", optarg);
                arguments->log = optarg;
                break;
            case 's':
                fprintf(stdout, "Option --size,\t   valeur : %s\n", optarg);
                if (atoi(optarg) >= 5) {
                    arguments->size = atoi(optarg);
                } else {
                    return -2;
                }
                break;
            case '?':
                /* getopt_long already printed an error message. */
                break;
            default:
                return -1;
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("Ceci n'est pas une option : ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        fprintf(stdout, "\n");
        return -1;
    }

    display_args(arguments);

    return 0;
}
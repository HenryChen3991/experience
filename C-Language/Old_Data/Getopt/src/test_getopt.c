#include <unistd.h>  /* getopt */
#include <stdio.h>

int main(int argc, char **argv)
{
    int cmd_opt = 0;

    fprintf(stderr, "argc:%d\n", argc);
    while(1) {
        fprintf(stderr, "proces index:%d\n", optind);
        cmd_opt = getopt(argc, argv, "abc:d:e::");

        /* End condition always first */
        if (cmd_opt == -1) {
            break;
        }

        /* Print option when it is valid */
        if (cmd_opt != '?') {
            fprintf(stderr, "option:-%c\n", cmd_opt);
        }

        /* Lets parse */
        switch (cmd_opt) {
            /* No args */
            case 'a':
            case 'b':
                break;

            /* Single arg */
            case 'c':
            case 'd':
                fprintf(stderr, "option arg:%s\n", optarg);
                break;

            /* Optional args */
            case 'e':
                if (optarg) {
                    fprintf(stderr, "option arg:%s\n", optarg);
                }
                break;


            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                fprintf(stderr, "Illegal option:-%c\n", isprint(optopt)?optopt:'#');
                break;
            default:
                fprintf(stderr, "Not supported option\n");
                break;
        }
    }

    /* Do we have args? */
    if (argc > optind) {
        int i = 0;
        for (i = optind; i < argc; i++) {
            fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
        }
    }

    return 0;
}
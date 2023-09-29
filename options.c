#include "options.h"

void
parse_options (int argc, char **argv, struct options* opts)
{
    /* Check options.  */
    bool valid = true;
    int opt;
    while((opt = getopt(argc, argv, "i:o:")) != -1)
    {
        switch(opt)
        {
            case 'i':
            {
                opts->iflag = optarg;

                if (!(strcmp(opts->iflag, "mrand48_r") == 0) &&
                    !(strcmp(opts->iflag, "rdrand") == 0) &&
                    !(*(opts->iflag) == '/')) {
                    valid = false;
                }
                break;
            }
            case 'o':
            {
                /* If -o trailing argument is not 'stdio', check that it is valid.  */
                if (!(strcmp(optarg, "stdio") == 0)) {
                    char *endptr;
                    int temp;
                    errno = 0;
                    temp = strtoll (optarg, &endptr, 10);
                    if (errno)
                    perror (optarg);
                    else {}
                    valid = !*endptr && 0 < temp;

                    if (valid) {
                        opts->N = temp;
                    }
                    else {
                    }
                }
                break;
            }
            case '?':
            {
                exit(1);
            }
        }
    }

    /* Check arguments.  */
    if (argc >= 2) {
        if (argv[optind] == NULL) {
            valid = false;
        } else {
            char *endptr;
            errno = 0;
            opts->nbytes = strtoll (argv[optind], &endptr, 10);
            if (errno)
            perror (argv[optind]);
            else
            valid = valid && !*endptr && 0 <= opts->nbytes;
        }
    } else {
        valid = false;
    }

    /* Check validity of arguments and options. */
    if (!valid)
    {
        fprintf (stderr, "%s: usage: %s NBYTES -i input -o output\n", argv[0], argv[0]);
        exit(1);
    }
}

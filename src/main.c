#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "test/tests.h"
#include "util/aliases.h"


i32 main(i32 argc, char *argv[])
{
        char *arg = NULL;

        if (argc != 2) {
                /* print help/usage message */
                printf("main: invalid number of arguments\n");
                exit(EXIT_FAILURE);
        }

        arg = argv[1];
        if (!arg) {
                printf("main: argv[1] is null\n");
                exit(EXIT_FAILURE);
        }

        if (!strncmp("test", arg, 4)) {
                xstring_run_tests();
        }


        return (EXIT_SUCCESS);
}

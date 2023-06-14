#include <syscall.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


extern int main(int argc, char **argv);
int entry2(int argc, char **argv);

int entry(int argc, char **argv) {
    return entry2(argc, argv);
}

int entry2(int argc, char **argv) {
    // patch argv

    int new_argc = argc - 1;
    if (new_argc < 1) new_argc = 1;
    char **new_argv = malloc(sizeof(char *) * (new_argc + 1));
    printf("new_argc = %d\n", new_argc);
    for (int i = 0; i <= new_argc; i++) {
        if (i == 0) {
            new_argv[i] = malloc(5);
            strcpy(new_argv[i], "sulfur");
        } else if (i == new_argc) {
            printf("argv[%d] = NULL\n", i);
            new_argv[i] = NULL;
            break;
        } else {
            new_argv[i] = argv[i + 1];
        }
        printf("argv[%d] = %s\n", i, new_argv[i]);
    }

    main(new_argc, new_argv);

    // free memory
    int freed = c_mem_free_all(c_process_get_pid());
    printf("%d alloc freed\n", freed);

    return 0;
}

long long __moddi3(long long a, long long b) {
    return a % b;
}

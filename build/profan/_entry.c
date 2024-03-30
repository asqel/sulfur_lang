#include <syscall.h>
#include <stdlib.h>
#include <string.h>
#include <profan.h>
#include <stdio.h>


extern int main(int argc, char **argv);

int entry(int argc, char **argv, char **envp) {
    // init the environ pointer
    init_environ_ptr(envp);

    // we need to call a other entry function
    int exit_code = main(argc, argv);

    // free memory
    profan_cleanup();

    return exit_code;
}

long long __moddi3(long long a, long long b) {
    return (long long) ((int) a % (int) b);
}

long long __divdi3(long long a, long long b) {
    return (long long) ((int) a / (int) b);
}

#include <syscall.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


extern int main(int argc, char **argv);
int entry2(int argc, char **argv);

int entry(int argc, char **argv) {
    return entry2(argc, argv);
}

char *current_dir;

int entry2(int argc, char **argv) {
    current_dir = getenv("PWD");

    main(argc, argv);

    // free memory
    int freed = c_mem_free_all(c_process_get_pid());
    printf("$6%d alloc freed$7\n", freed);

    return 0;
}

long long __moddi3(long long a, long long b) {
    return a % b;
}

long long __divdi3(long long a, long long b) {
    return a / b;
}

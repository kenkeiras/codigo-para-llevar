#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_PROGRAM_MEMORY 4096

typedef struct {
    int pc; // Program counter
    int mp; // Memory pointer
    char mem[MAX_PROGRAM_MEMORY]; // Memory
    char* program;
} state;

int is_same_state(state s1, state s2);
int has_finished(state s);
state step(state s);

int check_halts(char* program) {
    // Initialize hare
    state hare = { .pc = 0, .mp = 0, .program=program };
    bzero(hare.mem, MAX_PROGRAM_MEMORY);

    // Initialize tortoise
    state tortoise = { .pc = 0, .mp = 0, .program=program };
    bzero(tortoise.mem, MAX_PROGRAM_MEMORY);

    // Prepare to start
    assert(is_same_state(hare, tortoise));

    // Run hare two steps for each tortoise's step until either hare finishes or
    // the hare meets the tortoise again.
    tortoise = step(tortoise);
    hare = step(step(hare));

    while ((!is_same_state(hare, tortoise)) && (!has_finished(hare))) {
        assert(!has_finished(tortoise)); // Hare should always finish faster

        tortoise = step(tortoise);
        hare = step(step(hare));
    }

    if (has_finished(hare)) {
        return 1; // It does halt
    }
    else {
        assert(is_same_state(hare, tortoise));
        return 0; // It won't halt
    }
}

int has_finished(state s) {
    return s.program[s.pc] == '\0';
}

state step(state s) {
    char op = s.program[s.pc];
    switch(op) {
    case '\0':
        break; // Finished, won't change

    case '+':
        s.mem[s.mp]++;
        s.pc++;
        break;

    case '-':
        s.mem[s.mp]--;
        s.pc++;
        break;

    case '<':
        s.mp--;
        assert((s.mp >= 0) && (s.mp < MAX_PROGRAM_MEMORY));
        s.pc++;
        break;

    case '>':
        s.mp++;
        assert((s.mp >= 0) && (s.mp < MAX_PROGRAM_MEMORY));
        s.pc++;
        break;

    case '.':
        // putchar(s.mem[s.mp]); // Uncomment for printing
        s.pc++;
        break;

    case '[':
    { // Find matching ']' going forwards
        int matching_seeker = s.pc;
        int count = 0;
        while (count >= 0) {
            matching_seeker++;
            char seeker_op = s.program[matching_seeker];
            assert(seeker_op!= '\0');
            if (seeker_op == '[') {
                count++;
            }
            else if (seeker_op == ']') {
                count--;
            }
        }
        if (s.mem[s.mp] == 0) {
            s.pc = matching_seeker + 1;
        }
        else {
            s.pc++;
        }
    }
    break;

    case ']':
    { // Find matching '[' going backwards
        int matching_seeker = s.pc;
        int count = 0;
        while (count >= 0) {
            matching_seeker--;
            assert(matching_seeker >= 0);
            char seeker_op = s.program[matching_seeker];
            if (seeker_op == ']') {
                count++;
            }
            else if (seeker_op == '[') {
                count--;
            }
        }
        s.pc = matching_seeker; // Move to matching [
    }
    break;
    }

    return s;
}

int is_same_state(state s1, state s2) {
    if ((s1.pc != s2.pc) || (s1.mp != s2.mp)) {
        return 0;
    }

    return memcmp(s1.mem, s2.mem, MAX_PROGRAM_MEMORY) == 0;
}

int is_program_char(char c) {
    switch (c) {
    case ',':
        printf("Console input not supported :(\n");
        abort();
        break;
    case '.':
    case '[': case ']':
    case '+': case '-':
    case '<': case '>':
        return 1;

    default:
        return 0;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "%s <program.bf>\n", argv[0]);
        return 2;
    }

    // Read program
    char* fname = argv[1];
    FILE* f = fopen(fname, "rt");
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f);
    rewind(f);

    char* program = malloc(file_size);
    int i;
    for (i = 0;;) {
        int c = fgetc(f);
        if (c == EOF) {
            break;
        }

        if (is_program_char((char) c)) {
            program[i++] = (char) c;
        }
    }

    program[i] = '\0';
    fprintf(stderr, "Program [size=%i] %s\n", i, program);

    if (check_halts(program)) {
        printf("HALTS\n");
        return 0;
    }
    else {
        printf("LOOPS\n");
        return 1;
    }
}

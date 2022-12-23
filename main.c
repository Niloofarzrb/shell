#include <stdio.h>
#include <unistd.h> // includes fork(), execvp()
#include <string.h>
#include <ctype.h> // includes atoi
#include <sys/types.h> // include pid_t
#include <sys/wait.h> // includes wait()
#include <malloc.h>
#include <stdlib.h> // contains atoi(), exit()
#include <stdbool.h>

#define MAX_LINE        80 /* 80 chars per line, per input */
// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")


void init_shell() {
    clear();
    printf("\n\n******************"
           "************************");
    printf("\n\n\t****Project 1****");
    printf("\n\n*******************"
           "***********************");
    char *username = getenv("USER");
    printf("\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

void help() {

}

int execute(char *input) {

    char *args[MAX_LINE / 2 + 1];    /* input line (of 80) has max of 40 arguments */
    char *words;
    bool isWaiting = true;
    int i, getVal;


    if (input[0] == 'c' && input[1] == 'd') {

    }

    if (!strcmp(input, "hints")) {
        printf("%s", help);
        fflush(stdin);
        fflush(stdout);
        return 1;
    }

    // invoke execvp()
    getVal = execvp(args[0], args);

    //else return 0;
    if (isWaiting == false) return 2;

    return getVal; // success

}


int main() {
    FILE *f; // this file will save history

    bool should_run = true;
    int i, getVal;
    int lastPos; // will be required for waiting / not-waiting child process to end

    pid_t pid;
    char inp[MAX_LINE];

    init_shell();

    while (should_run) {
        printf("\nuser> ");
        fflush(stdout);  //flushes the output buffer of a stream

        // take the string input, and update history (if it's not invoking a previous input through '!')
        gets(inp);

        if (!strcmp(inp, "up")) {
            should_run = false;
            exit(0);
        }
        //if (inp[0] != '!') { updateHistory(inp); }

        // fork a child process
        pid = fork();

        if (pid < 0) // error
        {
            fprintf(stderr, "System error\n");
            return 1;
        } else if (pid == 0) // child process
        {

            getVal = execute(inp);

            if (getVal == 1) continue;
            if (getVal == -1)
                printf("Sorry, that input is not supported in this shell.");
        } else // parent
        {
            // Wait for child to finish execution unless user ends with '&' (reversing the original part of the problem)
            lastPos = strlen(inp) - 1;
            if (inp[lastPos] != '&') wait(NULL);
        }
    }

    return 0;
}



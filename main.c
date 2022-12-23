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
    sleep(2);
    clear();
}

void help() {

    printf("\nSFW : Splits the first word of a line\n"
           "MRS : Returns the most repeated world in a file\n"
           "DAS : Deletes all spaces and prints the file\n"
           "SNC : Shows all lines that are not comment. comments represented with #\n"
           "SNL : Shows the number of lines\n"
           "STFL : Shows ten first line in a file");
}

int execute(char *input) {

    char *args[MAX_LINE / 2 + 1];    /* input line (of 80) has max of 40 arguments */
    char *words;
    bool isWaiting = true;
    int i, getVal;
    words = strtok(input, " ");
    int j;

    i = 0;
    while (words != NULL) {
        if (strcmp(words, "&"))
            args[i] = words;
        i++;
        words = strtok(NULL, " ");
    }
    args[i] = NULL;
    if (!strcmp(args[0], "help")) {
        help();
    }
    else if (!strcmp(args[0], "cd")) {
        //printf("0");
        if (!strcmp(input[2], NULL)) {
            //printf("1");
        } else {
            //printf("2");
            chdir(args[1]);
        }
    }
    else {
        printf("err");
    }

    // invoke execvp()
    getVal = execvp(args[0], args);

    //else return 0;
    if (isWaiting == false) return 2;
    int k;
    for(k=0; k< sizeof(args); k++){
        args[k] = NULL;
    }

    return getVal; // success

}


int main() {
    FILE *f; // this file will save history

    bool should_run = true;
    int i, getVal;
    int lastPos; // will be required for waiting / not-waiting child process to end

    pid_t pid;
    char inp[MAX_LINE];
    char location[MAX_LINE];
    init_shell();
    getcwd(location, MAX_LINE);
    while (should_run) {
        printf("\nuser%s ", location);
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
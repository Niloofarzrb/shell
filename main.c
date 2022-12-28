#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<signal.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

#define clear() printf("\033[H\033[J")

void init_shell() {
    clear();
    printf("\n\n*************************************"
           "************************");
    printf("\n\n\t*Operating Sysetm - Midterm Project*");
    printf("\n\n\t*Niloofar Zarabiyan && Kourosh Hassanzadeh*");
    printf("\n\n*************************************"
           "***********************");
    char *username = getenv("USER");
    printf("\n\nUSER is: @%s", username);
    printf("\n");
    sleep(2);
    clear();
}

int takeInput(char *str) {
    char *buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

void printDir() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

void execArgs(char **parsed) {

    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            fprintf(stderr, "\nCould not execute command..");
        }
        exit(0);
    } else {
        wait(NULL);
        return;
    }
}

void help() {
    puts("\nSFW : Splits the first word of a line\n"
         "MRW : Returns the most repeated word in a file\n"
         "DAS : Deletes all spaces and prints the file\n"
         "SNC : Shows all lines that are not comment. comments represented with #\n"
         "SNL : Shows the number of lines\n"
         "STFL : Shows ten first line in a file");

    return;
}

void splitFirstWord(char *name) {
    printf("\n");
    FILE *file;
    file = fopen(name, "r");
    char *line;
    size_t size = 0;
    while (getline(&line, &size, file) != -1) {
        printf("%s \n", strtok(line, " "));
    }
    fclose(file);

}

void mostRepeatedWord(char *name) {

    char *command = "bash MRW.sh ";
    char *newString = malloc(strlen(command) + strlen(name) + 1);
    printf("\n");
    strcpy(newString, command);
    strcat(newString, name);
    system(newString);

}

void deleteSpaces(char *name) {

    FILE *file;
    char line[1000];
    int fileChar;
    printf("\n");
    file = fopen(name, "r");
    if (file) {
        fgets(line, 1000, file);
        fseek(file, 0, SEEK_SET);

        do {
            fileChar = fgetc(file);
            if (isgraph(fileChar)) {
                putchar(fileChar);
            }
        } while (fileChar != EOF);
        fclose(file);
    }
    printf("\n");

}

void showNoneComments(char *name) {
    printf("\n");
    FILE *file;
    file = fopen(name, "r");
    char line[MAXLIST];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') {
            continue;
        } else {
            printf("%s", line);
        }
    }
    fclose(file);
}

void showNumberOfLines(char *name) {
    printf("\n");
    FILE *file;
    file = fopen(name, "r");
    char line[MAXLIST];
    int counter = 0;
    while (fgets(line, sizeof(line), file)) {
        counter++;
    }
    printf("%d\n", counter);
    fclose(file);

}

void showTenFirstLine(char *name) {
    printf("\n");
    FILE *file;
    file = fopen(name, "r");
    char line[MAXLIST];
    int counter = 0;
    while (fgets(line, sizeof(line), file)) {
        counter++;
        if (counter <= 10) {
            printf("%s", line);
        } else {
            break;
        }
    }
    fclose(file);

}

int commandHandler(char **parsed) {
    int numberOfCommands = 8, i, switchOwnArg = 0;
    char *commandList[numberOfCommands];

    commandList[0] = "help";
    commandList[1] = "cd";
    commandList[2] = "SFW";
    commandList[3] = "MRW";
    commandList[4] = "DAS";
    commandList[5] = "SNC";
    commandList[6] = "SNL";
    commandList[7] = "STFL";

    for (i = 0; i < numberOfCommands; i++) {
        if (strcmp(parsed[0], commandList[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        switch (switchOwnArg) {
            case 1:
                help();
                return 1;
            case 2:
                chdir(parsed[1]);
                return 1;
            case 3:
                splitFirstWord(parsed[1]);
                return 1;
            case 4:
                mostRepeatedWord(parsed[1]);
                return 1;
            case 5:
                deleteSpaces(parsed[1]);
                return 1;
            case 6:
                showNoneComments(parsed[1]);
                return 1;
            case 7:
                showNumberOfLines(parsed[1]);
                return 1;
            case 8:
                showTenFirstLine(parsed[1]);
                return 1;
            default:
                break;
        }
        exit(0);
    } else if (pid < 0) {
        fprintf(stderr, "fork was not successful");
    } else {
        wait(NULL);
    }
    return 0;
}

void parseSpace(char *str, char **parsed) {
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char *str, char **parsed) {

    parseSpace(str, parsed);

    if (commandHandler(parsed))
        return 0;
    else
        return 1;
}

void addToHistory(char *input) {

    FILE *history = fopen("/home/kourosh/Desktop/history.txt", "a+");
    fprintf(history, "%s\n", input);
    fclose(history);
}

void sigintHandler(int sig_num) {

    signal(SIGINT, sigintHandler);
    fflush(stdin);
    return;
}

int main() {
    signal(SIGINT, sigintHandler);
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    int execFlag = 0;
    FILE *history;
    init_shell();
    history = fopen("/home/kourosh/Desktop/history.txt", "w+");
    fclose(history);
    while (1) {

        printDir();

        if (takeInput(inputString))
            continue;

        addToHistory(inputString);

        execFlag = processString(inputString, parsedArgs);

        if (execFlag == 1)
            execArgs(parsedArgs);

    }

    return 0;
}

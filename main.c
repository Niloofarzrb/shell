// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
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

// Function to take input
int takeInput(char* str)
{
    char* buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

// Help command builtin
void help()
{
    puts("\nSFW : Splits the first word of a line\n"
         "MRW : Returns the most repeated word in a file\n"
         "DAS : Deletes all spaces and prints the file\n"
         "SNC : Shows all lines that are not comment. comments represented with #\n"
         "SNL : Shows the number of lines\n"
         "STFL : Shows ten first line in a file");

    return;
}

void splitFirstWord(char *name){

    FILE *file;
    file= fopen(name, "r");
    char *line;
    size_t size=0;
    while(getline(&line, &size, file)!=-1){
        printf("%s \n" , strtok(line, " "));
   }
    fclose(file);

}

void mostRepeatedWord(char *name){
    //command = esme dastur    cat beshe ba .sh    cat beshe ba      cat beshe ba bash
    char *command = "bash MRW.sh ";
    char *newString = malloc(strlen(command) + strlen(name) + 1);
    printf("\n");
    strcpy(newString, command);
    strcat(newString, name);
    system(newString);
    //bashHandler(command, name);

}

void deleteSpaces(char *name){
	FILE *file;
	char line[1000];
	int fileChar;
	printf("\n");

	file = fopen(name, "r");
	if(file){
	   fgets(line , 1000 , file);
	   fseek(file, 0, SEEK_SET);

	   do{

	      fileChar = fgetc(file);

	      if(isgraph(fileChar)){
	      	putchar(fileChar);
	      }

	   } while(fileChar != EOF);
	   fclose(file);
         }
         printf("\n");

}

void showNoneComments(char *name){

    FILE *file;
    file= fopen(name, "r");
    char line[MAXLIST];
    char *newString;
    while(fgets(line, sizeof(line) , file)){
    	newString = strtok(line, " ");
    	if(newString[0]=='#'){
    	continue;
    	}
    	else{
    	printf("%s", line);
    	}
    	newString="";

   }
    fclose(file);


}

void showNumberOfLines(){

}

void showTenFirstLine(){

}

//void bashHandler(char *command, char *name){
//    char *newCommand;
//    newCommand = strcat(command, ".sh ");
//    char *fullCommand;
//    fullCommand = strcat("bash ",newCommand);
//    char *newString;
//    newString = malloc(strlen(fullCommand) + strlen(name) + 1);
//    strcpy(newString, fullCommand);
//    strcat(newString, name);
//    system(newString);

//     }
// Function to execute builtin commands
int commandHandler(char** parsed)
{
    int NoOfOwnCmds = 8, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];

    ListOfOwnCmds[0] = "help";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "SFW";
    ListOfOwnCmds[3] = "MRW";
    ListOfOwnCmds[4] = "DAS";
    ListOfOwnCmds[5] = "SNC";
    ListOfOwnCmds[6] = "SNL";
    ListOfOwnCmds[7] = "STFL";

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }

    pid_t pid = fork();
    if(pid==0) {//child
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
    }
    else if(pid<0){
    	printf("error");
    }
    else{ //parent should wait for children
        wait(NULL);
    }
    return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else {

        parseSpace(str, parsed);
    }

    if (commandHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // process
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        // execute
        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}

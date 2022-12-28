Linux Shell
In addition to Linux commands we have some special Commands. Here we explain about their syntax and their job:
1- SFW : This command split the first word of every line of a file and prints it. The syntax of entering this command is: SFW nameOfFile
2- MRW : This command prints the most repeated word in a file. The syntax of entering this command is: MRW nameOfFile
3- DAS : This command delete all spaces and tabs and enters(" ","\t","\n")in a file and prints the new string. The syntax of entering this command is: DAS nameOfFile
4- SNC : This command prints all lines that are not comment(comments are specified by #)in a file. The syntax of entering this command is: SNC nameOfFile
5- SNL : This command count the number of lines in a file and prints it. The syntax of entering this command is: SNL nameOfFile
6- STFL : This command prints the ten first line of a file. The syntax of entering this command is: STFL nameOfFile
and we have also a "help" command that shows all special commands and their job.

We also have a history file. When you run the shell, a file that named history.txt will be created.Every command you enter, goes to this file and after runnig the shell again,
the content of this file will be deleted. In the shell, if you enter up or down button, you can move between prevoius commands that you entered.

In a normal situation, if you press ctrl+c, the command line will close, but here if you enter that, it wont't close and you can keep enter input.
If you want to exit the shell you should press ctrl+c.

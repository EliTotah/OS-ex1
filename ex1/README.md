# OS-ex1

## bash script

Write a script called sh.gccfind that accepts the following arguments:

• Path to the folder (or the name of a folder in the current location, it can be assumed that it exists and does not contain spaces
in her name 

• some word (presumably not empty)

When the file is run, it goes through the resulting folder and first deletes all the files from it
The compiled ones (it can be assumed that they all end in out.). Then, reading the c files in the folder,
and compiles only the c files in the same folder (the name of the output files is the same as the name of the c files from which they were compiled)
and instead of c. The suffix out. See Doge (which contain the given word (regardless of whether it is
Smooth in upper or lower case letters.

A possible flag (it can be assumed that if an instance will always appear after the first 2 arguments):

• The r- flag can be added/decreased, if it appears, execute the same command described only in
Recursive on the given folder (goes over the received folder, and then all subfolders)

## Tennis game

you can read the rules here https://en.wikipedia.org/wiki/Tennis_(paper_game) 


## shell implemention

The program will be called c.myshell. The program will show on
The screen will mark (prompt) and allow the user to type commands in Linux (for example sleep, cat, ls). after the
Pressing ENTER, the typed command will be executed, in a separate process.
Also, the shell will be able to accept any number of arguments where each argument (if any) will contain a full path to the folder,
Now when the program is running, any command that enters the shell will be able to run if it is a command that is already known as (ls (or
If it is a command that is in one of the folders entered in Dodge
her (up to the last level in the tree).

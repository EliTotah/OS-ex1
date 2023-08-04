## Guidelines for 21ex:

● Name of file to send: c21.ex

Write a program that accepts paths to two files as arguments to main, and checks whether the two files
Identical/similar/different.

For the avoidance of doubt, the paths include the filenames.


If the files are identical (contain exactly the same content), the program will return 1, if the files are similar (explanation
Later, the program will return 3, otherwise, if the files are different, it will return 2 (note, this is not the case here
in a program that prints to the screen).


Identical files are files in which all characters are equal (for example: World Hello and World Hello).
Similar files are files that are not identical, but contain the same text, and there is a difference in usage
In lower or uppercase letters, space and/or line breaks.


Different files are files that are neither identical nor similar.

## Instructions for 22ex


● Name of file to send: c22.ex


Write a program that accepts a path to a configuration file as an argument to main. It can be assumed that the file exists
(and not a folder).


The configuration file contains 3 lines:


Line 1: path to the folder containing subfolders. On the first level (one inside), each folder
represents a user, and should contain a c file.


Line 2: Path to the file containing input.


Line 3: Path to the file containing the correct output for the input file from line 2
The configuration file will end with a newline character.


Your program must enter all subfolders (and ignore other files that are not folders,
If they exist (in the folder from row 1), search in each of its subfolders (and not in levels
deeper (c file, and compile it. The created runtime file must be run with the input that appears
In the file at the location in line 2) the program you want to run receives from stdin and prints to stdout, so you must
i/o redirection-b to use).


Assume that there will be at most one c-file in the folder (there may not be a c-file at all, and you can).
be files and folders of other types, except for files with the extension out).


It can be assumed that the c file will necessarily have a suitable extension, for example c.file. The name of has no meaning
The c file.


The output of the program must be compared with the desired output file, whose location comes from line 3, using
The program out.comp that you implemented in part A of the exercise) run the program out.comp and let
as arguments to main the location of the correct output, found in the configuration file in line 3
and the output of the user's program that you ran).


Your program should create a file (in the folder from which your program was run) named csv.results
which contains for each username (subfolder name), its score (between 0 and 100) depending on the answer
that out.comp returned, and why. The character "," must be written between the username and the grade, and
His reason (without profits).


possible reasons:

1. FILE_C_NO - there is no file in the user's folder with the extension .c the grade to be given, 0

2. ERROR_COMPILATION – compilation error (file does not compile). The grade to be given, 10.
3. TIMEOUT - The compiled c file runs for more than 5 seconds. The grade to be given, .20
4. WRONG – the output is different from the desired output. The grade to be given, .50
5. SIMILAR – the output is different from the desired output but similar. The score to be given, .75
6. EXCELLENT – the output is the same as the desired output. The grade to be given, 100.

// Eli Totah 209186014

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/file.h>
#include <dirent.h>
#include <sys/time.h>


// read line from buffer text
int readline(int fd, char *buffer) {
    int i = 0;
    char ch;
    int rd = read(fd, &ch, 1);
    while (rd > 0) {
        if (ch == '\n')
            break;
        buffer[i] = ch;
        i++;
        rd = read(fd, &ch, 1);
    }
    if(rd<0) {
        perror("Error in: read\n");
        return -1;
    }
    buffer[i] = '\0';
}

// check if the path is dir
int isDir(const char *path) {
    struct stat status;

    if (stat(path, &status) == -1) {
        return 0;
    }

    return S_ISDIR(status.st_mode);
}

// check if the path is file
int isFile(const char *path) {
    struct stat status;

    if (stat(path, &status) == -1)
        return 0;

    return S_ISREG(status.st_mode);
}

//check the validation of the config file
int checkValidity (char mainDir[351], char input[351], char output[351]) {
    // Verify the main Dir exists and is a directory
    if (isDir(mainDir) == 0) {
        const char *err_msg = "Not a valid directory\n";
        if(write(1, err_msg, strlen(err_msg)) == -1)
            return -1;
        return -1;    
    }

    // Verify input_file exists and is a file
    if (isFile(input) == 0) {
        const char *err_msg = "Input file not exist\n";
        if (write(1, err_msg, strlen(err_msg)) == -1)
            return -1;
        return -1;
    }

    // Verify correct_output_file exists and is a file
    if (isFile(output) == 0) {
        const char *err_msg = "Output file not exist\n";
        if(write(1, err_msg, strlen(err_msg))== -1) 
            return -1;
        return -1;
    }
    return 1;
}


int main(int argc, char *argv[]) {

    // Read the config file
    int fd = open(argv[1], O_RDONLY);

    char mainDir[351] = "";
    char inputFile[351] = "";
    char outputFile[351] = "";

    //read the lines from the config file
    if (readline(fd, mainDir) == -1) 
        return -1;
    if (readline(fd, inputFile) == -1)
        return -1;
    if (readline(fd, outputFile) == -1)
        return -1;

    // close the config file
    close(fd);


    if (checkValidity(mainDir, inputFile, outputFile) == -1)
        return -1;
        
    // Create errors.txt file
    int errorsFile = open("errors.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (errorsFile == -1) {
        perror("Error in: open\n");
        return -1;
    }

    // Create results.csv file
    int resultsFile = open("results.csv", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (resultsFile == -1) {
        perror("Error in: open\n");
        return -1;
    }

    // open the input file
    int inputFD = open(inputFile, O_RDWR);
    if (inputFD == -1) {
        perror("Error in: open\n");
        return -1;
    }

    // Open the students_folder
    DIR *user_dir = opendir(mainDir);
    if (user_dir == NULL)
    {
        perror("Error in: opendir\n");
        return -1;
    }

    // Iterate over the mainDir
    struct dirent *firstLevel;
    while ((firstLevel = readdir(user_dir)) != NULL) {

        // Skip the . and .. folders
        if (strcmp(firstLevel->d_name, ".") == 0 || strcmp(firstLevel->d_name, "..") == 0)
            continue;

        // the path for the sub dir
        char pathStFolder[151] = "";
        strcat(pathStFolder, mainDir);
        strcat(pathStFolder, "/");
        strcat(pathStFolder, firstLevel->d_name);

        // if it's not dir, continue to the next one
        if (!isDir(pathStFolder)) {
            continue;
        }

        else {

            // Open the student_dir
            DIR *studentDir = opendir(pathStFolder);
            if (studentDir == NULL)
            {
                perror("Error in: opendir\n");
                return -1;
            }

            // Iterate over the student_folder
            int flagC = 0;

            // open output txt file
            int outputFD = open("output.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            if (outputFD == -1) {
                perror("Error in: open");
                continue; // Move to the next student
            }

            // iterate for every file in the subdir
            struct dirent *secondLevel;
            while ((secondLevel = readdir(studentDir)) != NULL) {

                // Skip the . and .. folders
                if (strcmp(secondLevel->d_name, ".") == 0 || strcmp(secondLevel->d_name, "..") == 0) 
                    continue;

                // the path for the file
                char student_C_File[151] = "";
                strcat(student_C_File, pathStFolder);
                strcat(student_C_File, "/");
                strcat(student_C_File, secondLevel->d_name);

                // if it's not file, continue to next one
                if (!isFile(student_C_File)) {
                    continue;
                }

                // Check if student_file is a c file
                char *fileName = secondLevel->d_name;
                int len = strlen(fileName);
                if ((fileName[len - 1] != 'c') || (fileName[len - 2] != '.')) {
                    continue;
                } 

                // we found c file
                flagC = 1;

                // Compile the student_file on a child process
                int pid = fork();
                if (pid < 0) {
                    perror("Error in: fork");
                    continue; // Move to the next one
                }

                // child process
                if (pid == 0) {
                    //complie to c file
                    if (dup2(errorsFile, 2)== -1) {
                        perror("Error in: dup2");
                        break;
                    }
                    char *args[] = {"gcc", "-o","a.out",student_C_File, NULL};
                    int result = execvp("gcc", args);
                    if (result == -1) {
                        perror("Error in: execvp");
                        break;
                    }
                    //father process
                } else if (pid > 0) {
                    int status;
                    wait(&status);
                    // Check if the compilation was successful
                    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                        int wr = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                        if(wr == -1) {
                            break;
                        }
                        int wr2 = write(resultsFile, ",10,COMPILATION_ERROR\n", strlen(",10,COMPILATION_ERROR\n"));
                        if(wr2 == -1) {
                            break;
                        }
                        break;
                    }
                }

                // check the time we start
                    struct timeval tStart, tEnd;
                    if(gettimeofday(&tStart,NULL)) {
                        perror("Error in: gettimeofday");
                        break;
                    }
                    pid = fork();
                    if (pid < 0) {
                        perror("Error in: fork");
                        continue; // Move to the next one
                    }
                    //child process
                    if (pid == 0) {
                        // run the program
                        if((lseek(inputFD, 0, SEEK_SET)) == -1){
                            perror("Error in: lseek");
                            break;
                        }
                        if ((dup2(inputFD, 0) == -1) || (dup2(outputFD, 1) == -1) || (dup2(errorsFile, 2) == -1)) {
                            perror("Error in: dup2");
                            break;
                        }
                        char *args[] = {"./a.out", NULL};
                        int result = execvp(args[0], args);
                        if (result == -1) {
                            perror("Error in: execvp");
                            break;
                        }
                        // father process
                    } else {
                        int status;
                        wait(&status);
                        // get the end time
                        if(gettimeofday(&tEnd,NULL)) {
                            perror("Error in: gettimeofday");
                            break;
                        }
                        int wr4,wr3;
                        int elap = (tEnd.tv_sec - tStart.tv_sec) * 1000000 + (tEnd.tv_usec - tStart.tv_usec);
                        // check if the run time took more than 5 seconds
                        if(elap > 5000000) {
                            wr3 = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                            if(wr3 == -1) {
                                break;
                            }
                            wr4 = write(resultsFile, ",20,TIMEOUT\n", strlen(",20,TIMEOUT\n"));
                            if(wr4 == -1) {
                                break;
                            }
                            break;
                        }
                    }

                    
                    // Compare the output with the correct output
                    pid = fork();
                    if (pid < 0) {
                        perror("Error in: fork");
                        continue; // Move to the next one
                    }
                    // child process
                    if (pid == 0) {
                        // compare the results
                        char *args[] = {"./comp.out", "./output.txt", outputFile, NULL};
                        int result = execvp(args[0], args);
                        if (result == -1) {
                            perror("Error in: execvp");
                            break;
                        }
                    // father process  
                    } else if (pid>0){
                        // check the results of the compare
                        int wr4,wr3;
                        int status;
                        wait(&status);
                        int exit_status = WEXITSTATUS(status);
                        switch (exit_status) {
                            case 1:
                                wr3 = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                                if(wr3 == -1) {
                                    break;
                                }
                                wr4 = write(resultsFile, ",100,EXCELLENT\n", strlen(",100,EXCELLENT\n"));
                                if(wr4 == -1) {
                                    break;
                                }
                                break;
                            case 2:
                                wr3 = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                                if(wr3 == -1) {
                                    break;
                                }
                                wr4 = write(resultsFile, ",50,WRONG\n", strlen(",50,WRONG\n"));
                                if(wr4 == -1) {
                                    break;
                                }
                                break;
                            case 3:
                                wr3 = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                                if(wr3 == -1) {
                                    break;
                                }
                                wr4 =  write(resultsFile, ",75,SIMILAR\n", strlen(",75,SIMILAR\n"));
                                if(wr4 == -1) {
                                    break;
                                }
                                break;
                        }
                    }
                    break; // We only need to check the first c file
                }

                if (closedir(studentDir)==-1) {
                    perror("Error in: close");
                    break;
                }
                int c = close(outputFD);
                if (c == -1) {
                    perror("Error in: close");
                    break;
                }

                if (remove("output.txt")) {
                    perror("Error in: remove");
                    return -1; // This will be a problem
                }
                remove("a.out"); // Error is OK

                // if we didn't found c file 
                if (!flagC) {
                    int w3 = write(resultsFile, firstLevel->d_name, strlen(firstLevel->d_name));
                    if(w3==-1) {
                        continue;;
                    }
                    int w4 = write(resultsFile, ",0,NO_C_FILE\n", strlen(",0,NO_C_FILE\n"));
                    if(w4 == -1) {
                        continue;;
                    }
                }
            }
        }    
    
        if(closedir(user_dir)) {
            perror("Error in: close");
            return -1;
        }
            
        if (close(inputFD)) {
            perror("Error in: close");
            return -1;
        }
        if (close(errorsFile)) {
            perror("Error in: close");
            return -1;
        }
    }

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

int main(int argc, char *argv[]) {
    char *file1 = argv[1];
    char *file2 = argv[2];
    int similarFlag;
    int noIden;
    char ch1;
    char ch2;
    char cb1;
    char cb2;
    int c,c2;

    //check if we got enough args
    if(argc != 3 ) {
        perror("not enough args");
        return -1;
    }

    // open the first file
    int fd1 = open(file1 ,O_RDONLY);
    if (fd1 < 0) {
        perror("Error in: open");
        return -1;
    }

    //open the second file
    int fd2 = open(file2 ,O_RDONLY);
    if (fd2 < 0) {
        c = close(fd1);
        if(c == -1) {
            perror("Error in: close");
        }
        perror("Error in: open");
        return -1;
    }

    // read 1byte from the first file
    int rd1 = read(fd1, &ch1, 1);
    // read 1 byte from the second file
    int rd2 = read(fd2, &ch2, 1);

    while (1)
    {   
        //check if we got error in read
        if(rd1 < 0) {
            perror("Error in: read");
            c = close(fd1);
            if(c == -1) {
                perror("Error in: close");
            }
            c2 = close(fd2);
            if(c2 == -1) {
                perror("Error in: close");
            }
            return -1;
        }

        //check if we got error in read
        if(rd2 < 0) {
            perror("Error in: read");
            c = close(fd1);
            if(c == -1) {
                perror("Error in: close");
            }
            c2 = close(fd2);
            if(c2 == -1) {
                perror("Error in: close");
            }
            return -1;
        }

        // we successed in our both read
        if (rd1 && rd2) {

            //check if there is spaces or down line
            if(ch1 == ' ' || ch1 == '\n') {
                if(ch2 == ' ' || ch2 == '\n') {
                    rd1 = read(fd1, &ch1, 1);
                    rd2 = read(fd2, &ch2, 1);
                    continue;
                }
                else {
                    // if there is space in the first and there isn't in the second so the files are not identical
                    noIden = 1;
                    rd1 = read(fd1, &ch1, 1);
                    continue;
                }
            }

            if(ch2 == ' ' || ch2 == '\n') {
                if(ch1 == ' ' || ch1 == '\n') {
                    rd1 = read(fd1, &ch1, 1);
                    rd2 = read(fd2, &ch2, 1);
                    continue;
                }
                else {
                    // if there is space in the second and there isn't in the first so the files are not identical
                    noIden = 1;
                    rd2 = read(fd2, &ch2, 1);
                    continue;
                }
            }

            // move the letter we readed to lower case
            cb1 = tolower(ch1);
            cb2 = tolower(ch2);
            
            // if the lower case is differnet so the files are differnts
            if(cb1 != cb2) {
                c = close(fd1);
                if(c == -1) {
                    perror("Error in: close");
                    return -1;
                }
                c2 = close(fd2);
                if(c2 == -1) {
                    perror("Error in: close");
                    return -1;
                }
                return 2;
            }

            else {
                // if the lower is equal but the actual letter not the files are similar
                if (ch1 != ch2) {
                    similarFlag = 1;
                    rd1 = read(fd1, &ch1, 1);
                    rd2 = read(fd2, &ch2, 1);
                    continue;
                }
            
                // the letter is equals in both files
                else {
                    rd1 = read(fd1, &ch1, 1);
                    rd2 = read(fd2, &ch2, 1);
                    continue;
                }
                
            }
        
        }

        // one of the files is over and the second isn't
        if (rd1 || rd2) {

            // check if there is more letters and if yes so the files are differtnes
            while (rd1)
            {   
                //failure in the read
                rd1 = read(fd1, &ch1, 1);
                if (rd1 < 0) {
                    perror("Error in: read");
                    c = close(fd1);
                    if(c == -1) {
                        perror("Error in: close");
                    }
                    c2 = close(fd2);
                    if(c2 == -1) {
                        perror("Error in: close");
                    }
                    return -1;
                }
                // we have some txt in the first and zero in he second, if this not space so the files different
                if (ch1 != ' ' && ch1 != '\n') {
                    c = close(fd1);
                    if(c == -1) {
                        perror("Error in: close");
                        return -1;
                    }
                    c2 = close(fd2);
                    if(c2 == -1) {
                        perror("Error in: close");
                        return -1;
                    }
                    return 2;
                }
            }

            // check if there is more letters and if yes so the files are differtnes
            while (rd2)
            {
                rd2 = read(fd2, &ch2, 1);
                if(rd2 < 0) {
                    perror("Error in: read");
                    c = close(fd1);
                    if(c == -1) {
                        perror("Error in: close");
                    }
                    c2 = close(fd2);
                    if(c2 == -1) {
                        perror("Error in: close");
                    }
                    return -1;
                }
                if (ch2 != ' ' && ch2 != '\n') {
                    c = close(fd1);
                    if(c == -1) {
                        perror("Error in: close");
                        return -1;
                    }
                    c2 = close(fd2);
                    if(c2 == -1) {
                        perror("Error in: close");
                        return -1;
                    }
                    return 2;
                }
            }
            similarFlag = 1;
        }
        break;
    }

    // if the similar flag is turn on so the files are similar
    if (similarFlag == 1 || noIden == 1) {
        c = close(fd1);
        if(c == -1) {
            perror("Error in: close");
            return -1;
        }
        c2 = close(fd2);
        if(c2 == -1) {
            perror("Error in: close");
            return -1;
        }
        return 3;
    }

    // else, we the files not similar, and we didn't retunred already so the files are equal
    c = close(fd1);
    if(c == -1) {
        perror("Error in: close");
        return -1;
    }
    c2 = close(fd2);
    if(c2 == -1) {
        perror("Error in: close");
        return -1;
    }
    return 1;
}


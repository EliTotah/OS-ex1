#!/bin/bash
#Eli Totah 209186014

# A function to compile a C file if it contains a specific word
compile_file() {
    local file=$1
    local filename=${file%.*}
    if grep -q -i -w $word $file
    then
        gcc -w $file -o ${filename}.out
    fi
}

# A function to process a directory
proceess_directory() {
    # Get the script directory
    script_dir=$(dirname "$(readlink -f "$0")")
    local dir=$1
    cd $dir

    # Remove any previous output files
    rm -f *.out

    # Loop through all the C files in the directory and compile them if they contain the specified word
    for file in *.c
    do
        compile_file $file
    done
    
    # Recursively process subdirectories if the recursive flag is true
    if $recursive = true
    then 
        for subdir in */
        do
        # Check if the current item is a directory
            if [ -d "$subdir" ] 
            then 
        # Call the current script with the current subdirectory as the path argument, the same word argument and the recursive flag
                ("$script_dir/gccfind.sh" "$subdir" "$word" "-r")
            fi
        done
    fi
    cd ..
} 

# Check if there are enough parameters
if [ "$#" -lt 2 ]
then
    echo Not enough parameters
    exit 1
fi

path=$1
word=$2

# Check if the recursive flag is provided as the third parameter
recursive=false
if [ $# -gt 2 ] && [ $3 == "-r" ]
then 
    recursive=true
fi 

# Call the process_directory function with the provided path argument
proceess_directory $path
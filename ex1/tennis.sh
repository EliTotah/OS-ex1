#!/bin/bash
#Eli Totah 209186014

# Function to display the welcome board
welcom_board() {

    player1Points=$1
    player2Points=$2
    echo " Player 1: ${player1Points}        Player 2: ${player1Points} "
    echo " --------------------------------- "
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    echo " |       |       O       |       | " 
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    echo " --------------------------------- "
}

# Function to display the game board
print_board() {
    player1Points=$1
    player2Points=$2
    ballPosition=$3
    player1LastChoice=$4
    player2LastChoice=$5

    echo " Player 1: ${player1Points}        Player 2: ${player2Points} "
    echo " --------------------------------- "
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    # Display the ball in the correct position based on the ballPosition variable
    case ${ballPosition} in 
    -3) echo " |       |       #       |       |O" ;;
    -2) echo " |       |       #       |   O   | " ;;
    -1) echo " |       |       #   O   |       | " ;;
    0)  echo " |       |       O       |       | " ;;
    1)  echo " |       |   O   #       |       | " ;;
    2) echo  " |   O   |       #       |       | " ;;
    3) echo  "O|       |       #       |       | " ;; 
    esac
    echo " |       |       #       |       | "
    echo " |       |       #       |       | "
    echo " --------------------------------- "
    # Display the last move made by each player
    echo -e "       Player 1 played: ${player1LastChoice}\n       Player 2 played: ${player2LastChoice}\n\n"
}

# Function to run the game loop
while_loop() {
    # Initialize player points and ball position
    player1Points=50
    player2Points=50
    ballPosition=0
    player1LastChoice=
    player2LastChoice=

    welcom_board "$player1Points" "$player2Points"
    while true
    do 
        # Player 1 selects a move
        while true  
        do 
            echo "PLAYER 1 PICK A NUMBER: "
            read -s -e choice1
            regex='^[0-9]+$'
            # Validate the input
            if  ! [[ $choice1 =~ $regex ]] || [[ "$choice1" -gt $player1Points ]]
            then 
                echo "NOT A VALID MOVE !"
                continue
            fi
            break
        done

        # Player 2 selects a move
        while true  
        do
            echo "PLAYER 2 PICK A NUMBER: "
            read -s -e choice2
            # Validate the input
            if  ! [[ $choice2 =~ $regex ]] || [[ "$choice2" -gt $player2Points ]]
            then 
                echo "NOT A VALID MOVE !"
                continue
            fi
            break
        done

        # Update the ball position based on the moves made by each 
        # If player 1's move is greater than player 2's move and the ball is on player 1's side
        if (( $choice1>$choice2 )) && (( ballPosition >= 0 ))
        then 
        # Set the ball position to player 2's side
            ballPosition=-1
        # If player 1's move is greater than player 2's move and the ball is on player 2's side
        elif (( $choice1>$choice2 )) && (( ballPosition < 0 ))   
        then 
        # Move the ball one step towards player 1
            ballPosition=$[ballPosition-1]
        # If player 2's move is greater than player 1's move and the ball is on player 1's side
        elif (( $choice1<$choice2 )) && (( ballPosition >= 0 ))
        then   
        # Move the ball one step towards player 2
            ballPosition=$[ballPosition+1]
        # If player 2's move is greater than player 1's move and the ball is on player 2's side
        elif (( $choice1<$choice2 )) && (( ballPosition < 0 ))
        then    
        # Set the ball position to player 1's side
            ballPosition=1
        fi



        player1LastChoice=$choice1
        player2LastChoice=$choice2
        player1Points=$[player1Points-choice1]
        player2Points=$[player2Points-choice2]
        
        print_board "$player1Points" "$player2Points" "$ballPosition" "$player1LastChoice" "$player2LastChoice"

        # If the ball reaches player 2's end
        if (( ballPosition==-3 ))
        then 
            echo "PLAYER 1 WINS !"
            # end the game
            break

        # If the ball reaches player 1's end    
        elif (( ballPosition==3 ))
        then   
            echo "PLAYER 2 WINS !"
            break
        fi

        # If player 1 has no points left and player 2 has points left
        if (( player1Points<=0 && player2Points>0 ))
        then 
            echo "PLAYER 2 WINS !"
            break
        # If player 2 has no points left and player 1 has points left    
        elif (( player1Points>0 && player2Points<=0 ))
        then   
            echo "PLAYER 1 WINS !"
            break
        # If the ball is on player 1's side and both players have no points left    
        elif (( ballPosition > 0 && player1Points==0 && player2Points==0 ))
        then 
            echo "PLAYER 2 WINS !"
            break
        # If the ball is on player 2's side and both players have no points left    
        elif (( ballPosition < 0 && player1Points==0 && player2Points==0 ))
        then 
            echo "PLAYER 1 WINS !"
            break
        # If the ball is in the middle and both players have no points left    
        elif (( ballPosition == 0 && player1Points==0 && player2Points==0 ))   
        then
            echo "IT'S A DRAW !"
            break
        fi
    done
            
}

while_loop
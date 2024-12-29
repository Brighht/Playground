#include <stdio.h>
#include <stdlib.h>

/* findRow: Function to calculate the row index */ 
int findRow(int index, int cols) {
    return index / cols;
}

/*findCol: Function to calculate the column index*/ 
int findCol(int index, int cols) {
    return index % cols;
}

/*printGameboard: Function to display the current gameboard*/ 
void printGameBoard(int rows, int cols, int gameboard[rows][cols]) {
    printf("Current Board:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c\t", gameboard[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int rows = 3, cols = 3;
    int gameboard[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    int p1Moves = 0, p2Moves = 0;
    char p1Char, p2Char;

    // Player character selection
    printf("Enter 'X' or 'O' to choose for Player 1: ");
    scanf(" %c", &p1Char);
    printf("Enter 'X' or 'O' to choose for Player 2: ");
    scanf(" %c", &p2Char);

    // Validate that Player 1 and Player 2 do not choose the same character
    if (p1Char == p2Char) {
        printf("Both players cannot have the same character! Restart the game.\n");
        return 1;
    }

    /*Player selection 1 for p1 and 2 for p2*/
    int currentPlayer = 1; 

    while (1) {
        // Check if the board is full (game over)
        if (p1Moves + p2Moves == rows * cols) {
            printf("It's a draw!\n");
            break;
        }

        int moveIndex, curRow, curCol;

        // Current player's turn
        if (currentPlayer == 1) {
            printf("Player 1 (%c), enter index to make a move (0-8): ", p1Char);
        } else {
            printf("Player 2 (%c), enter index to make a move (0-8): ", p2Char);
        }

        scanf("%d", &moveIndex);

        // Calculate row and column from 1D index
        curRow = findRow(moveIndex, cols);
        curCol = findCol(moveIndex, cols);

        // Check for valid move
        if (curRow < 0 || curRow >= rows || curCol < 0 || curCol >= cols || 
            gameboard[curRow][curCol] != ' ') {
            printf("Invalid move! Position is either out of range or already occupied. Try again.\n");
            continue; // Repeat the turn
        }

        // Make the move
        if (currentPlayer == 1) {
            gameboard[curRow][curCol] = p1Char;
            p1Moves++;
            currentPlayer = 2; // Switch to Player 2
        } else {
            gameboard[curRow][curCol] = p2Char;
            p2Moves++;
            currentPlayer = 1; // Switch to Player 1
        }

        // Print the updated gameboard
        printGameBoard(rows, cols, gameboard);
    }

    return 0;
}

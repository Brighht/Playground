#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

void printMask(char *mask) {
    for (int i = 0; i < strlen(mask); i++) {
        printf("%c ", mask[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <words_file>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *wordsFile = fopen(filename, "r");
    if (!wordsFile) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return 1;
    }

    
    char words[100][50];
    int wordCount = 0;
    while (fscanf(wordsFile, "%49s", words[wordCount]) == 1) {
        wordCount++;
    }
    fclose(wordsFile);

    
    srand(time(NULL));
    char *targetWord = words[rand() % wordCount];
    int wordLength = strlen(targetWord);

    // Initialize game variables
    char mask[wordLength + 1];
    for (int i = 0; i < wordLength; i++) mask[i] = '_';
    mask[wordLength] = '\0';

    int remainingAttempts = 6;
    char guessedLetters[26] = {0};
    int guesses = 0;

    // Gameplay loop
    while (remainingAttempts > 0) {
        printf("\nWord: ");
        printMask(mask);
        printf("Remaining Attempts: %d\n", remainingAttempts);

        // Display guessed letters
        printf("Guessed Letters: ");
        for (int i = 0; i < guesses; i++) {
            printf("%c ", guessedLetters[i]);
        }
        printf("\n");

        // Take user input
        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);

        // Check if letter was already guessed
        int alreadyGuessed = 0;
        for (int i = 0; i < guesses; i++) {
            if (guessedLetters[i] == guess) {
                alreadyGuessed = 1;
                break;
            }
        }
        if (alreadyGuessed) {
            printf("You already guessed '%c'. Try again.\n", guess);
            continue;
        }

        // Add guess to guessed letters
        guessedLetters[guesses++] = guess;

        // Check if guess is in the word
        int correct = 0;
        for (int i = 0; i < wordLength; i++) {
            if (targetWord[i] == guess) {
                mask[i] = guess;
                correct = 1;
            }
        }

        if (!correct) {
            remainingAttempts--;
            printf("Wrong guess!\n");
        }

        // Check for win
        if (strcmp(mask, targetWord) == 0) {
            printf("\nCongratulations! You guessed the word: %s\n", targetWord);
            return 0;
        }
    }

    // Game over
    printf("\nGame Over! The word was: %s\n", targetWord);
    return 0;
}

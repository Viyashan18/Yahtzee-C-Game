#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROUNDS 13
#define DICE 5
#define CATEGORIES 13


// Function prototypes
void rollDice(int dice[]);
void displayDice(int dice[]);
void rerollDice(int dice[], int *numRolls);
int scoreCategory(int dice[], int category);
int computerOptimalChoice(int dice[], int usedCategories[]);
void playRound(int round, int *humanScore, int *computerScore, int usedCategories[], int computerUsedCategories[]);
int countOccurrences(int dice[], int value);
int isFullHouse(int dice[]);
int isSmallStraight(int dice[]);
int isLargeStraight(int dice[]);

int main() {
    srand(time(0));
    int humanScore = 0, computerScore = 0;
    int usedCategories[CATEGORIES] = {0};           // Human-used categories
    int computerUsedCategories[CATEGORIES] = {0};   // computer-used categories

    printf("Welcome to Yahtzee!\n");

    // Game Loop - 13 Rounds
    for (int round = 1; round <= ROUNDS; round++) {
        playRound(round, &humanScore, &computerScore, usedCategories, computerUsedCategories);
    }

    // Displaying Final Scores
    printf("\nGame Over! Final Scores:\n");
    printf("Human: %d, Computer: %d\n", humanScore, computerScore);
    if (humanScore > computerScore) printf("Human wins!\n");
    else if (computerScore > humanScore) printf("Computer wins!\n");
    else printf("It's a tie!\n");

    return 0;
}

// Function to roll the dice
void rollDice(int dice[]) {
    for (int i = 0; i < DICE; i++) {
        dice[i] = (rand() % 6) + 1;
    }
}

//  displaying dice values
void displayDice(int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("\n");
}

//  reroll dice
void rerollDice(int dice[], int *numRolls) {
    if (*numRolls < 3) {
        char choice;
        printf("Do you want to reroll dice? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            for (int i = 0; i < DICE; i++) {
                printf("Reroll dice %d? (y/n): ", i + 1);
                scanf(" %c", &choice);
                if (choice == 'y' || choice == 'Y') {
                    dice[i] = (rand() % 6) + 1;
                }
            }
            (*numRolls)++;
            displayDice(dice);
            rerollDice(dice, numRolls); // check if player wants more rerolls
        }
    }
}

// times of dice number
int countOccurrences(int dice[], int value) {
    int count = 0;
    for (int i = 0; i < DICE; i++) {
        if (dice[i] == value) count++;
    }
    return count;
}

// checking full house
int isFullHouse(int dice[]) {
    int counts[7] = {0}; // storeing  dice values 1-6
    for (int i = 0; i < DICE; i++) {
        counts[dice[i]]++;
    }

    int hasThree = 0, hasTwo = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) hasThree = 1;
        if (counts[i] == 2) hasTwo = 1;
    }
    return hasThree && hasTwo;
}

// checking small straight
int isSmallStraight(int dice[]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE; i++) {
        counts[dice[i]]++;
    }

    // Checking  sequences like 1-2-3-4, 2-3-4-5, or 3-4-5-6
    return (counts[1] && counts[2] && counts[3] && counts[4]) ||
           (counts[2] && counts[3] && counts[4] && counts[5]) ||
           (counts[3] && counts[4] && counts[5] && counts[6]);
}

// checking large straight
int isLargeStraight(int dice[]) {
    int counts[7] = {0};
    for (int i = 0; i < DICE; i++) {
        counts[dice[i]]++;
    }

    // Checking  sequences like 1-2-3-4-5 or 2-3-4-5-6
    return (counts[1] && counts[2] && counts[3] && counts[4] && counts[5]) ||
           (counts[2] && counts[3] && counts[4] && counts[5] && counts[6]);
}

// scoreing perticular category
int scoreCategory(int dice[], int category) {
    int score = 0;
    switch (category) {
        case 1: case 2: case 3: case 4: case 5: case 6: // Ones to Sixes
            for (int i = 0; i < DICE; i++) {
                if (dice[i] == category) score += category;
            }
            break;
        case 7: // Three of a Kind
            for (int i = 1; i <= 6; i++) {
                if (countOccurrences(dice, i) >= 3) {
                    for (int j = 0; j < DICE; j++)
                     score += dice[j];
                    break;
                }
            }
            break;
        case 8: // Four of a Kind
            for (int i = 1; i <= 6; i++) {
                if (countOccurrences(dice, i) >= 4) {
                    for (int j = 0; j < DICE; j++) 
                    score += dice[j];
                    break;
                }
            }
            break;
        case 9: // Full House
            score = isFullHouse(dice) ? 25 : 0;
            break;
        case 10: // Small Straight
            score = isSmallStraight(dice) ? 30 : 0;
            break;
        case 11: // Large Straight
            score = isLargeStraight(dice) ? 40 : 0;
            break;
        case 12: // Yahtzee
            for (int i = 1; i <= 6; i++) {
                if (countOccurrences(dice, i) == 5) {
                    score = 50;
                    break;
                }
            }
            break;
        case 13: // Chance
            for (int i = 0; i < DICE; i++) score += dice[i];
            break;
        default:
            printf("Invalid category!\n");
    }
    return score;
}

//  computer's choice
int computerOptimalChoice(int dice[], int usedCategories[]) {
    for (int i = 13; i >= 1; i--) {
        if (!usedCategories[i - 1]) { // Choose first available category
            usedCategories[i - 1] = 1;
            return i;
        }
    }
    return 13;
}

// play a round
void playRound(int round, int *humanScore, int *computerScore, int usedCategories[], int computerUsedCategories[]) {
    int dice[DICE];
    int numRolls = 0;

    printf("\n--- Round %d ---\n", round);
    
    // Human's turn
    printf("Human's turn:\n");
    rollDice(dice);
    displayDice(dice);
    rerollDice(dice, &numRolls);

    int category;
    do {
        printf("Choose a scoring category (1-13): ");
        scanf("%d", &category);
        if (usedCategories[category - 1]) {
            printf("Category already used! Choose another category.\n");
        }
    } while (usedCategories[category - 1]);

    usedCategories[category - 1] = 1;
    *humanScore += scoreCategory(dice, category);

    // Computer's turn
    printf("\nComputer's turn:\n");
    rollDice(dice);
    displayDice(dice);
    int computerCategory = computerOptimalChoice(dice, computerUsedCategories);
    *computerScore += scoreCategory(dice, computerCategory);

    printf("Scores after round %d - Human: %d, Computer: %d\n", round, *humanScore, *computerScore);
}


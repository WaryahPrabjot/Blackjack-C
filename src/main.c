#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52
#define MAX_HAND 12   // Max possible cards without bust

// ---------- Utility Functions ----------

// Initialize deck with values
void initDeck(int deck[]) {
    int values[] = {2,3,4,5,6,7,8,9,10,10,10,10,11}; // J,Q,K=10, Ace=11
    int index = 0;

    for (int suit = 0; suit < 4; suit++) {
        for (int i = 0; i < 13; i++) {
            deck[index++] = values[i];
        }
    }
}

// Fisher-Yates shuffle
void shuffleDeck(int deck[]) {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Calculate score with Ace logic
int calculateScore(int hand[], int count) {
    int sum = 0, aces = 0;

    for (int i = 0; i < count; i++) {
        sum += hand[i];
        if (hand[i] == 11)
            aces++;
    }

    while (sum > 21 && aces > 0) {
        sum -= 10;  // Convert Ace from 11 to 1
        aces--;
    }

    return sum;
}

void printHand(int hand[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%d", hand[i]);
        if (i != count - 1) {
            printf("|");
        }
    }
    printf("\n");
}


// ---------- Main Game ----------

int main() {
    srand(time(0));

    int money = 1000;

    while (money > 0) {

        int bet;
        printf("\nMoney: %d\nEnter bet: ", money);
        scanf("%d", &bet);

        if (bet > money || bet <= 0) {
            printf("Invalid bet.\n");
            continue;
        }

        int deck[DECK_SIZE];
        initDeck(deck);
        shuffleDeck(deck);

        int deckIndex = 0;

        int player[MAX_HAND], dealer[MAX_HAND];
        int pCount = 0, dCount = 0;

        // Initial Deal
        player[pCount++] = deck[deckIndex++];
        player[pCount++] = deck[deckIndex++];

        dealer[dCount++] = deck[deckIndex++];
        dealer[dCount++] = deck[deckIndex++];

        printf("\nYour hand: ");
        printHand(player, pCount);
        printf("Dealer shows: %d\n", dealer[0]);

        // Player Turn
        int choice;
        while (1) {
            int score = calculateScore(player, pCount);
            printf("Your score: %d\n", score);

            if (score > 21) {
                printf("Bust! You lose.\n");
                money -= bet;
                break;
            }

            printf("1. Hit  2. Stand: ");
            scanf("%d", &choice);

            if (choice == 1)
                player[pCount++] = deck[deckIndex++];
            else
                break;
        }

        // Dealer Turn
        if (calculateScore(player, pCount) <= 21) {
            while (calculateScore(dealer, dCount) < 17)
                dealer[dCount++] = deck[deckIndex++];

            int playerScore = calculateScore(player, pCount);
            int dealerScore = calculateScore(dealer, dCount);

            printf("\nDealer hand: ");
            printHand(dealer, dCount);
            printf("Dealer score: %d\n", dealerScore);

            if (dealerScore > 21 || playerScore > dealerScore) {
                printf("You win!\n");
                money += bet;
            } else if (playerScore == dealerScore) {
                printf("Push (Draw)\n");
            } else {
                printf("Dealer wins!\n");
                money -= bet;
            }
        }

        if (money <= 0) {
            printf("Game Over! You're out of money.\n");
            break;
        }

        printf("Play again? (1=Yes, 0=No): ");
        int again;
        scanf("%d", &again);
        if (!again)
            break;
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEAR_SIZE 10
#define CIRCULAR_SIZE 5

/* Structure to represent a Song */
typedef struct {
    int id;
    char title[50];
    char artist[50];
} Song;

/* ==========================================================================
   1. LINEAR QUEUE (Up Next Song Queue - Size 10)
   ========================================================================== */
Song linearQueue[LINEAR_SIZE];
int l_front = -1;
int l_rear = -1;

/* Enqueue: Add a song to the Linear Queue */
void enqueueLinear(Song s) {
    if (l_rear == LINEAR_SIZE - 1) {
        printf("\n[Linear Queue OVERFLOW] Queue is full! (Max %d songs)\n", LINEAR_SIZE);
        return;
    }
    if (l_front == -1) {
        l_front = 0;
    }
    l_rear++;
    linearQueue[l_rear] = s;
    printf("\n[Success] Added to Queue: ID %d | \"%s\" by %s\n", s.id, s.title, s.artist);
}

/* Dequeue: Remove a song from the front of the Linear Queue */
int dequeueLinear(Song *s) {
    if (l_front == -1 || l_front > l_rear) {
        printf("\n[Linear Queue UNDERFLOW] Queue is empty! No songs to remove.\n");
        return 0; // Failure
    }
    *s = linearQueue[l_front];
    l_front++;
    // Reset indices when queue becomes empty
    if (l_front > l_rear) {
        l_front = -1;
        l_rear = -1;
    }
    return 1; // Success
}

/* Display: View all upcoming songs in the Linear Queue */
void viewUpNext() {
    if (l_front == -1 || l_front > l_rear) {
        printf("\n[Linear Queue] Up Next queue is currently empty.\n");
        return;
    }
    printf("\n================ UP NEXT (LINEAR QUEUE) ================\n");
    for (int i = l_front; i <= l_rear; i++) {
        printf("  %2d. [ID: %d] %-25s | Artist: %s\n", 
               (i - l_front + 1), linearQueue[i].id, linearQueue[i].title, linearQueue[i].artist);
    }
    printf("========================================================\n");
}

/* ==========================================================================
   2. CIRCULAR QUEUE (Recently Played History - Size 5)
   ========================================================================== */
Song circularQueue[CIRCULAR_SIZE];
int c_front = -1;
int c_rear = -1;
int c_count = 0;

/* Insert into Circular Queue: Adds dequeued song to history */
void enqueueCircular(Song s) {
    if (c_count == 0) {
        c_front = 0;
        c_rear = 0;
        circularQueue[c_rear] = s;
        c_count = 1;
    } else if (c_count == CIRCULAR_SIZE) {
        // Queue is full: Circular overwrite replaces the oldest song
        c_front = (c_front + 1) % CIRCULAR_SIZE;
        c_rear = (c_rear + 1) % CIRCULAR_SIZE;
        circularQueue[c_rear] = s;
        printf("[Circular Queue] History buffer full (5 songs). Oldest track replaced.\n");
    } else {
        c_rear = (c_rear + 1) % CIRCULAR_SIZE;
        circularQueue[c_rear] = s;
        c_count++;
    }
    printf("[Circular Queue] Added to Recently Played: \"%s\"\n", s.title);
}

/* Display: View recently played songs in Circular Queue */
void viewRecentlyPlayed() {
    if (c_count == 0) {
        printf("\n[Circular Queue] Recently Played history is empty.\n");
        return;
    }
    printf("\n============= RECENTLY PLAYED (CIRCULAR QUEUE) =============\n");
    int curr = c_rear;
    for (int i = 0; i < c_count; i++) {
        printf("  %d. [ID: %d] %-25s | Artist: %s\n", 
               (i + 1), circularQueue[curr].id, circularQueue[curr].title, circularQueue[curr].artist);
        curr = (curr - 1 + CIRCULAR_SIZE) % CIRCULAR_SIZE; // Most recent first
    }
    printf("============================================================\n");
}

/* Helper to read safe strings with spaces */
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    }
}

/* ==========================================================================
   MAIN MENU
   ========================================================================== */
int main() {
    int choice;
    Song s;

    printf("\n=================================================================\n");
    printf("     DATA STRUCTURES MICRO PROJECT (SLA) - GROUP 2 (SYCM1)       \n");
    printf("     Linear Queue (Size: 10) & Circular Queue (Size: 5)          \n");
    printf("=================================================================\n");

    // Preload 3 initial songs into Linear Queue for immediate testing
    Song demo1 = {101, "Blinding Lights", "The Weeknd"};
    Song demo2 = {102, "Shape of You", "Ed Sheeran"};
    Song demo3 = {103, "Starboy", "The Weeknd"};
    enqueueLinear(demo1);
    enqueueLinear(demo2);
    enqueueLinear(demo3);

    while (1) {
        printf("\n======================= MENU =======================\n");
        printf(" 1. Add Song (Linear Queue)\n");
        printf(" 2. Remove Song (Linear Queue -> Circular Queue)\n");
        printf(" 3. View Up Next (Linear Queue)\n");
        printf(" 4. View Recently Played (Circular Queue)\n");
        printf(" 5. Exit\n");
        printf("====================================================\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // clear invalid input
            continue;
        }
        while (getchar() != '\n'); // consume newline after number

        switch (choice) {
            case 1:
                printf("\nEnter Song ID: ");
                scanf("%d", &s.id);
                while (getchar() != '\n'); // consume newline
                printf("Enter Song Title: ");
                readLine(s.title, sizeof(s.title));
                printf("Enter Artist / Genre: ");
                readLine(s.artist, sizeof(s.artist));
                enqueueLinear(s);
                break;

            case 2:
                if (dequeueLinear(&s)) {
                    printf("\n>>> [PLAYING] ID: %d | \"%s\" by %s\n", s.id, s.title, s.artist);
                    enqueueCircular(s);
                }
                break;

            case 3:
                viewUpNext();
                break;

            case 4:
                viewRecentlyPlayed();
                break;

            case 5:
                printf("\nExiting program. Thank you!\n");
                return 0;

            default:
                printf("\nInvalid choice! Please select 1 to 5.\n");
        }
    }

    return 0;
}

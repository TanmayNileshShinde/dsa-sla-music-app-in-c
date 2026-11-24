#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEAR_SIZE 10
#define CIRCULAR_SIZE 5

typedef struct {
    int id;
    char title[50];
    char artist[50];
} Song;

/* Linear Queue */
Song linearQueue[LINEAR_SIZE];
int l_front = -1;
int l_rear = -1;

void enqueueLinear(Song s) {
    if (l_rear == LINEAR_SIZE - 1) {
        printf("\nQueue is full!\n");
        return;
    }
    if (l_front == -1) {
        l_front = 0;
    }
    l_rear++;
    linearQueue[l_rear] = s;
    printf("\nAdded: ID %d - %s\n", s.id, s.title);
}

int dequeueLinear(Song *s) {
    if (l_front == -1 || l_front > l_rear) {
        printf("\nQueue is empty!\n");
        return 0;
    }
    *s = linearQueue[l_front];
    l_front++;
    if (l_front > l_rear) {
        l_front = -1;
        l_rear = -1;
    }
    return 1;
}

void viewUpNext() {
    if (l_front == -1 || l_front > l_rear) {
        printf("\nQueue is empty.\n");
        return;
    }
    printf("\n--- Up Next ---\n");
    for (int i = l_front; i <= l_rear; i++) {
        printf("  %d. ID: %d - %s by %s\n", 
               (i - l_front + 1), linearQueue[i].id, linearQueue[i].title, linearQueue[i].artist);
    }
}

/* Circular Queue */
Song circularQueue[CIRCULAR_SIZE];
int c_front = -1;
int c_rear = -1;
int c_count = 0;

void enqueueCircular(Song s) {
    if (c_count == 0) {
        c_front = 0;
        c_rear = 0;
        circularQueue[c_rear] = s;
        c_count = 1;
    } else if (c_count == CIRCULAR_SIZE) {
        c_front = (c_front + 1) % CIRCULAR_SIZE;
        c_rear = (c_rear + 1) % CIRCULAR_SIZE;
        circularQueue[c_rear] = s;
    } else {
        c_rear = (c_rear + 1) % CIRCULAR_SIZE;
        circularQueue[c_rear] = s;
        c_count++;
    }
}

void viewRecentlyPlayed() {
    if (c_count == 0) {
        printf("\nNo songs played yet.\n");
        return;
    }
    printf("\n--- Recently Played ---\n");
    int curr = c_rear;
    for (int i = 0; i < c_count; i++) {
        printf("  %d. ID: %d - %s by %s\n", 
               (i + 1), circularQueue[curr].id, circularQueue[curr].title, circularQueue[curr].artist);
        curr = (curr - 1 + CIRCULAR_SIZE) % CIRCULAR_SIZE;
    }
}

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
    }
}

int main() {
    int choice;
    Song s;

    while (1) {
        printf("\n======================= MENU =======================\n");
        printf(" 1. Add Song\n");
        printf(" 2. Remove Song\n");
        printf(" 3. View Up Next\n");
        printf(" 4. View Recently Played\n");
        printf(" 5. Exit\n");
        printf("====================================================\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("\nEnter Song ID: ");
                scanf("%d", &s.id);
                while (getchar() != '\n');
                printf("Enter Song Name: ");
                readLine(s.title, sizeof(s.title));
                printf("Enter Artist: ");
                readLine(s.artist, sizeof(s.artist));
                enqueueLinear(s);
                break;

            case 2:
                if (dequeueLinear(&s)) {
                    printf("\nPlayed: ID %d - %s\n", s.id, s.title);
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
                printf("\nExiting. Thank you!\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}

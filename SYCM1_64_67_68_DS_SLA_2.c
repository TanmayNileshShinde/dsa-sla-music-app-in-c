#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEAR_SIZE 10
#define CIRCULAR_SIZE 5

/* Base Song Structure */
typedef struct {
    int id;
    char title[50];
    char artist[50];
    char genre[30];
} Song;

/* ========================================================
   REVIEW 1 MODULES: LINEAR & CIRCULAR QUEUES (CO1)
   ======================================================== */

/* Linear Queue - Up Next */
Song linearQueue[LINEAR_SIZE];
int l_front = -1;
int l_rear = -1;

/* Circular Queue - Recently Played */
Song circularQueue[CIRCULAR_SIZE];
int c_front = -1;
int c_rear = -1;
int c_count = 0;

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
        printf("  %d. ID: %d - %s by %s [%s]\n",
               (i - l_front + 1), linearQueue[i].id, linearQueue[i].title,
               linearQueue[i].artist, linearQueue[i].genre);
    }
}

void enqueueCircular(Song s) {
    if (c_count == 0) {
        c_front = 0;
        c_rear = 0;
    } else {
        c_rear = (c_rear + 1) % CIRCULAR_SIZE;
        if (c_count == CIRCULAR_SIZE) {
            c_front = (c_front + 1) % CIRCULAR_SIZE;
        }
    }
    circularQueue[c_rear] = s;
    if (c_count < CIRCULAR_SIZE) {
        c_count++;
    }
}

void viewRecentlyPlayed() {
    if (c_count == 0) {
        printf("\nRecently Played is empty.\n");
        return;
    }
    printf("\n--- Recently Played ---\n");
    int idx = c_rear;
    for (int i = 0; i < c_count; i++) {
        printf("  %d. ID: %d - %s by %s [%s]\n",
               (i + 1), circularQueue[idx].id, circularQueue[idx].title,
               circularQueue[idx].artist, circularQueue[idx].genre);
        idx = (idx - 1 + CIRCULAR_SIZE) % CIRCULAR_SIZE;
    }
}

/* ========================================================
   REVIEW 2 MODULE: DOUBLY LINKED LIST PLAYLIST (CO2)
   (To be implemented by Bhavik Waykole - Roll 64)
   ======================================================== */

typedef struct PlaylistNode {
    Song song;
    struct PlaylistNode *prev;
    struct PlaylistNode *next;
} PlaylistNode;

PlaylistNode *playlistHead = NULL;

void addToPlaylist(Song s) {
    PlaylistNode *newNode = (PlaylistNode *)malloc(sizeof(PlaylistNode));
    if (!newNode) {
        printf("\nMemory allocation failed!\n");
        return;
    }
    newNode->song = s;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (playlistHead == NULL) {
        playlistHead = newNode;
    } else {
        PlaylistNode *temp = playlistHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
    printf("\nAdded to Playlist: ID %d - %s\n", s.id, s.title);
}

void removeFromPlaylist(int id) {
    if (playlistHead == NULL) {
        printf("\nPlaylist is empty!\n");
        return;
    }

    PlaylistNode *curr = playlistHead;
    while (curr != NULL && curr->song.id != id) {
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("\nSong with ID %d not found in playlist.\n", id);
        return;
    }

    if (curr == playlistHead) {
        playlistHead = curr->next;
        if (playlistHead != NULL) {
            playlistHead->prev = NULL;
        }
    } else {
        if (curr->prev != NULL) {
            curr->prev->next = curr->next;
        }
        if (curr->next != NULL) {
            curr->next->prev = curr->prev;
        }
    }

    printf("\nRemoved from Playlist: ID %d - %s\n", curr->song.id, curr->song.title);
    free(curr);
}

void viewPlaylist() {
    if (playlistHead == NULL) {
        printf("\nPlaylist is empty.\n");
        return;
    }
    printf("\n--- Custom Playlist ---\n");
    PlaylistNode *temp = playlistHead;
    int count = 1;
    while (temp != NULL) {
        printf("  %d. ID: %d - %s by %s [%s]\n",
               count++, temp->song.id, temp->song.title,
               temp->song.artist, temp->song.genre);
        temp = temp->next;
    }
}


/* ========================================================
   REVIEW 2 MODULE: BINARY SEARCH TREE GENRE CATALOG (CO3)
   (Implemented by Tanmay Shinde - Roll 67)
   ======================================================== */

typedef struct GenreNode {
    char genre[30];
    struct GenreNode *left;
    struct GenreNode *right;
} GenreNode;

GenreNode *genreRoot = NULL;

GenreNode* createGenreNode(const char *genre) {
    GenreNode *newNode = (GenreNode *)malloc(sizeof(GenreNode));
    if (!newNode) {
        return NULL;
    }
    strncpy(newNode->genre, genre, 29);
    newNode->genre[29] = '\0';
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

GenreNode* insertGenreBST(GenreNode *root, const char *genre) {
    if (root == NULL) {
        return createGenreNode(genre);
    }
    int cmp = strcmp(genre, root->genre);
    if (cmp < 0) {
        root->left = insertGenreBST(root->left, genre);
    } else if (cmp > 0) {
        root->right = insertGenreBST(root->right, genre);
    }
    /* cmp == 0: genre already exists in tree, no duplicate */
    return root;
}

int inorderCount = 0;
void inorderGenres(GenreNode *root) {
    if (root != NULL) {
        inorderGenres(root->left);
        inorderCount++;
        printf("  %d. %s\n", inorderCount, root->genre);
        inorderGenres(root->right);
    }
}

int preorderCount = 0;
void preorderGenres(GenreNode *root) {
    if (root != NULL) {
        preorderCount++;
        printf("  %d. %s\n", preorderCount, root->genre);
        preorderGenres(root->left);
        preorderGenres(root->right);
    }
}

int postorderCount = 0;
void postorderGenres(GenreNode *root) {
    if (root != NULL) {
        postorderGenres(root->left);
        postorderGenres(root->right);
        postorderCount++;
        printf("  %d. %s\n", postorderCount, root->genre);
    }
}

/* ========================================================
   REVIEW 2 MODULE: BST SEARCH & MENU INTEGRATION (CO3)
   (To be implemented by Aditya Shah - Roll 68)
   ======================================================== */

/* Placeholder: Aditya will implement searchGenreBST here */


/* Helper to safely read a line from stdin */
void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

/* ========================================================
   MAIN CONSOLE CONTROLLER
   ======================================================== */

int main() {
    int choice;
    Song s;
    char buffer[100];

    while (1) {
        printf("\n================ MENU ================\n");
        printf(" 1. Add Song\n");
        printf(" 2. Remove Song\n");
        printf(" 3. View Up Next\n");
        printf(" 4. View Recently Played\n");
        printf(" 5. Add to Playlist\n");
        printf(" 6. Remove from Playlist\n");
        printf(" 7. View Playlist\n");
        printf(" 8. Genres (Inorder)\n");
        printf(" 9. Genres (Preorder)\n");
        printf("10. Genres (Postorder)\n");
        printf("11. Search Genre\n");
        printf("12. Exit\n");
        printf("======================================\n");
        printf("Enter your choice (1-12): ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number between 1 and 12.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("\nEnter Song ID: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &s.id);

                printf("Enter Song Name: ");
                readLine(s.title, sizeof(s.title));

                printf("Enter Artist: ");
                readLine(s.artist, sizeof(s.artist));

                printf("Enter Genre: ");
                readLine(s.genre, sizeof(s.genre));

                enqueueLinear(s);
                genreRoot = insertGenreBST(genreRoot, s.genre);
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
                printf("\nEnter Song ID: ");
                fgets(buffer, sizeof(buffer), stdin);
                sscanf(buffer, "%d", &s.id);

                printf("Enter Song Name: ");
                readLine(s.title, sizeof(s.title));

                printf("Enter Artist: ");
                readLine(s.artist, sizeof(s.artist));

                printf("Enter Genre: ");
                readLine(s.genre, sizeof(s.genre));

                addToPlaylist(s);
                genreRoot = insertGenreBST(genreRoot, s.genre);
                break;

            case 6:
                printf("\nEnter Song ID to remove from playlist: ");
                fgets(buffer, sizeof(buffer), stdin);
                int remId;
                sscanf(buffer, "%d", &remId);
                removeFromPlaylist(remId);
                break;

            case 7:
                viewPlaylist();
                break;

            case 8:
                if (genreRoot == NULL) {
                    printf("\nNo genres recorded in tree yet.\n");
                } else {
                    printf("\n--- Genres in INORDER (Alphabetical A-Z) ---\n");
                    inorderCount = 0;
                    inorderGenres(genreRoot);
                }
                break;

            case 9:
                if (genreRoot == NULL) {
                    printf("\nNo genres recorded in tree yet.\n");
                } else {
                    printf("\n--- Genres in PREORDER (Root-First) ---\n");
                    preorderCount = 0;
                    preorderGenres(genreRoot);
                }
                break;

            case 10:
                if (genreRoot == NULL) {
                    printf("\nNo genres recorded in tree yet.\n");
                } else {
                    printf("\n--- Genres in POSTORDER (Bottom-Up) ---\n");
                    postorderCount = 0;
                    postorderGenres(genreRoot);
                }
                break;

            case 11:
                printf("\n[Feature under development by Aditya (Roll 68) - BST Search]\n");
                break;

            case 12:
                printf("\nExiting program...\n");
                return 0;

            default:
                printf("\nInvalid choice! Please choose between 1 and 12.\n");
        }
    }

    return 0;
}

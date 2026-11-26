#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINEAR_SIZE 10
#define CIRCULAR_SIZE 5
#define HASH_SIZE 10

typedef struct {
    int id;
    char title[50];
    char artist[50];
    char genre[30];
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
    printf("\nAdded to Queue: ID %d - %s\n", s.id, s.title);
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
    printf("\n--- Play Queue ---\n");
    for (int i = l_front; i <= l_rear; i++) {
        printf("  %d. ID: %d - %s by %s [%s]\n",
               (i - l_front + 1), linearQueue[i].id, linearQueue[i].title,
               linearQueue[i].artist, linearQueue[i].genre);
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
        printf("\nNo songs played yet.\n");
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

/* Doubly Linked List - Custom Playlist */
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
    printf("\nAdded: ID %d - %s\n", s.id, s.title);
}

int removeFromPlaylist(int id) {
    if (playlistHead == NULL) {
        printf("\nPlaylist is empty!\n");
        return 0;
    }

    PlaylistNode *curr = playlistHead;
    while (curr != NULL && curr->song.id != id) {
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("\nSong ID %d not found!\n", id);
        return 0;
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

    printf("\nRemoved: ID %d - %s\n", curr->song.id, curr->song.title);
    free(curr);
    return 1;
}

void viewPlaylist() {
    if (playlistHead == NULL) {
        printf("\nPlaylist is empty.\n");
        return;
    }
    printf("\n--- Playlist ---\n");
    PlaylistNode *temp = playlistHead;
    int count = 1;
    while (temp != NULL) {
        printf("  %d. ID: %d - %s by %s [%s]\n",
               count++, temp->song.id, temp->song.title,
               temp->song.artist, temp->song.genre);
        temp = temp->next;
    }
}

/* Binary Search Tree - Genre Catalog */
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

int searchGenreBST(GenreNode *root, const char *genre) {
    if (root == NULL) {
        return 0;
    }
    int cmp = strcmp(genre, root->genre);
    if (cmp == 0) {
        return 1;
    } else if (cmp < 0) {
        return searchGenreBST(root->left, genre);
    } else {
        return searchGenreBST(root->right, genre);
    }
}

/* Hash Table - Song ID Lookup */
typedef struct HashNode {
    Song song;
    struct HashNode *next;
} HashNode;

HashNode *hashTable[HASH_SIZE] = {NULL};

int hashFunction(int id) {
    int h = id % HASH_SIZE;
    return (h < 0) ? -h : h;
}

void insertHash(Song s) {
    int idx = hashFunction(s.id);
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newNode) {
        return;
    }
    newNode->song = s;
    newNode->next = hashTable[idx];
    hashTable[idx] = newNode;
}

Song* searchSongByID(int id) {
    int idx = hashFunction(id);
    HashNode *curr = hashTable[idx];
    while (curr != NULL) {
        if (curr->song.id == id) {
            return &(curr->song);
        }
        curr = curr->next;
    }
    return NULL;
}

void deleteFromHash(int id) {
    int idx = hashFunction(id);
    HashNode *curr = hashTable[idx];
    HashNode *prev = NULL;
    while (curr != NULL && curr->song.id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        return;
    }
    if (prev == NULL) {
        hashTable[idx] = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
}

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void freeAllMemory() {
    while (playlistHead != NULL) {
        PlaylistNode *tmp = playlistHead;
        playlistHead = playlistHead->next;
        free(tmp);
    }
    for (int i = 0; i < HASH_SIZE; i++) {
        while (hashTable[i] != NULL) {
            HashNode *tmp = hashTable[i];
            hashTable[i] = hashTable[i]->next;
            free(tmp);
        }
    }
}

/* Main Menu */
int main() {
    int choice;
    Song s;
    char buffer[100];

    while (1) {
        printf("\n================ MENU ================\n");
        printf(" 1. Add Song\n");
        printf(" 2. Remove Song\n");
        printf(" 3. View Playlist\n");
        printf(" 4. Add to Queue\n");
        printf(" 5. Play Song\n");
        printf(" 6. View Queue\n");
        printf(" 7. Recently Played\n");
        printf(" 8. Genres (Inorder)\n");
        printf(" 9. Genres (Preorder)\n");
        printf("10. Genres (Postorder)\n");
        printf("11. Search Genre\n");
        printf("12. Search Song\n");
        printf("13. Exit\n");
        printf("======================================\n");
        printf("Enter your choice (1-13): ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("\nInvalid input!\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("\nEnter Song ID: ");
                fgets(buffer, sizeof(buffer), stdin);
                if (sscanf(buffer, "%d", &s.id) != 1) {
                    printf("\nInvalid Song ID!\n");
                    break;
                }
                if (searchSongByID(s.id) != NULL) {
                    printf("\nSong ID %d already exists!\n", s.id);
                    break;
                }

                printf("Enter Song Name: ");
                readLine(s.title, sizeof(s.title));

                printf("Enter Artist: ");
                readLine(s.artist, sizeof(s.artist));

                printf("Enter Genre: ");
                readLine(s.genre, sizeof(s.genre));

                addToPlaylist(s);
                insertHash(s);
                genreRoot = insertGenreBST(genreRoot, s.genre);
                break;

            case 2: {
                if (playlistHead == NULL) {
                    printf("\nPlaylist is empty!\n");
                    break;
                }
                viewPlaylist();
                printf("\nEnter Song ID to remove: ");
                fgets(buffer, sizeof(buffer), stdin);
                int remId;
                if (sscanf(buffer, "%d", &remId) == 1) {
                    if (removeFromPlaylist(remId)) {
                        deleteFromHash(remId);
                    }
                }
                break;
            }

            case 3:
                viewPlaylist();
                break;

            case 4: {
                if (playlistHead == NULL) {
                    printf("\nPlaylist is empty! Add songs first.\n");
                    break;
                }
                viewPlaylist();
                printf("\nEnter Song ID to queue: ");
                fgets(buffer, sizeof(buffer), stdin);
                int qId;
                if (sscanf(buffer, "%d", &qId) != 1) {
                    printf("\nInvalid Song ID!\n");
                    break;
                }
                Song *found = searchSongByID(qId);
                if (found != NULL) {
                    enqueueLinear(*found);
                } else {
                    printf("\nSong ID %d not found!\n", qId);
                }
                break;
            }

            case 5:
                if (dequeueLinear(&s)) {
                    printf("\nPlayed: ID %d - %s\n", s.id, s.title);
                    enqueueCircular(s);
                }
                break;

            case 6:
                viewUpNext();
                break;

            case 7:
                viewRecentlyPlayed();
                break;

            case 8:
                if (genreRoot == NULL) {
                    printf("\nNo genres yet.\n");
                } else {
                    printf("\n--- Genres (Inorder) ---\n");
                    inorderCount = 0;
                    inorderGenres(genreRoot);
                }
                break;

            case 9:
                if (genreRoot == NULL) {
                    printf("\nNo genres yet.\n");
                } else {
                    printf("\n--- Genres (Preorder) ---\n");
                    preorderCount = 0;
                    preorderGenres(genreRoot);
                }
                break;

            case 10:
                if (genreRoot == NULL) {
                    printf("\nNo genres yet.\n");
                } else {
                    printf("\n--- Genres (Postorder) ---\n");
                    postorderCount = 0;
                    postorderGenres(genreRoot);
                }
                break;

            case 11:
                if (genreRoot == NULL) {
                    printf("\nNo genres yet.\n");
                } else {
                    char searchKey[30];
                    printf("\nEnter Genre to search: ");
                    readLine(searchKey, sizeof(searchKey));
                    if (searchGenreBST(genreRoot, searchKey)) {
                        printf("\nGenre '%s' found!\n", searchKey);
                    } else {
                        printf("\nGenre '%s' not found!\n", searchKey);
                    }
                }
                break;

            case 12: {
                printf("\nEnter Song ID to search: ");
                fgets(buffer, sizeof(buffer), stdin);
                int searchId;
                if (sscanf(buffer, "%d", &searchId) != 1) {
                    printf("\nInvalid Song ID!\n");
                    break;
                }
                Song *res = searchSongByID(searchId);
                if (res != NULL) {
                    printf("\nSong Found:\n");
                    printf("  ID: %d - %s by %s [%s]\n",
                           res->id, res->title, res->artist, res->genre);
                } else {
                    printf("\nSong ID %d not found!\n", searchId);
                }
                break;
            }

            case 13:
                freeAllMemory();
                printf("\nExiting...\n");
                return 0;

            default:
                printf("\nInvalid choice!\n");
        }
    }

    freeAllMemory();
    return 0;
}
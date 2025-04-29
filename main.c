#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Kullanici ve arkadaslik yapilari
typedef struct FriendNode FriendNode;
typedef struct User User;

struct FriendNode {
    User *user;
    FriendNode *next;
};

enum Color { RED, BLACK };

struct User {
    int id;
    FriendNode *friends;
    // RB Tree ozellikleri
    User *parent;
    User *left;
    User *right;
    enum Color color;
    bool visited;
};

// RB Tree global kok
User *root = NULL;

// Yardimci veri yapilari
typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct List {
    ListNode *head;
} List;

typedef struct StackNode {
    User *data;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;
} Stack;

typedef struct QueueNode {
    User *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

// Temel veri yapisi islemleri
List *create_list() {
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void add_to_list(List *list, int data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    node->data = data;
    node->next = list->head;
    list->head = node;
}

Stack *create_stack() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void stack_push(Stack *stack, User *data) {
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    node->data = data;
    node->next = stack->top;
    stack->top = node;
}

User *stack_pop(Stack *stack) {
    if (stack->top == NULL) return NULL;
    StackNode *temp = stack->top;
    User *data = temp->data;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

int stack_empty(Stack *stack) {
    return stack->top == NULL;
}

Queue *create_queue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, User *data) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->data = data;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
        return;
    }
    q->rear->next = node;
    q->rear = node;
}

User *dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    User *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return data;
}

int queue_empty(Queue *q) {
    return q->front == NULL;
}

// RB Tree islemleri
void rotate_left(User **root, User *x) {
    User *y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotate_right(User **root, User *x) {
    User *y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void insert_fixup(User **root, User *z) {
    while (z != *root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            User *y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(root, z->parent->parent);
            }
        } else {
            User *y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void insert_user(User **root, User *node) {
    User *y = NULL;
    User *x = *root;
    while (x != NULL) {
        y = x;
        if (node->id < x->id)
            x = x->left;
        else
            x = x->right;
    }
    node->parent = y;
    if (y == NULL)
        *root = node;
    else if (node->id < y->id)
        y->left = node;
    else
        y->right = node;
    node->left = node->right = NULL;
    node->color = RED;
    insert_fixup(root, node);
}

User *create_user(int id) {
    User *user = (User *)malloc(sizeof(User));
    user->id = id;
    user->friends = NULL;
    user->parent = user->left = user->right = NULL;
    user->color = RED;
    user->visited = false;
    return user;
}

User *find_user(User *root, int id) {
    User *current = root;
    while (current != NULL && current->id != id) {
        if (id < current->id)
            current = current->left;
        else
            current = current->right;
    }
    return current;
}

void add_friend(User *user, User *friend) {
    FriendNode *node = (FriendNode *)malloc(sizeof(FriendNode));
    node->user = friend;
    node->next = user->friends;
    user->friends = node;
}

// Ziyaret durumunu sifirla
void reset_visited(User *root) {
    if (root == NULL) return;
    reset_visited(root->left);
    root->visited = false;
    reset_visited(root->right);
}

// DFS ile belirli mesafedeki arkadaslari bulma
void dfs(User *current, int depth, int target, List *result) {
    if (current == NULL || current->visited) return;
    
    if (depth == target) {
        add_to_list(result, current->id);
        return;
    }
    
    current->visited = true;
    FriendNode *friend = current->friends;
    while (friend != NULL) {
        dfs(friend->user, depth + 1, target, result);
        friend = friend->next;
    }
    current->visited = false; 
}

List *find_users_at_distance(User *user, int distance) {
    List *result = create_list();
    reset_visited(root);
    dfs(user, 0, distance, result);
    return result;
}

// Ortak arkadas analizi
List *find_common_friends(User *a, User *b) {
    List *common = create_list();
    FriendNode *friendA = a->friends;
    while (friendA != NULL) {
        FriendNode *friendB = b->friends;
        while (friendB != NULL) {
            if (friendA->user == friendB->user) {
                add_to_list(common, friendA->user->id);
                break;
            }
            friendB = friendB->next;
        }
        friendA = friendA->next;
    }
    return common;
}

// Topluluk tespiti (Bagli bilesenler)
void dfs_community(User *user, List *community) {
    if (user->visited) return;
    user->visited = true;
    add_to_list(community, user->id);
    FriendNode *friend = user->friends;
    while (friend != NULL) {
        dfs_community(friend->user, community);
        friend = friend->next;
    }
}

List *find_communities() {
    List *communities = create_list();
    reset_visited(root);
    Stack *stack = create_stack();
    stack_push(stack, root);
    
    while (!stack_empty(stack)) {
        User *current = stack_pop(stack);
        if (!current->visited) {
            List *community = create_list();
            dfs_community(current, community);
            // Tüm topluluðu ekle
            ListNode *node = community->head;
            while (node != NULL) {
                add_to_list(communities, node->data);
                node = node->next;
            }
        }
    }
    return communities;
}

// Etki alani hesaplama
int calculate_influence(User *user) {
    reset_visited(root);
    Queue *q = create_queue();
    int count = 0;
    enqueue(q, user);
    user->visited = true;
    while (!queue_empty(q)) {
        User *current = dequeue(q);
        count++;
        FriendNode *friend = current->friends;
        while (friend != NULL) {
            if (!friend->user->visited) {
                friend->user->visited = true;
                enqueue(q, friend->user);
            }
            friend = friend->next;
        }
    }
    return count - 1; // Kendisi haric
}

// Dosyadan veri okuma
void read_dataset(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "USER", 4) == 0) {
            int id;
            sscanf(line, "USER %d", &id);
            User *new_user = create_user(id);
            insert_user(&root, new_user);
        } else if (strncmp(line, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(line, "FRIEND %d %d", &id1, &id2);
            User *u1 = find_user(root, id1);
            User *u2 = find_user(root, id2);
            if (u1 && u2) {
            add_friend(u1, u2);
            add_friend(u2, u1);
         }
        }
    }
    fclose(file);
}

// Test fonksiyonlari
void print_list(List *list) {  
    ListNode *current = list->head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    read_dataset("veriseti.txt");
    
    // Ornek kullanim
    User *user101 = find_user(root, 101);
    printf("101'in 2 uzakliktaki arkadaslari: ");
    List *friends = find_users_at_distance(user101, 2);
    print_list(friends);
    
    User *user104 = find_user(root, 104);
    List *common = find_common_friends(user101, user104);
    printf("101 ve 104 ortak arkadaslari: ");
    print_list(common);
    
    printf("Topluluklar: ");
    List *communities = find_communities();
    print_list(communities);
    
    printf("101'in etki alani: %d\n", calculate_influence(user101));
    
    return 0;
}

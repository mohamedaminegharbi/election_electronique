#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    administrator,
    voter,
    candidate
} role;

typedef struct {
    role role;
    char firstName[50];
    char lastName[50];
    long idNumber;
    int hasVoted;
} person;

typedef struct {
    char firstName[50];
    char lastName[50];
    int id;
    int votes;
} candidacy;

// Definition of the Stack structure
typedef struct Stack {
    int top;
    unsigned capacity;
    candidacy* array;
} Stack;

// Function to create a stack of given capacity
Stack* createStack(unsigned capacity) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (candidacy*) malloc(stack->capacity * sizeof(candidacy));
    return stack;
}

// Function to add an element to the stack
void push(Stack* stack, candidacy item) {
    stack->array[++stack->top] = item;
}

// Definition of the Queue structure
typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    person* array;
} Queue;

// Function to create a queue of given capacity
Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (person*) malloc(queue->capacity * sizeof(person));
    return queue;
}

// Function to add an element to the queue
void enqueue(Queue* queue, person item) {
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to check if an element exists in the queue
int idExistsInQueue(Queue* queue, long idNumber) {
    int count = queue->size;
    int i = queue->front;
    while (count--) {
        if (queue->array[i].idNumber == idNumber) {
            return (i - queue->front + queue->capacity) % queue->capacity;
        }
        i = (i + 1) % queue->capacity;
    }
    return -1;
}

// Function to check if an element exists in the stack
int idExistsInStack(Stack* stack, long idNumber) {
    for (int i = 0; i <= stack->top; i++) {
        if (stack->array[i].id == idNumber) {
            return i;
        }
    }
    return -1;
}

int main() {
    int numAdministrators, numVoters;

    printf("Enter the number of administrators: ");
    scanf("%d", &numAdministrators);
    printf("Enter the number of voters: ");
    scanf("%d", &numVoters);

    // Create the queue for people
    Queue* people = createQueue(numAdministrators + numVoters);

    // Create the stack for candidates
    Stack* candidates = createStack(numVoters);

    int inputRole;
    char inputFirstName[50];
    char inputLastName[50];
    long inputIdNumber;
    int inputVote;
    int numCandidates = 0;
    int index;

    printf("We will enter the administrators' information.\n");
    for (int i = 0; i < numAdministrators; i++) {
        person p;
        do {
            printf("Enter ID number: ");
            scanf("%ld", &inputIdNumber);
            while ((getchar()) != '\n');
            index = idExistsInQueue(people, inputIdNumber);
            if (index != -1) {
                printf("This ID already exists. Please verify and enter a unique ID.\n");
            }
        } while (idExistsInQueue(people, inputIdNumber) != -1);

        p.idNumber = inputIdNumber;

        printf("Enter first name: ");
        scanf("%s", p.firstName);
        while ((getchar()) != '\n');

        printf("Enter last name: ");
        scanf("%s", p.lastName);

        p.role = administrator;
        p.hasVoted = 0;

        enqueue(people, p);
    }

    printf("We will enter the students' information.\n");
    for (int i = 0; i < numVoters; i++) {
        person p;
        candidacy c;

        do {
            printf("Enter ID number: ");
            scanf("%ld", &inputIdNumber);
            while ((getchar()) != '\n');

            int index = idExistsInQueue(people, inputIdNumber);
            if (index != -1) {
                if (people->array[index].role == administrator) {
                    printf("This ID already belongs to an administrator. We are currently only entering student information. Please enter a new ID.\n");
                } else {
                    printf("This ID already belongs to a student. Please verify and enter a unique ID.\n");
                }
            }
        } while (idExistsInQueue(people, inputIdNumber) != -1);

        p.idNumber = inputIdNumber;

        printf("Please enter your first name: ");
        scanf("%s", p.firstName);
        while ((getchar()) != '\n');

        printf("Please enter your last name: ");
        scanf("%s", p.lastName);
        while ((getchar()) != '\n');

        printf("Do you want to run for candidacy? (0 for no, 1 for yes): ");
        scanf("%d", &inputRole);

        if (inputRole == 1) {
            // If the student is a candidate, add them to the 'candidates' stack
            c.id = candidates->top + 1;
            strcpy(c.firstName, p.firstName);
            strcpy(c.lastName, p.lastName);
            c.votes = 0;  // Initialize votes to 0
            push(candidates, c);  // Add the candidate to the stack

            FILE *file1 = fopen("candidates.txt", "w");
            fprintf(file1, "%d %s %s %d\n", c.id, c.firstName, c.lastName, c.votes);
            fclose(file1);
        }

        p.role = inputRole == 1 ? candidate : voter;
        p.hasVoted = 0;
        enqueue(people, p);  // Add the person to the queue

        FILE *file2 = fopen("voters.txt", "r");
        fscanf(file2, "%ld %s %s %d\n", &p.idNumber, p.firstName, p.lastName, &p.role);
        fclose(file2);
    }

    // Implement the voting process
    int i = 0;
    while (i < numVoters) {
        person p;

        printf("Enter ID number: ");
        scanf("%ld", &inputIdNumber);
        while ((getchar()) != '\n');

        int index = idExistsInQueue(people, inputIdNumber);
        if (index != -1) {
            // The ID number exists, so set the hasVoted flag to 1
            people->array[index].hasVoted = 1;

            // Now check if they're allowed to vote
            if (people->array[index].role != administrator) {
                for (int j = 0; j <= candidates->top; j++) {
                    printf("%d. %s\n", j, candidates->array[j].firstName);
                }

                do {
                    printf("Enter the number of the candidate you want to vote for: ");
                    scanf("%d", &inputVote);
                    if (inputVote < 0 || inputVote > candidates->top) {
                        printf("This number is not available. Please choose a valid number.\n");
                    }
                } while (inputVote < 0 || inputVote > candidates->top);

                // Update the votes for the chosen candidate
                candidates->array[inputVote].votes++;
                i++;
            } else {
                printf("You are an administrator. Enter 0 to stop the elections: ");
                scanf("%d", &inputVote);
                if (inputVote == 0) {
                    printf("The elections have been stopped by an administrator.\n");
                    exit(0);
                } else {
                    printf("The code is not valid. The elections continue.\n");
                }
            }
        } else {
            printf("Your ID is not registered, please verify your information.\n");
        }
    }

    for (int i = 0; i <= candidates->top; i++) {
        printf("Candidate number %d, votes: %d\n", i, candidates->array[i].votes);
    }

    int maxVotes = 0;
    int winners[candidates->top + 1];
    int numWinners = 0;

    for (int i = 0; i <= candidates->top; i++) {
        if (candidates->array[i].votes > maxVotes) {
            maxVotes = candidates->array[i].votes;
            winners[0] = i;
            numWinners = 1;
        } else if (candidates->array[i].votes == maxVotes) {
            winners[numWinners] = i;
            numWinners++;
        }
    }

    if (numWinners == 1) {
        printf("The winner is: %s\n", candidates->array[winners[0]].firstName);

        FILE *file3 = fopen("results.txt", "w");
        fprintf(file3, "The winner of the election is %s %s with %d votes.\n", candidates->array[winners[0]].firstName, candidates->array[winners[0]].lastName, candidates->array[winners[0]].votes);
        fclose(file3);

    } else {
        printf("There is a tie between the candidates\n");
        for (int i = 0; i < numWinners; i++) {
            printf("%s\n", candidates->array[winners[i]].firstName);

            FILE *file3 = fopen("results.txt", "w");
            fprintf(file3, "The winner of the election is %s %s with %d votes.\n", candidates->array[winners[i]].firstName, candidates->array[winners[i]].lastName, candidates->array[winners[i]].votes);
            fclose(file3);
        }
    }

    return 0;
}



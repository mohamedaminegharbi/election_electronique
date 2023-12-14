#include <stdio.h>
#include <string.h>

// Define the 'fonction' enumeration
typedef enum {
    administrateur,
    electeur,
    candidat 
} fonction;

// Define the 'personne' structure
typedef struct {
    fonction role;
    char nom[50];
    char prenom[50];
    long numero_de_carte;  // Changed to long
    int deja_voter;
} personne;

// Define the 'candidat' structure
typedef struct {
    char nom[50];
    char prenom[50];
    int num;
    int votes;
} candidature;

int num_exist(personne* array, int taille, long numero_de_carte) {
    for (int i = 0; i < taille; i++) {
        if (array[i].numero_de_carte == numero_de_carte) {
            return i;  // Card number found, return the index
        }
    }
    return -1;  // Card number not found
}

int main() {
    // Variables to hold the number of each role
    int num_administrateurs, num_electeurs;

    // Prompt the user to enter the number of each role
    printf("Enter the number of administrateurs: ");
    scanf("%d", &num_administrateurs);
    printf("Enter the number of electeurs: ");
    scanf("%d", &num_electeurs);

    // Create arrays of 'personne' and 'candidat'
    personne personnes[num_administrateurs + num_electeurs];
    personne administrateurs[num_administrateurs];  // Separate array for administrators
    personne etudiants[num_electeurs];  // Separate array for students
    candidature candidats[num_electeurs];  // Maximum possible number of candidates is the number of students

    // Variables to hold user input
    int input_role;
    char input_nom[50];
    char input_prenom[50];
    long input_numero_de_carte;  // Changed to long
    int input_vote;
    int num_candidats = 0;  // Initialize the number of candidates to 0

    // Fill the 'administrateurs' array with actual data
    for (int i=0; i<num_administrateurs; i++) {
        do {
            printf("Enter numero_de_carte: ");
            scanf("%ld", &input_numero_de_carte);  // Use %ld for long
            while ((getchar()) != '\n');
            
            if (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1) {
                printf("This identity number already exists. Please verify and enter a unique number.\n");
            }
        } while (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1);

        printf("Enter nom: ");
        scanf("%s", input_nom);
        while ((getchar()) != '\n');

        printf("Enter prenom: ");
        fgets(input_prenom, sizeof(input_prenom), stdin);

        administrateurs[i].role = administrateur;
        strcpy(administrateurs[i].nom, input_nom);
        strcpy(administrateurs[i].prenom, input_prenom);
        administrateurs[i].numero_de_carte = input_numero_de_carte;
        
        personnes[i].role = administrateur;
        strcpy(personnes[i].nom, input_nom);
        strcpy(personnes[i].prenom, input_prenom);
        personnes[i].numero_de_carte = input_numero_de_carte;
    }

    //fill candidats and etudiants
    for (int i=0; i<num_electeurs; i++) {
        do {
            printf("saisissez votre numero de carte d'identite : ");
            scanf("%ld", &input_numero_de_carte);  // Use %ld for long
            while ((getchar()) != '\n');
            if (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1) {
                printf("Ce numero d identite existe deja. Veuillez verifier et entrer un numero unique..\n");
            }
        } while (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1);

        printf("saisissez votre nom: ");
        scanf("%s", input_nom);
        while ((getchar()) != '\n');

        printf("saisissez votre prenom: ");
        fgets(input_prenom, sizeof(input_prenom), stdin);

        printf("voulez-vous postuler votre candidature ? (0 pour non, 1 pour oui): ");
        scanf("%d", &input_role);

        if (input_role == 1) {
            // If the student is a candidate, add them to the 'candidats' array
            candidats[num_candidats].num = num_candidats;
            strcpy(candidats[num_candidats].nom, input_nom);
            strcpy(candidats[num_candidats].prenom, input_prenom);
            candidats[num_candidats].votes = 0;  // Initialize votes to 0
            num_candidats++;  // Increase the number of candidates
        }

        etudiants[i].role = input_role == 1 ? candidat : electeur;
        strcpy(etudiants[i].nom, input_nom);
        strcpy(etudiants[i].prenom, input_prenom);
        etudiants[i].numero_de_carte = input_numero_de_carte;
        etudiants[i].deja_voter=0;

        personnes[num_administrateurs + i].role = input_role == 1 ? candidat : electeur;
        strcpy(personnes[num_administrateurs + i].nom, input_nom);
        strcpy(personnes[num_administrateurs + i].prenom, input_prenom);
        personnes[num_administrateurs + i].numero_de_carte = input_numero_de_carte;
    }


    // Implement the voting process
    int i = 0;
    while (i < num_electeurs) {
        printf("saisissez votre numero de carte d'identite : ");
        scanf("%ld", &input_numero_de_carte);
        while ((getchar()) != '\n');
        
        int index = num_exist(etudiants, num_electeurs, input_numero_de_carte);
        if (index != -1 && !etudiants[index].deja_voter) {
            for (int j = 0; j < num_candidats; j++) {
                printf("%d. %s\n", j, candidats[j].nom);
            }
            
            do {
                printf("Entrez le numéro du candidat pour lequel vous voulez voter: ");
                scanf("%d", &input_vote);
                
                if (input_vote < 0 || input_vote >= num_candidats) {
                    printf("Ce numéro n'est pas disponible. Veuillez choisir un numéro valide.\n");
                }
            } while (input_vote < 0 || input_vote >= num_candidats);

            // Mettre à jour les votes du candidat choisi
            candidats[input_vote].votes++;

            etudiants[index].deja_voter = 1;  // Marquer l'étudiant comme ayant voté
            i++;  // Incrémenter i seulement si num_existe ne renvoie pas -1 et que l'étudiant n'a pas déjà voté
        } else {
            if (index != -1) {
                printf("Vous avez deja vote. Chaque etudiant ne peut voter qu'une seule fois.\n");
            } else {
                printf("Votre carte d'identite n'est pas enregistree, veuillez verifier vos informations.\n");
            }
        }
    }


    // Find the candidate with the most votes
    int max_votes = 0;
    int winner = 0;
    for (int i = 0; i < num_candidats; i++) {
        if (candidats[i].votes > max_votes) {
            max_votes = candidats[i].votes;
            winner = i;
        }
    }

    // Print the winner
    printf("le gagnant est : %s\n", candidats[winner].nom);

    return 0;
}

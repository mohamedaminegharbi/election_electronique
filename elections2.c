#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    administrateur,
    electeur,
    candidat 
} fonction;

typedef struct {
    fonction role;
    char nom[50];
    char prenom[50];
    long numero_de_carte;  
    int deja_voter;
} personne;

typedef struct {
    char nom[50];
    char prenom[50];
    int num;
    int votes;
} candidature;

int num_exist(personne* array, int taille, long numero_de_carte) {
    for (int i = 0; i < taille; i++) {
        if (array[i].numero_de_carte == numero_de_carte) {
            return i;  
        }
    }
    return -1;  
}

int main() {

    int num_administrateurs, num_electeurs;

    printf("Entrer le nombre des administrateurs: ");
    scanf("%d", &num_administrateurs);
    printf("Entrez le nombre des electeurs: ");
    scanf("%d", &num_electeurs);

    personne personnes[num_administrateurs + num_electeurs];
    candidature candidats[num_electeurs];  

    int input_role;
    char input_nom[50];
    char input_prenom[50];
    long input_numero_de_carte;  
    int input_vote;
    int num_candidats = 0;  

    printf("On va saisir les informations des administrateurs.\n");
    for (int i=0; i<num_administrateurs; i++) {

        do {
            
            printf("Entrez le numero de carte: ");
            scanf("%ld", &input_numero_de_carte);  
            while ((getchar()) != '\n');
            
            int index = num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte);
            if (index != -1) {
                printf("Ce num d identite existe deja. Veuillez verifier et entrer un num unique.\n");
            }
            
        } while (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1);

        
        printf("Entrez nom: ");
        scanf("%s", input_nom);
        while ((getchar()) != '\n');
        
        printf("Entrez prenom: ");
        fgets(input_prenom, sizeof(input_prenom), stdin);
        
        personnes[i].role = administrateur;
        strcpy(personnes[i].nom, input_nom);
        strcpy(personnes[i].prenom, input_prenom);
        personnes[i].numero_de_carte = input_numero_de_carte;
    }

    printf("On va saisir les informations des etudiants.\n");
    for (int i=0; i<num_electeurs; i++) {

        do {
            printf("Entrez le numero de carte: ");
            scanf("%ld", &input_numero_de_carte);  
            while ((getchar()) != '\n');
            
            int index = num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte);
            if (index != -1) {
                if (personnes[index].role == administrateur) {
                    printf("Ce num d identite appartient deja a un administrateur. Nous saisissons actuellement uniquement les informations des étudiants. Veuillez entrer un nouveau numéro.\n");
                } else {
                    printf("Ce numero d identite appartient deja a un etudiant. Veuillez verifier et entrer un num unique.\n");
                }
            }
        } while (num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte) != -1);
        
        printf("Veuillez saisir votre nom : ");
        scanf("%s", input_nom);
        while ((getchar()) != '\n');
        
        printf("Veuillez saisir votre prenom : ");
        fgets(input_prenom, sizeof(input_prenom), stdin);
        
        printf("Voulez-vous postuler votre candidature ? (0 pour non, 1 pour oui) : ");
        scanf("%d", &input_role);
        
        if (input_role == 1) {
            // Si l'étudiant est un candidat, ajoutez-le au tableau 'candidats'
            candidats[num_candidats].num = num_candidats;
            strcpy(candidats[num_candidats].nom, input_nom);
            strcpy(candidats[num_candidats].prenom, input_prenom);
            candidats[num_candidats].votes = 0;  // Initialisez les votes à 0
            num_candidats++;  // Augmentez le nombre de candidats
        }
        
        personnes[num_administrateurs + i].role = input_role == 1 ? candidat : electeur;
        strcpy(personnes[num_administrateurs + i].nom, input_nom);
        strcpy(personnes[num_administrateurs + i].prenom, input_prenom);
        personnes[num_administrateurs + i].numero_de_carte = input_numero_de_carte;
        personnes[num_administrateurs + i].deja_voter=0;
    }



    // Implementer le processus du vote 
    int i = 0;
    while (i < num_electeurs) {
        printf("Entrez le num de carte: ");
        scanf("%ld", &input_numero_de_carte);
        while ((getchar()) != '\n');
        
        int index = num_exist(personnes, num_administrateurs + num_electeurs, input_numero_de_carte);
        if (index != -1 && !personnes[index].deja_voter && personnes[index].role != administrateur) {
            for (int j = 0; j < num_candidats; j++) {
                printf("%d. %s\n", j, candidats[j].nom);
            }
            
            do {
                printf("Entrez le num du candidat pour lequel vous voulez voter: ");
                scanf("%d", &input_vote);
                if (input_vote < 0 || input_vote >= num_candidats) {
                    printf("Ce num n'est pas disponible. Veuillez choisir un num valide.\n");
                }
            } while (input_vote < 0 || input_vote >= num_candidats);
            
            // Mettre à jour les votes du candidat choisi
            candidats[input_vote].votes++;
            
            personnes[index].deja_voter = 1;  // Marquer la personne comme ayant voté
            i++;  // Incrémenter i seulement si num_existe ne renvoie pas -1 et que la personne n'a pas déjà voté
        } else {

            if (index != -1) {
                if (personnes[index].role == administrateur) {
                    printf("Vous etes un administrateur. Entrez 0 pour arreter les elections: ");
                    scanf("%d", &input_vote);
                    if (input_vote == 0) {
                        printf("Les elections ont ete arretees par un administrateur.\n");
                        exit(0);
                    } else {
                        printf("Le code n est pas valide. Les elections continuent.\n");
                    }
                } else {
                    printf("Vous avez deja vote. Chaque etudiant ne peut voter qu une seule fois.\n");
                }
            } else {
                printf("Votre carte d identite n est pas enregistree, veuillez verifier vos informations.\n");
            }
            
        }
    }


    for (int i = 0; i < num_candidats; i++) {
        printf("Candidat num %d, votes: %d\n", i, candidats[i].votes);
    }

    int max_votes = 0;
    int winners[num_candidats];
    int num_winners = 0;

    for (int i = 0; i < num_candidats; i++) {
        if (candidats[i].votes > max_votes) {
            max_votes = candidats[i].votes;
            winners[0] = i;
            num_winners = 1;
        } else if (candidats[i].votes == max_votes) {
            winners[num_winners] = i;
            num_winners++;
        }
    }

    if (num_winners == 1) {
        printf("Le gagnant est : %s\n", candidats[winners[0]].nom);
    } else {
        printf("Il y a une egalite entre les candidats suivants :\n");
        for (int i = 0; i < num_winners; i++) {
            printf("%s\n", candidats[winners[i]].nom);
        }
    }

    return 0;
}

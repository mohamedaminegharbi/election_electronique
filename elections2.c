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


// Définition de la structure de la Pile
typedef struct Pile {
    int sommet;
    unsigned capacite;
    candidature* tableau;
} Pile;

// Fonction pour créer une pile de capacité donnée
Pile* creerPile(unsigned capacite) {
    Pile* pile = (Pile*) malloc(sizeof(Pile));
    pile->capacite = capacite;
    pile->sommet = -1;
    pile->tableau = (candidature*) malloc(pile->capacite * sizeof(candidature));
    return pile;
}

// Fonction pour ajouter un élément à la pile
void empiler(Pile* pile, candidature item) {
    pile->tableau[++pile->sommet] = item;
}

// Définition de la structure de la File
typedef struct File {
    int avant, arriere, taille;
    unsigned capacite;
    personne* tableau;
} File;

// Fonction pour créer une file de capacité donnée
File* creerFile(unsigned capacite) {
    File* file = (File*) malloc(sizeof(File));
    file->capacite = capacite;
    file->avant = file->taille = 0; 
    file->arriere = capacite - 1;
    file->tableau = (personne*) malloc(file->capacite * sizeof(personne));
    return file;
}

// Fonction pour ajouter un élément à la file
void enfiler(File* file, personne item) {
    file->arriere = (file->arriere + 1)%file->capacite;
    file->tableau[file->arriere] = item;
    file->taille = file->taille + 1;
}

//fonction pour verifier si un element existe dans la file
int num_exist_File(File* file, long numero_de_carte) {
    int count = file->taille;
    int i = file->avant;
    while (count--) {
        if (file->tableau[i].numero_de_carte == numero_de_carte) {
            return (i - file->avant + file->capacite) % file->capacite;  
        }
        i = (i + 1) % file->capacite;
    }
    return -1;  
}

//fonction pour verifier si un element existe dans la Pile
int num_exist_Pile(Pile* pile, long numero_de_carte) {
    for (int i = 0; i <= pile->sommet; i++) {
        if (pile->tableau[i].num == numero_de_carte) {
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

    // Création de la file pour les personnes
    File* personnes = creerFile(num_administrateurs + num_electeurs);
   
    // Création de la pile pour les candidats
    Pile* candidats = creerPile(num_electeurs);  

    int input_role;
    char input_nom[50];
    char input_prenom[50];
    long input_numero_de_carte;  
    int input_vote;
    int num_candidats = 0;  
    int index;

    printf("On va saisir les informations des administrateurs.\n");
    for (int i=0; i<num_administrateurs; i++) {
        personne p;
        do {
            printf("Entrez le numero de carte: ");
            scanf("%ld", &input_numero_de_carte);
            while ((getchar()) != '\n');
            index = num_exist_File(personnes, input_numero_de_carte);
            if (index != -1) {
                printf("Ce num d identite existe deja. Veuillez verifier et entrer un num unique.\n");
            }
        } while (num_exist_File(personnes, input_numero_de_carte) != -1);
        
        p.numero_de_carte=input_numero_de_carte;
        
        printf("Entrez nom: ");
        scanf("%s", p.nom);
        while ((getchar()) != '\n');
        
        printf("Entrez prenom: ");
        scanf("%s", p.prenom);
        
        p.role = administrateur;
        p.deja_voter = 0;
        
        enfiler(personnes, p);
        
    }

    printf("On va saisir les informations des etudiants.\n");
    for (int i=0; i<num_electeurs; i++) {
        personne p;
        candidature c;
        
        do {
            printf("Entrez le numero de carte: ");
            scanf("%ld", &input_numero_de_carte);  
            while ((getchar()) != '\n');
            
            int index = num_exist_File(personnes, input_numero_de_carte);
            if (index != -1) {
                if (personnes->tableau[index].role == administrateur) {
                    printf("Ce num d identite appartient deja a un administrateur. Nous saisissons actuellement uniquement les informations des étudiants. Veuillez entrer un nouveau numéro.\n");
                } else {
                    printf("Ce numero d identite appartient deja a un etudiant. Veuillez verifier et entrer un num unique.\n");
                }
            }
        } while (num_exist_File(personnes, input_numero_de_carte) != -1);

        p.numero_de_carte=input_numero_de_carte;
        
        printf("Veuillez saisir votre nom : ");
        scanf("%s", p.nom);
        while ((getchar()) != '\n');
        
        printf("Veuillez saisir votre prenom : ");
        scanf("%s", p.prenom);
        while ((getchar()) != '\n');
        
        printf("Voulez-vous postuler votre candidature ? (0 pour non, 1 pour oui) : ");
        scanf("%d", &input_role);
        
        if (input_role == 1) {
            // Si l'étudiant est un candidat, ajoutez-le à la pile 'candidats'
            c.num = candidats->sommet + 1;
            strcpy(c.nom, p.nom);
            strcpy(c.prenom, p.prenom);
            c.votes = 0;  // Initialisez les votes à 0
            empiler(candidats, c);  // Ajoutez le candidat à la pile

            FILE *fichier1 = fopen("candidats.txt", "w");
            fprintf(fichier1, "%d %s %s %d\n", c.num, c.nom, c.prenom, c.votes);
            fclose(fichier1);
        }
        
        p.role = input_role == 1 ? candidat : electeur;
        p.deja_voter = 0;
        enfiler(personnes, p);  // Ajoutez la personne à la file

        FILE *fichier2 = fopen("electeurs.txt", "r");
        fscanf(fichier2, "%ld %s %s %d\n", &p.numero_de_carte, p.nom, p.prenom, &p.role);
        fclose(fichier2);
    }
    
    
    // Implementer le processus du vote 
    int i=0;
    while (i < num_electeurs) {
        personne p;
        
        printf("Entrez le num de carte: ");
        scanf("%ld", &input_numero_de_carte);
        while ((getchar()) != '\n');
        
        int index = num_exist_File(personnes, input_numero_de_carte);
        if (index != -1) {
            // The card number exists, so set the deja_voter flag to 1
            personnes->tableau[index].deja_voter = 1;
            
            
            // Now check if they're allowed to vote
            if (personnes->tableau[index].role != administrateur) {
                for (int j = 0; j <= candidats->sommet; j++) {
                    printf("%d. %s\n", j, candidats->tableau[j].nom);
                }
            
                do {
                 printf("Entrez le num du candidat pour lequel vous voulez voter: ");
                 scanf("%d", &input_vote);
                 if (input_vote < 0 || input_vote > candidats->sommet) {
                     printf("Ce num n'est pas disponible. Veuillez choisir un num valide.\n");
                    }
                } while (input_vote < 0 || input_vote > candidats->sommet);
            
             // Mettre à jour les votes du candidat choisi
             candidats->tableau[input_vote].votes++;
             i++;
            } else {
              printf("Vous etes un administrateur. Entrez 0 pour arreter les elections: ");
              scanf("%d", &input_vote);
              if (input_vote == 0) {
                 printf("Les elections ont ete arretees par un administrateur.\n");
                 exit(0);
                } else {
                 printf("Le code n est pas valide. Les elections continuent.\n");
                }
            }
        } else {
         printf("Votre carte d identite n est pas enregistree, veuillez verifier vos informations.\n");
        }
    }


    
    
    for (int i = 0; i <= candidats->sommet; i++) {
        printf("Candidat num %d, votes: %d\n", i, candidats->tableau[i].votes);
    }
    
    int max_votes = 0;
    int winners[candidats->sommet + 1];
    int num_winners = 0;
    
    for (int i = 0; i <= candidats->sommet; i++) {
        if (candidats->tableau[i].votes > max_votes) {
            max_votes = candidats->tableau[i].votes;
            winners[0] = i;
            num_winners = 1;
        } else if (candidats->tableau[i].votes == max_votes) {
                winners[num_winners] = i;
                num_winners++;
        }
    }
    
    
    if (num_winners == 1) {
        printf("Le gagnant est : %s\n", candidats->tableau[winners[0]].nom);
        
        FILE *fichier3 = fopen("resultats.txt", "w");
        fprintf(fichier3, "Le gagnant de l'élection est %s %s avec %d votes.\n", candidats->tableau[winners[0]].nom, candidats->tableau[winners[0]].prenom, candidats->tableau[winners[0]].votes);
        fclose(fichier3);

    } else {
        printf("Il y a une egalite entre les candidats \n");
        for (int i = 0; i < num_winners; i++) {
            printf("%s\n", candidats->tableau[winners[i]].nom);

            FILE *fichier3 = fopen("resultats.txt", "w");
            fprintf(fichier3, "Le gagnant de l'élection est %s %s avec %d votes.\n", candidats->tableau[winners[i]].nom, candidats->tableau[winners[i]].prenom, candidats->tableau[winners[i]].votes);
            fclose(fichier3);
        }
    }

    return 0;
}


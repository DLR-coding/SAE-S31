#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include "serveur.h"

// Opération 1 : Tester
int operation_tester(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            return 0; // Succès
        }
    }
    return -1; // Échec
}

// Opération 2 : Afficher la liste des jeux
int operation_afficher(DemandeOperation *demande) {
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] != '\0') {
            printf("%s\n", jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    return nombre_de_jeux; // Nombre de jeux téléchargés
}

// Opération 3 : Ajouter
int operation_ajouter(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] == '\0') {
            strcpy(jeux[i].NomJeu, demande->NomJeu);
            jeux[i].Code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                jeux[i].Code[j] = '*';
            }
            return 1000; // Taille du jeu téléchargé en Kio
        }
    }
    return -1; // Échec
}

// Opération 4 : Supprimer
int operation_supprimer(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            return 0; // Succès
        }
    }
    return -1; // Échec
}

// Opération 5 : Simuler
void operation_simuler(DemandeOperation *demande) {
    // Pas de retour nécessaire
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            int gagnant = rand() % 2;
            if (gagnant == 0) {
                printf("Joueur 1 gagne !\n");
            } else {
                printf("Joueur 2 gagne !\n");
            }
            free(code);
            return;
        }
    }
}

// Opération 6 : Lancer
void operation_lancer(DemandeOperation *demande) {
    // Pas de retour nécessaire
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            printf("Appuyez sur une touche pour jouer...\n");
            getchar();
            int gagnant = rand() % 2;
            if (gagnant == 0) {
                printf("Serveur gagne !\n");
            } else {
                printf("Joueur gagne !\n");
            }
            free(code);
            return;
        }
    }
}

// Fonction pour exécuter une demande d'opération
int execute_demande(DemandeOperation OP) {
    switch (OP.CodeOp) {
        case 1:
            return operation_tester(&OP);
        case 2:
            return operation_afficher(&OP);
        case 3:
            return operation_ajouter(&OP);
        case 4:
            return operation_supprimer(&OP);
        case 5:
            operation_simuler(&OP);
            break;
        case 6:
            operation_lancer(&OP);
            break;
        default:
            printf("Opération inconnue\n");
            return -1;
    }
}



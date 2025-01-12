#include "serveur.h"

Jeu jeux[MAX_JEUX];

int operation_tester(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            return 0; // Succès
        }
    }
    return -1; // Échec
}

int operation_afficher(DemandeOperation *demande) {
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] != '\0') {
            printf("%s\n", jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    sleep(1);
    return nombre_de_jeux; // Nombre de jeux téléchargés
}

int operation_ajouter(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] == '\0') {
            strcpy(jeux[i].NomJeu, demande->NomJeu);
            jeux[i].Code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                jeux[i].Code[j] = '*';
            }
            sleep(10);
            return 1000; // Taille de jeu (c'est juste un nombre au pif)
        }
    }
    return -1; // Échec
}

int operation_supprimer(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            sleep(2);
            return 0; // Succès
        }
    }
    return -1; // Échec
}

void operation_simuler(DemandeOperation *demande) {
    // Pas de retour nécessaire
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            int gagnant = rand() % 2; sleep(20);
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

void operation_lancer(DemandeOperation *demande) {
    // Pas de retour nécessaire
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            printf("Appuyez sur une touche pour jouer...\n");
            getchar(); // attend le Enter du standardInput
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

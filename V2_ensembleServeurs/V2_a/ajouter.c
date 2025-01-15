#include "serveur.h"

int operation_ajouter(Jeu *jeux, DemandeOperation *demande) {
    printf("Adding game '%s'...\n", demande->NomJeu);
    char *code = malloc(1000 * sizeof(char));
    if (code == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int j = 0; j < 1000; j++) {
        code[j] = '*';
    }
    sleep(1);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] == '\0') {
            strcpy(jeux[i].NomJeu, demande->NomJeu);
            jeux[i].Code = code;
            printf("Game '%s' added.\n", demande->NomJeu);
            return 0;
        }
    }
    printf("Game '%s' could not be added.\n", demande->NomJeu);
    return -1; // Tableau plein
}

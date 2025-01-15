#include "serveur.h"

int operation_afficher(Jeu *jeux) {
    printf("Listing all games...\n");
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] != '\0') {
            printf("Game %d: %s\n", i, jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    return nombre_de_jeux;
}

#include "serveur.h"

void operation_simuler(Jeu *jeux, DemandeOperation *demande, int *gagnant) {
    printf("Simulating game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            *gagnant = rand() % 2;
            sleep(20);
            return;
        }
    }
    printf("Game '%s' not found for simulation.\n", demande->NomJeu);
}

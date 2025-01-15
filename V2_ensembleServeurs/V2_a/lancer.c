#include "serveur.h"

void operation_lancer(Jeu *jeux, DemandeOperation *demande) {
    printf("Launching game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            printf("Press ENTER to play...\n");
            getchar(); // attend le Enter du standardInput
            int gagnant = rand() % 2;
            if (gagnant == 0) {
                printf("Serveur gagne !\n");
            } else {
                printf("Joueur gagne !\n");
            }
            return;
        }
    }
    printf("Game '%s' not found for launch.\n", demande->NomJeu);
}

#include "serveur.h"

int operation_tester(Jeu *jeux, DemandeOperation *demande) {
    printf("Testing if game '%s' exists...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            printf("Game '%s' found.\n", demande->NomJeu);
            return 0; // Jeu trouvé
        }
    }
    printf("Game '%s' not found.\n", demande->NomJeu);
    return -1; // Jeu non trouvé
}

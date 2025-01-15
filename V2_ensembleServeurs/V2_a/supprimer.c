#include "serveur.h"

int operation_supprimer(Jeu *jeux, DemandeOperation *demande) {
    printf("Removing game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            printf("Game '%s' removed.\n", demande->NomJeu);
            return 0;
        }
    }
    printf("Game '%s' not found for removal.\n", demande->NomJeu);
    return -1; // Jeu non trouvé
}

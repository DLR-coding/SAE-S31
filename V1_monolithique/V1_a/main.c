#include "serveur.h"

int main() {
    pid_t PIDf[MAX_CHILDREN] = {0};
    int nbfils_non_bloquants = 0;

    // Déclaration des demandes
    DemandeOperation DeO1 = {1, "Echec", "", 1}; // Tester si le jeu "Echec" est présent (bloquant)
    DemandeOperation DeO2 = {3, "Go", "http://goetmat.com", 1}; // Ajouter le jeu "Go" (bloquant)
    DemandeOperation DeO3 = {3, "Othello", "http://oterleau.com", 1}; // Ajouter le jeu "Othello" (bloquant)
    DemandeOperation DeO4 = {5, "Echec", NULL, 0}; // Simuler un combat avec le jeu "Echec" (non bloquant)
    DemandeOperation DeO5 = {2, "Go", NULL, 0}; // Afficher la liste des jeux (non bloquant)
    DemandeOperation DeO6 = {6, "Othello", NULL, 0}; // Lancer le jeu "Othello" (non bloquant)

    // Exécution des opérations
    execute_demande_parallel(DeO1, PIDf, &nbfils_non_bloquants);
    execute_demande_parallel(DeO2, PIDf, &nbfils_non_bloquants);
    execute_demande_parallel(DeO3, PIDf, &nbfils_non_bloquants);
    execute_demande_parallel(DeO4, PIDf, &nbfils_non_bloquants);
    execute_demande_parallel(DeO5, PIDf, &nbfils_non_bloquants);
    execute_demande_parallel(DeO6, PIDf, &nbfils_non_bloquants);

    // Attendre la fin de tous les processus non bloquants
    wait_for_children(PIDf, nbfils_non_bloquants);

    return 0;
}

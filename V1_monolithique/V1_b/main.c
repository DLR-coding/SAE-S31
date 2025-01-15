#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "serveur.h"

int main() {
    pthread_t PIDf[MAX_CHILDREN];
    int nbfils_non_bloquants = 0;

    // Déclaration des demandes
    DemandeOperation DeO1 = {1, "Echec", "", 1}; // Tester si le jeu "Echec" est présent (bloquant)
    DemandeOperation DeO2 = {3, "Go", "http://goetmat.com", 1}; // Ajouter le jeu "Go" (bloquant)
    DemandeOperation DeO3 = {3, "Othello", "http://oterleau.com", 1}; // Ajouter le jeu "Othello" (bloquant)
    DemandeOperation DeO4 = {5, "Echec", "", 0}; // Simuler un combat avec le jeu "Echec" (non bloquant)
    DemandeOperation DeO5 = {2, "", "", 0}; // Afficher la liste des jeux (non bloquant)
    DemandeOperation DeO6 = {6, "Othello", "", 0}; // Lancer le jeu "Othello" (non bloquant)

    printf("V1_b\n");
    // Exécution des opérations bloquantes
    printf("EXECUTING BLOCKING OPERATION: Test if game 'Echec' exists (PID %d)\n", getpid());
    execute_demande_parallel(DeO1, PIDf, &nbfils_non_bloquants);
    printf("EXECUTING BLOCKING OPERATION: Add game 'Go' (PID %d)\n", getpid());
    execute_demande_parallel(DeO2, PIDf, &nbfils_non_bloquants);
    printf("EXECUTING BLOCKING OPERATION: Add game 'Othello' (PID %d)\n", getpid());
    execute_demande_parallel(DeO3, PIDf, &nbfils_non_bloquants);

    // Exécution des opérations non bloquantes
    printf("EXECUTING NON-BLOCKING OPERATION: Simulate a fight with game 'Echec' (PID %d)\n", getpid());
    execute_demande_parallel(DeO4, PIDf, &nbfils_non_bloquants);
    printf("EXECUTING NON-BLOCKING OPERATION: Display the list of games (PID %d)\n", getpid());
    execute_demande_parallel(DeO5, PIDf, &nbfils_non_bloquants);
    printf("EXECUTING NON-BLOCKING OPERATION: Launch game 'Othello' (PID %d)\n", getpid());
    execute_demande_parallel(DeO6, PIDf, &nbfils_non_bloquants);

    // Nettoyer les threads non bloquants
    clean_threads(PIDf, &nbfils_non_bloquants);

    return 0;
}
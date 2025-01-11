#include "serveur.h"

int main() {
    pid_t PIDf[100] = {0};
    int nbfils_non_bloquants = 0;

    // Déclaration des demandes
    DemandeOperation DeO1 = {1, "Echec", "", 1}; // Tester si le jeu "Echec" est présent (bloquant)
    DemandeOperation DeO2 = {3, "Go", "http://goetmat.com", 1}; // Ajouter le jeu "Go" (bloquant)
    DemandeOperation DeO3 = {3, "Othello", "http://oterleau.com", 1}; // Ajouter le jeu "Othello" (bloquant)
    DemandeOperation DeO4 = {5, "Echec", "", 0}; // Simuler un combat avec le jeu "Echec" (non bloquant)
    DemandeOperation DeO5 = {2, "", "", 1}; // Afficher la liste des jeux (non bloquant)
    DemandeOperation DeO6 = {6, "Othello", "", 0}; // Lancer le jeu "Othello" (non bloquant)
    
    printf("V1_a\n");
    // Exécution des opérations bloquantes
    printf("EXECUTING BLOCKING OPERATION: Test if game 'Echec' exists (PID %d)\n", getpid());
    execute_demande_parallel(DeO1, PIDf, &nbfils_non_bloquants); // PID 1

    printf("EXECUTING BLOCKING OPERATION: Add game 'Go' (PID %d)\n", getpid());
    execute_demande_parallel(DeO2, PIDf, &nbfils_non_bloquants); // PID 2

    printf("EXECUTING BLOCKING OPERATION: Add game 'Othello' (PID %d)\n", getpid());
    execute_demande_parallel(DeO3, PIDf, &nbfils_non_bloquants); // PID 3

    printf("EXECUTING BLOCKING OPERATION: List all games (PID %d)\n", getpid());
    execute_demande_parallel(DeO5, PIDf, &nbfils_non_bloquants); // PID 5

    printf("--------------------------------------------------------------------\n");
    
    // Exécution des opérations non bloquantes
    printf("EXECUTING BLOCKING OPERATION: Simulate game 'Echec' (PID %d)\n", getpid());
    execute_demande_parallel(DeO4, PIDf, &nbfils_non_bloquants); // PID 4

    
    printf("EXECUTING NON-BLOCKING OPERATION: Launch game 'Othello' (PID %d)\n", getpid());
    execute_demande_parallel(DeO6, PIDf, &nbfils_non_bloquants); // PID 6

    // Attendre la fin de toutes les opérations non bloquantes
    wait_for_non_blocking_operations(PIDf, nbfils_non_bloquants);
    
    return 0;
}

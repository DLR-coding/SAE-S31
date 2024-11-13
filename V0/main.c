#include <stdio.h>
#include "serveur.h"

int main() {

    // Déclaration des demandes
    DemandeOperation DeO1 = {1, "Echec", "", 0}; // Tester si le jeu "Echec" est présent
    DemandeOperation DeO2 = {2, "", "", 0}; // Afficher la liste des jeux
    DemandeOperation DeO3 = {3, "Go", "http://goetmat.com", 0}; // Ajouter le jeu "Go"
    DemandeOperation DeO4 = {4, "Echec", "", 0}; // Supprimer le jeu "Echec"
    DemandeOperation DeO5 = {5, "Go", "", 0}; // Simuler un combat avec le jeu "Go"
    DemandeOperation DeO6 = {6, "Go", "", 0}; // Lancer le jeu "Go"

    // Exécution des opérations
    int res1 = execute_demande(DeO1);
    int res2 = execute_demande(DeO2);
    int res3 = execute_demande(DeO3);
    int res4 = execute_demande(DeO4);
    execute_demande(DeO5); // Pas de retour pour cette opération
    execute_demande(DeO6); // Pas de retour pour cette opération

    // Affichage des résultats
    printf("Résultat 1 : %d\n", res1);
    printf("Résultat 2 : %d\n", res2);
    printf("Résultat 3 : %d\n", res3);
    printf("Résultat 4 : %d\n", res4);

}


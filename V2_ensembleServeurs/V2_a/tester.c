#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <game_name>\n", argv[0]);
        return 1;
    }

    printf("Nous sommes dans tester.c , on admet qu'on a une Database de jeu partagée ayant 'Echec' dedans\n");
    char *game_name = argv[1];
    printf("Checking if the game '%s' exists in the database...\n", game_name);

    // Simuler la vérification (ici on suppose que "Echec" existe dans la base)
    if (strcmp(game_name, "Echec") == 0) {
        printf("Game '%s' found!\n", game_name);
        return 0;  // Code de succès
    } else {
        printf("Game '%s' not found.\n", game_name);
        return 1;  // Code d'échec
    }
}

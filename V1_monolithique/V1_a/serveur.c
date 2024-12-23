#include "serveur.h"

Jeu jeux[MAX_JEUX];

/**
 * Vérifie si un jeu avec le nom spécifié dans la demande existe dans la liste des jeux.
 *
 * @param demande Un pointeur vers une structure DemandeOperation contenant le nom du jeu à tester.
 * @return 0 si le jeu est trouvé, -1 sinon.
 */
int operation_tester(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            return 0; // Succès
        }
    }
    return -1; // Échec
}

int operation_afficher(DemandeOperation *demande) {
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] != '\0') {
            printf("%s\n", jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    sleep(1);
    return nombre_de_jeux; // Nombre de jeux téléchargés
}

int operation_ajouter(DemandeOperation *demande, int pipe_fd[2]) {
    close(pipe_fd[PIPE_READ]); // Ferme l'extrémité de lecture du pipe dans le processus fils
    for (int i = 0; i < MAX_JEUX; i++) {
        if (jeux[i].NomJeu[0] == '\0') {
            strcpy(jeux[i].NomJeu, demande->NomJeu);
            jeux[i].Code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                jeux[i].Code[j] = '*';
            }
            sleep(10);
            write(pipe_fd[PIPE_WRITE], &i, sizeof(int)); // Envoie l'index du jeu ajouté au père
            write(pipe_fd[PIPE_WRITE], jeux[i].Code, 1000 * sizeof(char)); // Envoie le code du jeu au père
            close(pipe_fd[PIPE_WRITE]); // Ferme l'extrémité d'écriture du pipe
            return 1000; // Taille de jeu (c'est juste un nombre au pif)
        }
    }
    return -1; // Échec
}

int operation_supprimer(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            sleep(2);
            return 0; // Succès
        }
    }
    return -1; // Échec
}

void operation_simuler(DemandeOperation *demande, int pipe_fd[2]) {
    close(pipe_fd[PIPE_READ]); // Ferme l'extrémité de lecture du pipe dans le processus fils
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            int gagnant = rand() % 2;
            sleep(20);
            char result[10];
            snprintf(result, sizeof(result), "J%d", gagnant + 1);
            write(pipe_fd[PIPE_WRITE], result, sizeof(result)); // Envoie le résultat au père
            close(pipe_fd[PIPE_WRITE]); // Ferme l'extrémité d'écriture du pipe
            free(code);
            return;
        }
    }
}

void operation_lancer(DemandeOperation *demande) {
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            printf("Appuyez sur une touche pour jouer...\n");
            getchar(); // attend le Enter du standardInput
            int gagnant = rand() % 2;
            if (gagnant == 0) {
                printf("Serveur gagne !\n");
            } else {
                printf("Joueur gagne !\n");
            }
            free(code);
            return;
        }
    }
}

int execute_demande(DemandeOperation OP) {
    switch (OP.CodeOp) {
        case 1:
            return operation_tester(&OP);
        case 2:
            return operation_afficher(&OP);
        case 3:
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(1);
            }
            return operation_ajouter(&OP, pipe_fd);
        case 4:
            return operation_supprimer(&OP);
        case 5:
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(1);
            }
            operation_simuler(&OP, pipe_fd);
            break;
        case 6:
            operation_lancer(&OP);
            break;
        default:
            printf("Opération inconnue\n");
            return -1;
    }
    return 0;
}

void execute_demande_parallel(DemandeOperation OP, pid_t *PIDf, int *nbfils_non_bloquants) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (OP.CodeOp == 3 || OP.CodeOp == 5) {
            int pipe_fd[2];
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                exit(1);
            }
            execute_demande(OP);
            close(pipe_fd[PIPE_WRITE]); // Ferme l'extrémité d'écriture du pipe dans le processus père
            if (OP.CodeOp == 3) {
                int index;
                char code[1000];
                read(pipe_fd[PIPE_READ], &index, sizeof(int)); // Lit l'index du jeu ajouté
                read(pipe_fd[PIPE_READ], code, 1000 * sizeof(char)); // Lit le code du jeu
                jeux[index].Code = malloc(1000 * sizeof(char));
                memcpy(jeux[index].Code, code, 1000 * sizeof(char));
            } else if (OP.CodeOp == 5) {
                char result[10];
                read(pipe_fd[PIPE_READ], result, sizeof(result)); // Lit le résultat du combat
                printf("Gagnant : %s\n", result);
            }
            close(pipe_fd[PIPE_READ]); // Ferme l'extrémité de lecture du pipe
        } else {
            execute_demande(OP);
        }
        if (OP.flag == 0) {
            (*nbfils_non_bloquants)++;
        }
        exit(0);
    } else if (pid > 0) {
        // Parent process
        if (OP.flag == 1) {
            waitpid(pid, NULL, 0); // Attend la fin du processus fils --> bloqué
        } else {
            PIDf[*nbfils_non_bloquants] = pid;
            (*nbfils_non_bloquants)++;
        }
    } else {
        perror("fork");
        exit(1);
    }
}

void wait_for_children(pid_t *PIDf, int nbfils_non_bloquants) {
    int status;
    pid_t kidpid;
    while (nbfils_non_bloquants > 0) {
        kidpid = waitpid(-1, &status, WNOHANG);
        if (kidpid > 0) {
            for (int i = 0; i < nbfils_non_bloquants; i++) {
                if (PIDf[i] == kidpid) {
                    PIDf[i] = 0; // Marque le processus comme terminé
                    nbfils_non_bloquants--;
                    break;
                }
            }
        } else {
            break;
        }
    }
}

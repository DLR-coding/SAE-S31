#include "serveur.h"

Jeu jeux[MAX_JEUX];

int operation_tester(DemandeOperation *demande) {
    printf("Testing if game '%s' exists...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            printf("Game '%s' found.\n", demande->NomJeu);
            return 0; // Succès
        }
    }
    printf("Game '%s' not found.\n", demande->NomJeu);
    return -1; // Échec
}

int operation_afficher() {
    printf("Listing all games...\n");
    int nombre_de_jeux = 0;
    for (int i = 0; i < MAX_JEUX; i++) {
        printf("Game %d: %s => incremente nombre_de_jeux\n", i, jeux[i].NomJeu);
        if (jeux[i].NomJeu[0] != '\0') {
            printf("%s\n", jeux[i].NomJeu);
            nombre_de_jeux++;
        }
    }
    sleep(1);
    printf("Total games listed: %d\n", nombre_de_jeux);
    return nombre_de_jeux; // Nombre de jeux téléchargés
}

int operation_ajouter(DemandeOperation *demande, int pipefd) {
    printf("Adding game '%s'...\n", demande->NomJeu);
    char *code = malloc(1000 * sizeof(char));
    for (int j = 0; j < 1000; j++) {
        code[j] = '*';
    }
    sleep(10);
    write(pipefd, code, 1000 * sizeof(char));
    free(code);
    printf("Game '%s' added.\n", demande->NomJeu);
    return 1000; // Taille de jeu (c'est juste un nombre au pif)
}

int operation_supprimer(DemandeOperation *demande) {
    printf("Removing game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            free(jeux[i].Code);
            jeux[i].NomJeu[0] = '\0';
            sleep(2);
            printf("Game '%s' removed.\n", demande->NomJeu);
            return 0; // Succès
        }
    }
    printf("Game '%s' not found for removal.\n", demande->NomJeu);
    return -1; // Échec
}

void operation_simuler(DemandeOperation *demande, int pipefd) {
    printf("Simulating game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            int gagnant = rand() % 2; sleep(20);
            write(pipefd, &gagnant, sizeof(int));
            free(code);
            return;
        }
    }
}

void operation_lancer(DemandeOperation *demande) {
    printf("Launching game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            printf("Press a ENTER to play...\n");
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
    printf("Executing request with CodeOp: %d\n", OP.CodeOp);

    if (OP.CodeOp == 1) {
        return operation_tester(&OP);
    }

    if (OP.CodeOp == 2) {
        return operation_afficher(&OP);
    }

    if (OP.CodeOp == 3) {
        int pipefd[2];
        pipe(pipefd);
        int result = operation_ajouter(&OP, pipefd[PIPE_WRITE]);
        close(pipefd[PIPE_WRITE]);
        char *code = malloc(1000 * sizeof(char));
        read(pipefd[PIPE_READ], code, 1000 * sizeof(char));
        close(pipefd[PIPE_READ]);
        for (int i = 0; i < MAX_JEUX; i++) {
            if (jeux[i].NomJeu[0] == '\0') {
                strcpy(jeux[i].NomJeu, OP.NomJeu);
                jeux[i].Code = code;
                break;
            }
        }
        return result;
    }

    if (OP.CodeOp == 4) {
        return operation_supprimer(&OP);
    }

    if (OP.CodeOp == 5) {
        int pipefd2[2];
        pipe(pipefd2);
        operation_simuler(&OP, pipefd2[PIPE_WRITE]);
        close(pipefd2[PIPE_WRITE]);
        int gagnant;
        read(pipefd2[PIPE_READ], &gagnant, sizeof(int));
        close(pipefd2[PIPE_READ]);
        if (gagnant == 0) {
            printf("Joueur 1 gagne !\n");
        } else {
            printf("Joueur 2 gagne !\n");
        }
    }

    if (OP.CodeOp == 6) {
        operation_lancer(&OP);
    }

    printf("Unknown operation\n");
    return -1;
}

void execute_demande_parallel(DemandeOperation OP, pid_t *PIDf, int *nbfils_non_bloquants) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("Child process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
        if (OP.CodeOp == 3 || OP.CodeOp == 5) {
            int pipefd[2];
            pipe(pipefd);
            if (OP.CodeOp == 3) {
                operation_ajouter(&OP, pipefd[PIPE_WRITE]);
            } else if (OP.CodeOp == 5) {
                operation_simuler(&OP, pipefd[PIPE_WRITE]);
            }
            close(pipefd[PIPE_WRITE]);
            if (OP.flag == 1) {
                close(pipefd[PIPE_READ]);
            } else {
                if (OP.CodeOp == 5) {
                    int gagnant;
                    read(pipefd[PIPE_READ], &gagnant, sizeof(int));
                    if (gagnant == 0) {
                        printf("Joueur 1 gagne !\n");
                    } else {
                        printf("Joueur 2 gagne !\n");
                    }
                }
                close(pipefd[PIPE_READ]);
            }
        } else {
            execute_demande(OP);
        }
        printf("EXIT Child process (PID %d) who did request with CodeOp: %d\n", getpid(), OP.CodeOp);
        exit(0);
    } else if (pid > 0) {
        // Parent process
        printf("Parent process (PID %d) forked child process (PID %d)\n", getpid(), pid);
        if (OP.flag == 1) {
            waitpid(pid, NULL, 0);
        } else {
            PIDf[*nbfils_non_bloquants] = pid;
            (*nbfils_non_bloquants)++;
        }
    } else {
        perror("fork");
        exit(1);
    }
}

void wait_for_non_blocking_operations(pid_t *PIDf, int nbfils_non_bloquants) {
    int status;
    pid_t kidpid;
    printf("Parent process (PID %d) waiting for non-blocking operations to complete...\n", getpid());
    do {
        kidpid = waitpid(-1, &status, WNOHANG);
        if (kidpid > 0) {
            printf("Child process (PID %d) completed.\n", kidpid);
            for (int i = 0; i < nbfils_non_bloquants; i++) {
                if (PIDf[i] == kidpid) {
                    PIDf[i] = 0;
                    break;
                }
            }
            nbfils_non_bloquants--;
        }
    } while (nbfils_non_bloquants > 0);
    printf("All non-blocking operations completed.\n");
}

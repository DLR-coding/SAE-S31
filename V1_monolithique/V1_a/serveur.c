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
        if (jeux[i].NomJeu[0] != '\0') {
            printf("Game %d: %s => incremente nombre_de_jeux\n", i, jeux[i].NomJeu);
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

void operation_simuler(DemandeOperation *demande, int pipefd_write) {
    printf("Simulating game '%s'...\n", demande->NomJeu);
    for (int i = 0; i < MAX_JEUX; i++) {
        if (strcmp(jeux[i].NomJeu, demande->NomJeu) == 0) {
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            int gagnant = rand() % 2; sleep(20);
            write(pipefd_write, &gagnant, sizeof(int));
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

void execute_demande_parallel(DemandeOperation OP, pid_t *PIDf, int *nbfils_non_bloquants) {
    pid_t pid;
    

    if (OP.CodeOp == 1 || OP.CodeOp == 2) {
        // For Tester and Lister, we can delegate to child process
        pid = fork();
        if (pid == 0) {
            // Child process
            printf("Child process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
            if (OP.CodeOp == 1) {
                operation_tester(&OP);
            } else if (OP.CodeOp == 2) {
                operation_afficher();
            }
            exit(0);
        } else {
            // Parent process
            printf("Parent process (PID %d) forked child process (PID %d)\n", getpid(), pid);
            if (OP.flag == 1) {
                printf("BLOQUANT , parent (PID %d) attend fin de l'enfant (PID %d)\n", getpid(), pid);
                waitpid(pid, NULL, 0);
            } else {
                printf("NON BLOQUANT , parent (PID %d) continue son execution\n", getpid());

                PIDf[*nbfils_non_bloquants] = pid;
                (*nbfils_non_bloquants)++;
            }
        }
    } else if (OP.CodeOp == 3) {
        // For Ajouter, we need to simulate loading and transfer via pipe
        int pipefd[2];
        pipe(pipefd);
        pid = fork();
        if (pid == 0) {
            // Child process
            printf("Child process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
            close(pipefd[PIPE_READ]); // Close read end in child
            operation_ajouter(&OP, pipefd[PIPE_WRITE]);
            close(pipefd[PIPE_WRITE]); // Close write end in child
            exit(0);
        } else {
            // Parent process
            printf("Parent process (PID %d) forked child process (PID %d)\n", getpid(), pid);
            close(pipefd[PIPE_WRITE]); // Close write end in parent
            printf("BLOQUANT , parent (PID %d) attend fin de l'enfant (PID %d)\n", getpid(), pid);
            waitpid(pid, NULL, 0); // Wait for child process to finish
            char *code = malloc(1000 * sizeof(char));
            read(pipefd[PIPE_READ], code, 1000 * sizeof(char));
            close(pipefd[PIPE_READ]); // Close read end in parent
            printf("adding Game '%s' in parent process...\n", OP.NomJeu);
            for (int i = 0; i < MAX_JEUX; i++) {
                printf("jeux[%d].NomJeu: %s\n", i, jeux[i].NomJeu);
                if (jeux[i].NomJeu[0] == '\0') {
                    strcpy(jeux[i].NomJeu, OP.NomJeu);
                    jeux[i].Code = code;
                    printf("added Game '%s' in parent process...\n", jeux[i].NomJeu);
                    break;
                }
            }
            free(code);
        }
    } else if (OP.CodeOp == 4) {
        // For Supprimer, must be done by parent process
        printf("Parent process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
        printf("BLOQUANT , parent (PID %d)\n", getpid());    
        operation_supprimer(&OP);
    } else if (OP.CodeOp == 5) {
        // For Combat, parent transmits code to child via pipe
        int pipe_parent_to_child[2]; // Pipe 1 : Parent écrit, enfant lit
        int pipe_child_to_parent[2]; // Pipe 2 : Enfant écrit, parent lit

        pipe(pipe_parent_to_child) ;
        pipe(pipe_child_to_parent) ;
        pid = fork();
        if (pid == 0) {
            // --- Processus enfant ---
            close(pipe_parent_to_child[PIPE_WRITE]); // Fermer l'extrémité d'écriture du pipe parent -> enfant
            close(pipe_child_to_parent[PIPE_READ]); // Fermer l'extrémité de lecture du pipe enfant -> parent

            printf("Child process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
            printf(" '%s' code received from parent process , loading it...\n", OP.NomJeu);
            char *code = malloc(1000 * sizeof(char));
            read(pipe_parent_to_child[PIPE_READ], code, 1000 * sizeof(char));
            printf(" '%s' code to loaded in child process...\n", OP.NomJeu);

            operation_simuler(&OP, pipe_child_to_parent[PIPE_WRITE]);
            close(pipe_child_to_parent[PIPE_WRITE]); // Fermer l'extrémité d'écriture du pipe enfant -> parent
            close(pipe_parent_to_child[PIPE_READ]); // Fermer l'extrémité de lecture du pipe parent -> enfant
            exit(0);
        } else {
            // --- Processus parent ---
            close(pipe_parent_to_child[PIPE_READ]); // Fermer l'extrémité de lecture du pipe parent -> enfant
            close(pipe_child_to_parent[PIPE_WRITE]); // Fermer l'extrémité d'écriture du pipe enfant -> parent

            printf("Parent process (PID %d) forked child process (PID %d)\n", getpid(), pid);
            printf("Loading '%s' code to child process...\n", OP.NomJeu);
            char *code = malloc(1000 * sizeof(char));
            for (int j = 0; j < 1000; j++) {
                code[j] = '*';
            }
            write(pipe_parent_to_child[PIPE_WRITE], code, 1000 * sizeof(char));
            free(code);
            printf(" '%s' code sent to child process...\n", OP.NomJeu);

            int gagnant;
            read(pipe_child_to_parent[PIPE_READ], &gagnant, sizeof(int));
            close(pipe_child_to_parent[PIPE_READ]); // Fermer l'extrémité de lecture du pipe enfant -> parent
            close(pipe_parent_to_child[PIPE_WRITE]); // Fermer l'extrémité d'écriture du pipe parent -> enfant

            if (gagnant == 0) {
                printf("Joueur 1 gagne !\n");
            } else {
                printf("Joueur 2 gagne !\n");
            }

            printf("BLOQUANT , parent (PID %d) attend fin de l'enfant (PID %d)\n", getpid(), pid);
            waitpid(pid, NULL, 0); // Attendre la fin de l'enfant
        }
    }
    else if (OP.CodeOp == 6) {
        // For Lancer, can be done by child process
        pid = fork();
        if (pid == 0) {
            // Child process
            printf("Child process (PID %d) executing request with CodeOp: %d\n", getpid(), OP.CodeOp);
            operation_lancer(&OP);
            exit(0);
        } else {
            // Parent process
            printf("Parent process (PID %d) forked child process (PID %d)\n", getpid(), pid);
            if (OP.flag == 1) {
                printf("BLOQUANT , parent (PID %d) attend fin de l'enfant (PID %d)\n", getpid(), pid);            
                waitpid(pid, NULL, 0);
            } else {
                PIDf[*nbfils_non_bloquants] = pid;
                (*nbfils_non_bloquants)++;
            }
        }
    } else {
        printf("Unknown operation\n");
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

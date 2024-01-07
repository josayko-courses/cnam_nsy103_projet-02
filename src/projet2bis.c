// Author: Jonny SAYKOSY
// CNAM 2023-2024

#define _POSIX_SOURCE // needed for kill() function prototype
#include "biblio.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MESSAGE_LENGTH 1024

int child_process()
{
    pid_t pid = getpid();
    char message[MESSAGE_LENGTH];
    memset(&message, 0, sizeof(message));

    printf("%d : %s\n", pid, "Je suis le fils");

    while (1) {
        printf("%d : Veuillez saisir un message: ", pid);
        fgets(message, sizeof(message), stdin);

        // Removes trailing newline
        if ((strlen(message) > 0) && (message[strlen(message) - 1] == '\n'))
            message[strlen(message) - 1] = '\0';

        emission("127.0.0.1", 8000, message);
        kill(pid, SIGSTOP);
    }

    printf("%d : %s\n", pid, "Fin du processus fils...");
    return 0;
}

int main()
{
    pid_t child_pid = fork();
    pid_t pid = getpid();

    if (child_pid < 0) {
        perror("Failed to create process");
        exit(1);
    }

    if (child_pid == 0) {
        child_process();
    } else {
        printf("%d : %s\n", pid, "Je suis le père");

        char buffer[MESSAGE_LENGTH];
        memset(&buffer, 0, sizeof(buffer));
        int ret = 0;
        while ((ret = reception(8000, buffer)) >= 0) {
            printf("%d : Message reçu : %s\n", pid, buffer);
            kill(child_pid, SIGCONT);
        }

        // Wait for child process to finish
        wait(NULL);
        printf("%d : %s\n", pid, "Fin du processus père...");
        return 0;
    }
}

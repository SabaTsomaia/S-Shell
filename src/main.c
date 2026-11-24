#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/wait.h> 


#define BUFSIZE 1024
#define CMDSIZE 64

void evaluate()
{
    char *buffer = malloc(BUFSIZE * sizeof *buffer);
    char **commands = malloc(CMDSIZE * sizeof *commands);

    if (commands == NULL || buffer == NULL)
    {
        fprintf(stderr, "S: allocation Error/Fail\n");
        exit(EXIT_FAILURE);
    }

    // parsing lines and tokenising words together.
    while (1)
    {
        printf("S: ");
        // Immediately write buffered data to the terminal
        fflush(stdout);

        int ch = 0;
        int bufPointer = 0;
        int stPointer = 0;
        int cmdPtr = 0;

        // Parse every new line --- char by char
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            if (ch == ' ')
            {
                // Store word only when the bufPointer is above stPointer
                // make stPointer = bufPointer every time we find a word
                // Skip Everything else.
                if (stPointer < bufPointer)
                {
                    // change ' ' to '\0' and increment counter
                    buffer[bufPointer++] = '\0';
                    // store string from stPointer to changed '\0'
                    commands[cmdPtr++] = &buffer[stPointer];
                    // After a successfull word, 2 pointers are equal.
                    stPointer = bufPointer;
                }
            }
            else
            {
                // store regular characters and increment memory index number (bufPointer)
                buffer[bufPointer++] = ch;
            }
        }

        // Store last word before \n
        if (stPointer < bufPointer)
        {
            buffer[bufPointer] = '\0';
            commands[cmdPtr++] = &buffer[stPointer];
        }

        // Null terminate the array for execvp
        commands[cmdPtr] = NULL;

        if (ch == EOF)
        {
            printf("Bye bye\n");
            break;
        }

        if (cmdPtr == 0)
        {
            continue;
        }




        // handle built-ins like cd and exit.
        if (strcmp(commands[0], "exit") == 0)
        {
            break;
        }

        if(strcmp(commands[0], "cd") == 0) 
        {
            char *target = commands[1];

            // if second argument is empty, go to the HOME directory.
            if(target == NULL) 
            {
                target = getenv("HOME");
            }

            if((chdir(target) < 0)){
                perror("S: cd");
            }

            continue;
        }



        // Execute external commands 
        pid_t pid = fork();

        if(pid < 0)
        {
            perror("S: fork failed");
        } else if(pid == 0)
        {
            // Replace child process with typed program
            execvp(commands[0], commands);

            // Line after execvp never run unless there is an error.
            perror("S ");
            _exit(EXIT_FAILURE);
        } else
        {
            int status;
            if(waitpid(pid,&status,0) < 0)
            {
                perror("S: waitpid failed");
            }
        }
    }
    free(buffer);
    free(commands);
}

int main()
{
    // get line in the buffer
    // identify each command in the buffer using whitespaces

    evaluate();

    return 0;
}
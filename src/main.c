#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1024
#define CMDSIZE 64

void evaluate()
{
    char *buffer = malloc(BUFSIZE * sizeof *buffer);
    char **commands = malloc(CMDSIZE * sizeof *commands);


    if (!commands && !buffer) {
        fprintf(stderr, "S: allocation error\n");
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

        // Log taken commands
        for (int i = 0; i < cmdPtr; i++)
        {
            printf("commands[%d] = \"%s\"\n", i, commands[i]);
        }

        if (ch == EOF)
        {
            printf("Bye bye\n");
            break;
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
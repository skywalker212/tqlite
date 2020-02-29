#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * struct to hold the input and all the jazz that comes with it
 * this is for the repl
 */
typedef struct {
	char* buffer;
	size_t buffer_length;
	ssize_t input_length;
} InputBuffer;

/**
 * since we love OOP
 */
InputBuffer* new_input_buffer() {
	// allocate memory, only allocates for the struct
	InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
	input_buffer -> buffer = NULL;
	input_buffer -> buffer_length = 0;
	input_buffer -> input_length = 0;

	return input_buffer;
}

/**
 * to print the prompt, to let the user know that he/she is in da REPL!!
 */
void print_prompt() {
	printf("tqlite > ");
}

/**
 * to read the input from stdin into the buffer that we have created
 */
void read_input(InputBuffer* input_buffer) {
	// pay attention to the pointer references
	ssize_t bytes_read = getline(&(input_buffer -> buffer), &(input_buffer -> buffer_length), stdin);

	if (bytes_read <= 0) {
		printf("Error reading the input\n");
		exit(EXIT_FAILURE);
	}

	// ignore trailing newline
	input_buffer -> input_length = bytes_read - 1;
	input_buffer -> buffer[bytes_read - 1] = 0;	// null termination
}

/**
 * function to free up input buffer memory cause we are not savages
 */
void close_input_buffer(InputBuffer* input_buffer) {
	free(input_buffer -> buffer);
	free(input_buffer);
}

/**
 * the father of all
 */
int main(int argc, char* argv[]) {
	InputBuffer* input_buffer = new_input_buffer();
	while (true) {
		print_prompt();
		read_input(input_buffer);

		// heart of the REPL
		if(strcmp(input_buffer -> buffer, "exit") == 0) {
			close_input_buffer(input_buffer);
			printf("bye\n");
			exit(EXIT_SUCCESS);
		} else {
			printf("Unrecognized command '%s'\n", input_buffer -> buffer);
		}
	}
}

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ------------------------------------------------------------
 * Struct/enum definitions start
 */

// struct to hold the input and all the jazz that comes with it this is for the repl
typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

// enum to hold the status of meta command execution
typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

//enum to hold the satus of prepared command execution
typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

// enum to hold the type of prepared statement
typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

// struct to hold the prepared statement metadata
typedef struct {
  StatementType type;
} Statement;

/**
 * Struct/enum definitions end
 * ------------------------------------------------------------------
 */

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
  input_buffer -> buffer[bytes_read - 1] = 0; // null termination
}

/**
 * function to free up input buffer memory cause we are not savages
 */
void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer -> buffer);
  free(input_buffer);
}


/**
 * ---------------------------------------------------------------------------------------
 * START functions related to frontend
 */

/**
 * function to handle meta commands
 * Meta commands are commands that start with ".", .exit is a meta command
 */
MetaCommandResult do_meta_command (InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    printf("bye\n");
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

/**
 * function to parse the prepared statement
 */
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  } else if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  } else {
    return PREPARE_UNRECOGNIZED_STATEMENT;
  }
}

/**
 * END functions related to frontend
 * -------------------------------------------------------------------------------------
 */

/**
 * -------------------------------------------------------------------------------------
 * START functions related to backend
 */

void execute_statement(Statement* statement) {
  switch (statement->type) {
    case (STATEMENT_INSERT):
      printf("You asked for an insert operation monsiour/madame\n");
      break;
    case (STATEMENT_SELECT):
      printf("Select statement, eh?\n");
      break;
  }
}

/**
 * END functions related to backend
 * -------------------------------------------------------------------------------------
 */

/**
 * the father of all
 */
int main(int argc, char* argv[]) {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    // heart of the REPL
    if(input_buffer -> buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECOGNIZED_COMMAND):
          printf("Unrecognized command '%s'\n", input_buffer->buffer);
          continue;
      }
    } else {
      Statement statement;
      switch (prepare_statement(input_buffer, &statement)) {
        case (PREPARE_SUCCESS):
          break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
          printf("Unrecognized keyword at start of '%s'\n", input_buffer->buffer);
          continue;
      }
      execute_statement(&statement);
      printf("Executed. \n");
    }
  }
}

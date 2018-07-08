#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

// If we are compiling on Windows compile these functions
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

// fake readline function
char* readline(char* prompt){
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

// fake add_history function
void add_history(char* unused) {}

// otherwise include the editline headers
#else
#include <editline/readline.h>
#endif

int main(int argc, char** argv){
    // Create some parsers
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Decimal = mpc_new("decimal");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                                               \
        number      : /-?[0-9]+/ ;                                                  \
        decimal     : /-?[1-9]+\\.[0-9]+|-?[0]\\.[0-9]+/;                           \
        operator    : '+' | '-' | '*' | '/' | '%'                                   \
                    | \"add\" | \"sub\" | \"mul\" | \"div\" | \"mod\" ;             \
        expr        : <decimal> | <number> | '(' <operator> <expr>+ ')' ;           \
        lispy       : /^/ <operator> <expr>+ /$/ ;                                  \
    ",
    Number, Decimal, Operator, Expr, Lispy);


    // Print Version and Exist Information
    puts("Lispy Version 0.0.0.0.1");
    puts("Author - Matt Peebles, peebles.mm@gmail.com");
    puts("Press Ctrl-c to Exit/n");

    // In a never ending loop
    while(1){
        // Output our prompt and get input
        char* input = readline("lispy> ");

        // Add input to history
        add_history(input);
        

        // Attempt to parse the user input

        mpc_result_t r;
        if(mpc_parse("<stdin>", input, Lispy, &r)){
            // On Sucess print the AST
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        }
        else {
            // otherwise print the error
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        // Free retrieved input
        free(input);
    }

    mpc_cleanup(5, Number, Decimal, Operator, Expr, Lispy);

    return 0;
}

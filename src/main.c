#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ast.h"
#include "parser.h"
#include "eval.h"

int main(int argc, char** argv) {
    int count = 0;
    while (true) {
        char* line = readline("> ");
        add_history(line);
        parser_result_t* node = parse(line);
        nodePP(evaluate(builtins, node->value));
        puts("");
        free(line);
    }
}

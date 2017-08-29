#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ast.h"
#include "parser.h"
#include "eval.h"

int main(void) {
    while (true) {
        char* line = readline("> ");
        add_history(line);
        parser_result_t* node = parse(line);
        nodePP(evaluate(builtins, node->value));
        puts("");
    }
}

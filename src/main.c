#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ast.h"
#include "parser.h"
#include "eval.h"

int main(int argc, char** argv) {
    printf("zeptolisp version: %s\n\n", VERSION);

    context_t* context = defaultEnv();

    while (true) {
        char* line = readline("> ");
        add_history(line);
        if (strcmp(line, "") != 0) {
            parser_result_t* node = parse(line);
            nodePP(evaluate(context, node->value));
            puts("");
            free(line);
        }
    }

    freeContext(context);
}

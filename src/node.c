#include "node.h"
#include "scope.h"
#include <string.h>


struct Node *node_alloc(int type)
{
    struct Node *node = malloc(sizeof(struct Node));
    node->type = type;

    node->compound_nodes = 0;
    node->compound_size = 0;

    node->function_def_body = 0;
    node->function_def_name = 0;
    node->function_def_params = 0;
    node->function_def_params_size = 0;
    node->function_def_return_type = 0;

    node->int_value = 0;

    node->string_value = 0;
    node->string_asm_id = 0;

    node->return_value = 0;

    node->variable_def_value = 0;
    node->variable_def_name = 0;
    node->variable_def_type = 0;
    node->variable_def_stack_offset = 0;

    node->variable_name = 0;

    node->function_call_name = 0;
    node->function_call_args = 0;
    node->function_call_args_size = 0;

    node->param_name = 0;
    node->param_type = 0;
    node->param_stack_offset = 0;

    node->assignment_dst = 0;
    node->assignment_src = 0;

    node->error_line = 0;

    return node;
}


void node_free(struct Node *node)
{
    if (node->compound_nodes)
    {
        for (size_t i = 0; i < node->compound_size; ++i)
            node_free(node->compound_nodes[i]);

        free(node->compound_nodes);
    }

    if (node->function_call_args)
    {
        for (size_t i = 0; i < node->function_call_args_size; ++i)
            node_free(node->function_call_args[i]);

        free(node->function_call_args);
    }

    if (node->function_def_params)
    {
        for (size_t i = 0; i < node->function_def_params_size; ++i)
            node_free(node->function_def_params[i]);

        free(node->function_def_params);
    }

    if (node->function_def_body)
        node_free(node->function_def_body);

    if (node->return_value)
        node_free(node->return_value);

    if (node->variable_def_value)
        node_free(node->variable_def_value);

    if (node->string_asm_id)
        free(node->string_asm_id);

    if (node->assignment_dst)
        node_free(node->assignment_dst);

    if (node->assignment_src)
        node_free(node->assignment_src);

    free(node);
}


struct Node *node_strip_to_literal(struct Node *node, struct Scope *scope)
{
    switch (node->type)
    {
    case NODE_INT:
    case NODE_STRING:
        return node;
    case NODE_VARIABLE:
        return node_strip_to_literal(scope_find_variable(scope, node->variable_name), scope);
    case NODE_VARIABLE_DEF:
        return node_strip_to_literal(node->variable_def_value, scope);
    default: return node;
    }
}


char *node_str_from_type(int type)
{
    switch (type)
    {
    case NODE_INT: return "int";
    case NODE_STRING: return "str";
    default: return 0;
    }
}


int node_type_from_str(char *str)
{
    if (strcmp(str, "int") == 0)
        return NODE_INT;
    if (strcmp(str, "str") == 0)
        return NODE_STRING;

    return -1;
}


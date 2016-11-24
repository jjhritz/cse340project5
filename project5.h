//
// Created by student on 11/24/16.
//

#ifndef CSE340PROJECT5_PROJECT5_H
#define CSE340PROJECT5_PROJECT5_H

#include <vector>

extern "C"
{
    #include "compiler.h"
}

using namespace std;

extern vector<struct ValueNode> vars;

struct StatementNode* parse_program();
void parse_var_decl();
void parse_id_list();
struct StatementNode* parse_body();
bool comp_vars(const ValueNode& a, const ValueNode& b);
struct ValueNode* find_var(char* symbol);

#endif //CSE340PROJECT5_PROJECT5_H

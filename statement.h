//
// Created by student on 11/24/16.
//

#ifndef CSE340PROJECT5_STATEMENT_H
#define CSE340PROJECT5_STATEMENT_H

struct StatementNode* parse_stmt_list();
struct StatementNode* parse_stmt();
struct AssignmentStatement* assign_stmt();
struct PrintStatement* print_stmt();

#endif //CSE340PROJECT5_STATEMENT_H

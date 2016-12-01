//
// Created by student on 12/1/16.
//

#ifndef CSE340PROJECT5_SWITCH_H
#define CSE340PROJECT5_SWITCH_H

struct StatementNode* parse_case_list(struct StatementNode* exit, struct ValueNode* var);
struct StatementNode* parse_case(struct StatementNode* exit, struct ValueNode* var);

#endif //CSE340PROJECT5_SWITCH_H

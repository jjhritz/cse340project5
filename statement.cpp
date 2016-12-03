//
// Created by student on 11/24/16.
//

#include <cstdlib>
#include "project5.h"
#include "statement.h"
#include "if.h"
#include "switch.h"

struct StatementNode* parse_stmt_list()
{
    //create NOOP node as head of list
    //struct StatementNode* head = new struct StatementNode;
    //head->type = NOOP_STMT;
    struct StatementNode* stmt;

    //get first statement
    //get token
    getToken();

    //if type is ID or PRINT or IF or WHILE or SWITCH, it's a statement
    if(ttype == ID || ttype == PRINT || ttype == IF || ttype == WHILE || ttype == SWITCH)
    {
        //store statement token type
        int stmt_ttype = ttype;
        //unget token
        ungetToken();
        //parse statement
        stmt = parse_stmt();

        //get token
        getToken();
        //if type is ID or PRINT or IF or WHILE or SWITCH, it's another statement
        if(ttype == ID || ttype == PRINT || ttype == IF || ttype == WHILE || ttype == SWITCH)
        {
            //unget token
            ungetToken();
            //parse statement
            struct StatementNode* next_stmt = parse_stmt_list();

            //if statement token type is IF or WHILE, there's a trailing NO-OP we need to follow
            if(stmt_ttype == IF || stmt_ttype == WHILE)
            {
                stmt->next->next = next_stmt;
            }
            //else if statement token type is SWITCH, we need to follow the end of the case list
            else if(stmt_ttype == SWITCH)
            {
                //the end of the case list should be NULL,
                //so we need to advance through the statement until we reach a null pointer

                //create pointer at statement
                struct StatementNode* inst_ptr = stmt;
                //while pointer->next is not NULL
                while(inst_ptr->next != NULL)
                {
                    //if type is GOTO, we need to jump to the target
                    if(inst_ptr->type == GOTO_STMT)
                    {
                        //jump to target
                        inst_ptr = inst_ptr->goto_stmt->target;
                    }
                    //else advance normally
                    else
                    {
                        //advance pointer
                        inst_ptr  = inst_ptr->next;
                    }
                }

                //if type is GOTO, we need to jump to the target
                if(inst_ptr->type == GOTO_STMT)
                {
                    //jump to target
                    inst_ptr = inst_ptr->goto_stmt->target;
                }
                //we should be at the end of the default case here
                inst_ptr->next = next_stmt;
            }
            //else, statement list continues normally
            else
            {
                stmt->next = next_stmt;
            }
            //endif

        }
        //else, we've reached the end of the statement list. Let the caller handle what's next
        else
        {
            ungetToken();
        }
        //endif
    }
    //else, no more statements in this list; let the caller handle it
    else
    {
        ungetToken();
    }
    //endif

    return stmt;
}

struct StatementNode* parse_stmt()
{
    //create statement node
    struct StatementNode* stmt = new struct StatementNode;

    //determine the type of statement
    //get token
    getToken();

    //switch on type
    switch(ttype)
    {
        //if type is ID, it's an assignment statement
        case ID:
        {
            //unget token
            ungetToken();

            //parse assign statement
            stmt->assign_stmt = assign_stmt();
            stmt->type = ASSIGN_STMT;
            break;
        }
        //endcase

        //if type is PRINT, it's a print statement
        case PRINT:
        {
            //unget token
            ungetToken();

            //parse print_stmt
            stmt->print_stmt = print_stmt();
            stmt->type = PRINT_STMT;
            break;
        }
        //endcase

        //if type is IF, it's an IF statement
        case IF:
        {
            //unget token
            ungetToken();

            //parse if_stmt
            stmt->if_stmt = parse_if_stmt();
            stmt->type = IF_STMT;

            //create NO-OP trailer
            struct StatementNode* trailer = new struct StatementNode;
            trailer->type = NOOP_STMT;
            stmt->next = trailer;

            //if statement->if_stmt->false_branch is NULL
            if(stmt->if_stmt->false_branch == NULL)
            {
                stmt->if_stmt->false_branch = trailer;

                //add exit point to true branch
                //create index pointer at true branch
                struct StatementNode* inst_ptr = stmt->if_stmt->true_branch;

                //while pointer->next is not NULL
                while(inst_ptr->next != NULL)
                {
                    //advance to next instruction
                    inst_ptr = inst_ptr->next;
                }

                //assign exit point
                inst_ptr->next = trailer;
            }
            //else if statement->if_stmt->true_branch is NULL
            else if(stmt->if_stmt->true_branch == NULL)
            {
                stmt->if_stmt->true_branch = trailer;

                //add exit point to false branch
                //create index pointer at false branch
                struct StatementNode* inst_ptr = stmt->if_stmt->false_branch;

                //while pointer->next is not NULL
                while(inst_ptr->next != NULL)
                {
                    //advance to next instruction
                    inst_ptr = inst_ptr->next;
                }

                //assign exit point
                inst_ptr->next = trailer;
            }
            //else, both branches on the IF are not NULL; this shouldn't happen
            else
            {
                debug("Both branches of an IF statement are already assigned.");
            }
            //endif
            break;
        }
        //endcase

        //if type is WHILE, it's a WHILE statement
        case WHILE:
        {
            //unget token
            ungetToken();

            //parse if_stmt
            stmt->if_stmt = parse_if_stmt();
            stmt->type = IF_STMT;

            //create NO-OP trailer
            struct StatementNode* trailer = new struct StatementNode;
            trailer->type = NOOP_STMT;
            stmt->next = trailer;

            //create loopback GOTO
            struct StatementNode* loopback = new struct StatementNode;
            loopback->type = GOTO_STMT;
            loopback->goto_stmt = new struct GotoStatement;
            loopback->goto_stmt->target = stmt;
            loopback->next = trailer;

            //if statement->if_stmt->false_branch is NULL
            if(stmt->if_stmt->false_branch == NULL)
            {
                stmt->if_stmt->false_branch = trailer;

                //add loopback point to true branch
                //create index pointer at true branch
                struct StatementNode* inst_ptr = stmt->if_stmt->true_branch;

                while(inst_ptr->next != NULL)
                {
                    /*
                    //if type is GOTO, we need to jump to the target
                    if(inst_ptr->type == GOTO_STMT)
                    {
                        //jump to target
                        inst_ptr = inst_ptr->goto_stmt->target;
                    }
                    //else advance normally
                    else
                    {
                        //advance pointer
                        inst_ptr  = inst_ptr->next;
                    }
                     */

                    inst_ptr = inst_ptr->next;
                }

                //if type is GOTO, we need to jump to the target
                /*if(inst_ptr->type == GOTO_STMT)
                {
                    //jump to target
                    inst_ptr = inst_ptr->goto_stmt->target;
                }
                 */

                inst_ptr->next = loopback;
            }
            //else if statement->if_stmt->true_branch is NULL
            else if(stmt->if_stmt->true_branch == NULL)
            {
                stmt->if_stmt->true_branch = trailer;

                //add loopback point to false branch
                //create index pointer at false branch
                struct StatementNode* inst_ptr = stmt->if_stmt->false_branch;

                //while pointer->next is not NULL
                while(inst_ptr->next != NULL)
                {
                    //advance to next instruction
                    inst_ptr = inst_ptr->next;
                }

                //assign loopback point
                inst_ptr->next = loopback;
            }
            //else, both branches on the IF are not NULL; this shouldn't happen
            else
            {
                debug("Both branches of a WHILE statement are already assigned.");
            }
            //endif
            break;
        }
        //endcase

        //if type is SWITCH, it's a switch statement
        case SWITCH:
        {
            //get token
            getToken();
            //if type is ID
            if(ttype == ID)
            {
                //find and store variable
                struct ValueNode* switch_var = find_var(token);

                //get token
                getToken();
                //if type is LBRACE
                if(ttype == LBRACE)
                {
                    //create GO-TO exit
                    struct StatementNode* exit = new struct StatementNode;
                    exit->type = GOTO_STMT;
                    exit->goto_stmt = new struct GotoStatement;

                    //create NO-OP trailer
                    exit->goto_stmt->target = new struct StatementNode;
                    exit->goto_stmt->target->type = NOOP_STMT;
                    exit->next = exit->goto_stmt->target;

                    //parse case lise
                    struct StatementNode* case_list = parse_case_list(exit, switch_var);

                    //get token
                    getToken();

                    //if token is RBRACE, it's the end of the switch statement
                    if(ttype == RBRACE)
                    {
                        stmt = case_list;
                    }
                    //else, no closing RBRACE
                    else
                    {
                        debug("no closing RBRACE on SWITCH statement");
                    }
                    //endif
                }
                //else, no LBRACE
                else
                {
                    debug("no LBRACE in SWITCH statement");
                }
                //endif
            }
            //else, ID not found
            else
            {
                debug("No ID after SWITCH token");
            }
            //endif

            break;
        }
        //endcase

        default:
        {
            debug("Reached a statement that isn't PRINT or ASSIGN or IF or WHILE or SWITCH");
            break;
        }
        //endcase
    }
    //endswitch

    return stmt;
}

struct AssignmentStatement* assign_stmt()
{
    //create assign_stmt node
    struct AssignmentStatement* assign_stmt = new struct AssignmentStatement;

    //get left hand side
    //get token
    getToken();
    //if type is ID
    if(ttype == ID)
    {
        //find variable in vector
        assign_stmt->left_hand_side = find_var(token);

        //get left operand
        //get token
        getToken();

        //if type is EQUAL
        if(ttype == EQUAL)
        {
            //get left operand
            //get token
            getToken();

            //if type is NUM
            if(ttype == NUM)
            {
                //create new value node
                assign_stmt->operand1 = new struct ValueNode;

                //convert token to int and store in operand1->value
                assign_stmt->operand1->value = atoi(token);
            }
            //else if type is ID
            else if(ttype == ID)
            {
                //find variable in vector
                assign_stmt->operand1 = find_var(token);
            }
            //endif

            //get operator if it exists
            //get token
            getToken();
            //if type is SEMICOLON, there is no operator or right operand
            if(ttype == SEMICOLON)
            {
                assign_stmt->op = 0;
            }
            //else, there is an operator and a right operand
            else
            {
                //if type is PLUS or MINUS or MULT or DIV
                if(ttype == PLUS
                   || ttype == MINUS
                   || ttype == MULT
                   || ttype == DIV)
                {
                    //make the operation type the token type
                    assign_stmt->op = ttype;

                    //get right operand
                    //get token
                    getToken();
                    if(ttype == NUM)
                    {
                        //create new value node
                        assign_stmt->operand2 = new struct ValueNode;

                        //convert token to int and store in operand2->value
                        assign_stmt->operand2->value = atoi(token);
                    }
                    else if(ttype == ID)
                    {
                        //find variable in vector
                        assign_stmt->operand2 = find_var(token);
                    }
                    //endif

                    //get semicolon
                    //get token
                    getToken();
                    //if type is SEMICOLON
                    if(ttype == SEMICOLON)
                    {
                        //this is the end of the statement
                    }
                    //endif
                }
                //endif
            }
            //endif
        }
        //endif
    }
    //endif

    return assign_stmt;
}

struct PrintStatement* print_stmt()
{
    //create print statement node
    struct PrintStatement* print_stmt = new struct PrintStatement;

    //get token
    getToken();

    //if type is PRINT
    if(ttype == PRINT)
    {
        //get token
        getToken();

        //if type is ID
        if(ttype == ID)
        {
            //find variable in vector
            print_stmt->id = find_var(token);

            //get token
            getToken();
            //if type is SEMICOLON
            if(ttype == SEMICOLON)
            {
                //this is the end of the statement
                return print_stmt;
            }
            //endif
        }
        //endif
    }

    return print_stmt;
}
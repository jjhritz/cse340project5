//
// Created by student on 11/24/16.
//

#include <cstdio>
#include <cstdlib>
#include "project5.h"
#include "statement.h"

struct StatementNode* parse_stmt_list()
{
    //create NOOP node as head of list
    struct StatementNode* head = new struct StatementNode;

    //get first statement
    //get token
    getToken();

    //if type is ID or PRINT, it's a statement
    if(ttype == ID || ttype == PRINT)
    {
        //unget token
        ungetToken();
        //parse statement
        struct StatementNode* stmt = parse_stmt();

        //get token
        getToken();
        //if type is ID or PRINT, it's another statement
        if(ttype == ID || ttype == PRINT)
        {
            //unget token
            ungetToken();
            //parse statement
            struct StatementNode* next_stmt = parse_stmt();

            stmt->next = next_stmt;
        }
        //endif

        head->next = stmt;
    }
    //else, no more statements in this list; let the caller handle it
    else
    {
        head->next = NULL;
    }
    //endif

    return head;;
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
}
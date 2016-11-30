//
// Created by student on 11/29/16.
//

#include <cstdlib>
#include "project5.h"
#include "if.h"

struct IfStatement* parse_if_stmt()
{
    //create if_stmt
    struct IfStatement* if_stmt = new struct IfStatement;

    //relop variable
    int relop;

    //get token
    getToken();

    //if type is IF or WHILE
    if(ttype == IF || ttype == WHILE)
    {
        //parse condition

        //parse left operand
        getToken();

        //if type is ID, operand is a variable
        if(ttype == ID)
        {
            //find variable
            if_stmt->condition_operand1 = find_var(token);
        }
        //else if type is NUM, operand is a number
        else if(ttype == NUM)
        {
            //create new value node
            if_stmt->condition_operand1 = new struct ValueNode;
            //convert token to int and store in value_node->value
            if_stmt->condition_operand1->value = atoi(token);
        }
        //else, unexpected token; something went wrong
        else
        {
            debug("Condition left operand is neither NUM nor ID");
        }
        //endif

        //parse operator
        //get token
        getToken();
        relop = ttype;

        //if type is GREATER
        if(ttype == GREATER)
        {
            if_stmt->condition_op = GREATER;
        }
        //else if type is LESS
        else if (ttype == LESS)
        {
            if_stmt->condition_op = LESS;
        }
        //else if type is EQUAL or NOTEQUAL
        else if(ttype == EQUAL || ttype == NOTEQUAL)
        {
            if_stmt->condition_op = NOTEQUAL;
        }
        //else, unexpected token; something went wrong
        else
        {
            debug("Condition relop is neither GREATER, LESS, EQUAL, nor NOTEQUAL");
        }
        //endif

        //parse right operand
        //get token
        getToken();

        //if type is ID, operand is a variable
        if(ttype == ID)
        {
            //find variable
            if_stmt->condition_operand2 = find_var(token);
        }
        //else if type is NUM, operand is a number
        else if(ttype == NUM)
        {
            //create new value node
            if_stmt->condition_operand2 = new struct ValueNode;
            //convert token to int and store in value_node->value
            if_stmt->condition_operand2->value = atoi(token);
        }
        //else, unexpected token; something went wrong
        else
        {
            debug("Condition right operand is neither NUM nor ID");
        }
        //endif

        //parse body
        //get token
        getToken();
        //if type is LBRACE
        if(ttype == LBRACE)
        {
            //unget token
            ungetToken();
            //parse body
            struct StatementNode* body = parse_body();

            //if relop is EQUAL, the body is the FALSE branch
            if(relop == EQUAL)
            {
                if_stmt->true_branch = NULL;
                if_stmt->false_branch = body;
            }
            //else, statement body is the TRUE branch
            else
            {
                if_stmt->true_branch = body;
                if_stmt->false_branch = NULL;
            }
            //endif
        }
        //else, unexpected token; something went wrong
        else
        {
            debug("no opening LBRACE in body of IF statement");
        }
        //endif
    }
    //else, we're missing the IF token
    else
    {
        debug("No IF or WHILE token while parsing IF statement");
    }
    //endif

    return if_stmt;
}
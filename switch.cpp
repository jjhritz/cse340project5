//
// Created by student on 12/1/16.
//

#include <cstdlib>
#include "switch.h"
#include "project5.h"

struct StatementNode* parse_case_list(struct StatementNode* exit, struct ValueNode* var)
{
    //create case_list node
    struct StatementNode* case_list;

    //get token
    getToken();

    //if type is CASE or DEFAULT
    if(ttype == CASE || ttype == DEFAULT)
    {
        //unget token
        ungetToken();

        case_list = parse_case(exit, var);

        //get next case
        getToken();

        //if type is CASE, it's another case or case_list
        if(ttype == CASE)
        {
            //unget token
            ungetToken();

            //get the next case list
            struct StatementNode* next_case_list = parse_case_list(exit, var);
            case_list->next = next_case_list;
            case_list->if_stmt->true_branch = next_case_list;
        }
        //else if type is default, it's the default and last case
        else if(ttype == DEFAULT)
        {
            //unget token
            ungetToken();

            //get the next case
            struct StatementNode* next_case = parse_case(exit, var);
            case_list->next = next_case;
            case_list->if_stmt->true_branch = next_case;
        }
        //else, it's the end of the list; let the caller handle it
        else
        {
            //set true branch of last case to exit
            case_list->if_stmt->true_branch = exit;

            //set the next pointer to the NO-OP trailer
            case_list->next = exit->goto_stmt->target;

            //unget token
            ungetToken();
        }
    }
    //else, no CASE or DEFAULT token
    else
    {
        debug("No CASE or DEFAULT token at start of case_list");
    }

    return case_list;
}


struct StatementNode* parse_case(struct StatementNode* exit, struct ValueNode* var)
{
    //create case
    struct StatementNode* cas;

    //get token
    getToken();

    //if type is CASE or DEFAULT
    if(ttype == CASE || ttype == DEFAULT)
    {
        //store case_type
        int case_ttype = ttype;

        //get token
        getToken();
        //if type is NUM, we have our switch value
        if((case_ttype == CASE && ttype == NUM)
            || case_ttype == DEFAULT)
        {
            int case_val;
            //if case_type is CASE, we need to store the value
            if(case_ttype == CASE)
            {
                //store token as integer
                 case_val = atoi(token);
            }
            //else if case_type is DEFAULT, we just ate the COLON
            else if(case_ttype == DEFAULT)
            {
                //unget token
                ungetToken();
            }

            //get token
            getToken();

            //if type is COLON
            if(ttype == COLON)
            {
                //parse body
                struct StatementNode* body = parse_body();

                //if case_type is CASE, we need a comparison
                if(case_ttype == CASE)
                {
                    //allocate cas;
                    cas = new struct StatementNode;

                    //set cas's type to IF statement
                    cas->type = IF_STMT;

                    //allocate IF statement
                    cas->if_stmt = new IfStatement;

                    //store the operands
                    cas->if_stmt->condition_operand1 = var;
                    //create new ValueNode as cas->if_stmt->condition_operand2
                    cas->if_stmt->condition_operand2 = new ValueNode;
                    //store token as int in cas->if_stmt->condition_operand2->value
                    cas->if_stmt->condition_operand2->value = case_val;

                    cas->if_stmt->condition_op = NOTEQUAL;

                    //We're using NOTEQUAL instead of EQUAL for our cases
                    //So the body has to go in the false_branch
                    cas->if_stmt->false_branch = body;

                    //Add the exit point to the end of the body
                    //1. index to the end of the body statement list

                    //create index pointer at false branch
                    struct StatementNode* inst_ptr = cas->if_stmt->false_branch;
                    //while pointer->next is not NULL
                    while(inst_ptr->next != NULL)
                    {
                        //advance to next statement
                        inst_ptr = inst_ptr->next;
                    }
                    //add exit node
                    inst_ptr->next = exit;

                    //true_branch will point to next case in case_list
                    cas->if_stmt->true_branch = NULL;
                }
                //else, case_type is DEFAULT and it's just a body/stmt_list
                else
                {
                    cas = body;

                    //create index pointer at start of body
                    struct StatementNode* inst_ptr = cas;
                    //while pointer->next is not NULL
                    while(inst_ptr->next != NULL)
                    {
                        //advance to next statement
                        inst_ptr = inst_ptr->next;
                    }
                    //add exit node
                    inst_ptr->next = exit;
                }
                //endif

                return cas;
            }
            //else type is not COLON; unexpected token
            else
            {
                debug("No COLON in case header");
            }
        }
        //else, no NUM as case index
        else
        {
            debug("No NUM after CASE token.");
        }
    }
    //else, missing CASE or DEFAULT token
    else
    {
        debug("No CASE or DEFAULT token at start of case");
    }
}
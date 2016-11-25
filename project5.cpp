#include <iostream>
#include <algorithm>
#include <memory.h>
#include "project5.h"
#include "statement.h"

struct ValueNode* vars;
int var_count = 0;

struct StatementNode* parse_generate_intermediate_representation()
{
    //allocate variable array
    vars = (struct ValueNode*) malloc(sizeof(struct ValueNode));

    //create no-op statement pointer as head
    struct StatementNode* head = new struct StatementNode;
    head->type = NOOP_STMT;

    //parse program
    head->next = parse_program();

    return head;
}

struct StatementNode* parse_program()
{
    //parse variable declarations
    parse_var_decl();

    //parse body
    struct StatementNode* body = parse_body();

    return body;
}

void parse_var_decl()
{
    //parse id list
    parse_id_list();

    //sort variable vector
    sort_arr(vars);
}

void parse_id_list()
{
    //get first ID
    getToken();
    //create value node with token as node name and 0 as value
    //vars.resize(vars.size() + 1);
    //vars.back().name = strdup(token);
    add_var(strdup(token));

    //get next token
    getToken();

    //if token is COMMA
    if(ttype == COMMA)
    {
        //parse rest of id list
        parse_id_list();
    }
    //endif

    //if token is not COMMA, it's SEMICOLON and we've reached the end of the declaration
    //this token will be consumed and won't need to be dealt with
}

struct StatementNode* parse_body()
{
    //create NOOP node as head of body
    struct StatementNode* head = new struct StatementNode;
    head->type = NOOP_STMT;

    //create statement list node
    struct StatementNode* stmt_list;

    //get token
    getToken();

    //if type is LBRACE
    if(ttype == LBRACE)
    {
        //get next token
        getToken();

        //if type is PRINT or ID
        if(ttype == PRINT || ttype == ID)
        {
            //unget token
            ungetToken();

            //parse statement list, store in statement list node
            stmt_list = parse_stmt_list();

            //get token
            getToken();
            //if type is RBRACE
            if(ttype == RBRACE)
            {
                head->next = stmt_list;
            }
            //endif
        }
        //endif
    }
    //endif

    return head;
}

bool comp_vars(const ValueNode& a, const ValueNode& b)
{
    //if a.id comes before b.id
    if(strcmp(a.name, b.name) < 0)
    {
        return true;
    }
    //else, a.id comes after b.id
    {
        return false;
    }
}

/*
//predicate for searching variables vector
bool operator== (const ValueNode& varl, const ValueNode& varr)
{
    //if the name of the left variable is the same as the name of the right variable
    if(strcmp(varl.name, varr.name) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
 */

struct ValueNode* find_var(char* symbol)
{
    /*
    //search types using std::find_if and is_type_symbol
    vector<ValueNode>::iterator it = find(vars.begin(), vars.end(), symbol);
    //if the returned iterator does not equal the end of the vector, the symbol was found
    if(it != vars.end())
    {
        //return pointer to element at iterator
        return &(*it);
    }
        //else, symbol was not found and the symbol is not a known variable
    else
    {
        return NULL;
    }
     */

    bool found = false;
    bool dne = false;
    struct ValueNode* var = NULL;

    for(int i = 0; (!found || !dne) && (i < var_count); i++)
    {
        int comp = strcmp(symbol, vars[i].name);

        if(comp == 0)
        {
            var = &(vars[i]);
            found = true;
        }
        else if(comp > 0)
        {
            dne = true;
        }
    }

    return var;
}

void add_var(char* name)
{
    //increment variable count
    var_count++;
    //resize array
    vars = (struct ValueNode*) realloc(vars, var_count * sizeof(struct ValueNode));

    //update new element at end of array
    vars[var_count - 1].name = strdup(name);
    vars[var_count - 1].value = 0;
}

void sort_arr(struct ValueNode* arr)
{
    int i, j;
    struct ValueNode key;

    for(i = 1; j < var_count; i++)
    {
        j = i;

        while(j > 0 && (strcmp(arr[j - 1].name, arr[j].name) > 0))
        {
            key = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = key;
            j--;
        }
    }
}

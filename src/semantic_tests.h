#include "ast.h"
#include "symbol_table.h"
#define TEST_FAILED 1
#define TEST_PASSED 0



Chained_Node* lookup(string variable_name,string function_scope,Chained_Node* symbol_table[]);
int assign_test(Node * assign_node,string function_name,Chained_Node * symbol_table[]);
string get_type(string variable_name, string function_name,Chained_Node * symbol_table[]);
int expression_evaluation_test(Node * expression);
void parse_instr(Node* instr,int *error_count,string function_name);
int function_call_test(Node * function_call,string function_name,Chained_Node * symbol_table[]);
int return_value_test(Node* return_node,string function_name);
void test_tree(Node* root,int * error_count);
Chained_Node *lookup_function(string function_name);
Chained_Node *lookup_variable(string variable_name, Function *function);
int legal_function_name_test(string name);
extern int error_count; 
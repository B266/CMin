#pragma once

#ifndef _UTIL_H_
#define UTIL_H_
#include"globals.h"
/* Procedure printToken prints a token
* and tis lexeme to the listing file
*/
void printToken(TokenType, const char*);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind);

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind);

/* Function newParamNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode* newDeclNode(DeclKind);

/* Function newParamNode creates a new parameter
 * node for syntax tree construction
 */
TreeNode* newParamNode(ParamKind);

/* Function newTypeNode creates a new type
 * node for syntax tree construction
 */
TreeNode* newTypeNode(TypeKind);

/* Funtion copyString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char*);

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode*);
#endif


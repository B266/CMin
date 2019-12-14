#include "globals.h"
#include "util.h"


 /* Procedure printToken prints a token
  * and its lexeme to the listing file
  */

  /* Procedure printToken prints a token
   * and its lexeme to the listing file
   */
void printToken(TokenType token, const char* tokenString)
{
	switch (token)//ELSE, IF, INT, RETURN, VOID, WHILE,
	{
	case ELSE:
	case IF:
	case INT:
	case RETURN:
	case VOID:
	case WHILE:
		fprintf(listing, "reserved word : %s\n", tokenString);
		break;
	case PLUS:fprintf(listing, "+\n"); break;
	case MINUS:fprintf(listing, "-\n"); break;
	case MULTIPLY:fprintf(listing, "*\n"); break;
	case DIVIDE:fprintf(listing, "/\n"); break;
	case LESSTHAN:fprintf(listing, "<\n"); break;
	case LESSOREQUAL:fprintf(listing, "<=\n"); break;
	case GREATERTHAN:fprintf(listing, ">\n"); break;
	case GREATEROREQUAL:fprintf(listing, ">=\n"); break;
	case EEQUAL:fprintf(listing, "==\n"); break;
	case UNEQUAL:fprintf(listing, "!=\n"); break;
	case EQUAL:fprintf(listing, "=\n"); break;
	case SEMI:fprintf(listing, ";\n"); break;
	case COMMA:fprintf(listing, ",\n"); break;
	case LPAREN:fprintf(listing, "(\n"); break;
	case RPAREN:fprintf(listing, ")\n"); break;
	case LMPAREN:fprintf(listing, "[\n"); break;
	case RMPAREN:fprintf(listing, "]\n"); break;
	case LLPAREN:fprintf(listing, "{\n"); break;
	case LRPAREN:fprintf(listing, "}\n"); break;
	case ENDFILE:fprintf(listing, "EOF\n"); break;
	case NUM:
		fprintf(listing, "NUM, val= %s\n", tokenString);
		break;
	case ID:
		fprintf(listing, "ID, name = %s\n", tokenString);
		break;
	case ERROR:
		fprintf(listing, "ERROR: %s\n", tokenString);
		break;
	default:
		fprintf(listing, "Unknown token: %d\n", token);
	}
}


/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++) t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = ExpK;
		t->kind.exp = kind;
		t->lineno = lineno;
		t->type = Void;
	}
	return t;
}

/* Function newParamNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode* newDeclNode(DeclKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = DeclK;
		t->kind.decl = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newParamNode creates a new parameter
 * node for syntax tree construction
 */
TreeNode* newParamNode(ParamKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = ParamK;
		t->kind.param = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function newTypeNode creates a new type
 * node for syntax tree constrcution
 */
TreeNode* newTypeNode(TypeKind kind)
{
	TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else {
		for (i = 0; i < MAXCHILDREN; i++)t->child[i] = NULL;
		t->sibling = NULL;
		t->nodeKind = TypeK;
		t->kind.type = kind;
		t->lineno = lineno;
	}
	return t;
}

/* Function copuString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char* s)
{
	int n;
	char* t;
	if (s == NULL)return NULL;
	n = strlen(s) + 1;
	t = (char*)malloc(n);
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* maros to increase/decrease indentation */
#define INDENT indentno+=2;
#define UNINDENT indentno -= 2;

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode* t) {

}
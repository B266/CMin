#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* declaration_list();
static TreeNode* declaration();
static TreeNode* val_declaration();
static TreeNode* arrval_declaration();
static TreeNode* fun_declaration();
static TreeNode* params();
static TreeNode* param_list();
static TreeNode* param();
static TreeNode* compound_stmt();

static void syntaxError(const char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/* 匹配当前token，若匹配，获取下一个token，若不匹配，输出错误信息 */
static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		fprintf(listing, "        ");
	}
}

/* 2. declaration_list -> declaration_list declaration | declaration */
TreeNode* declaration_list() /* declaration_list 声明列表 */
{
	TreeNode* t = declaration();
	TreeNode* p = t;
	while (token != ENDFILE)
	{
		TreeNode* q;
		q = declaration();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/* 3. declaration -> val_declaration | fun_declaration */
TreeNode* declaration() 
{
	TreeNode* t = NULL;
	TreeNode* p = t;
	TokenType tokentype = token;
	if (token == INT || token == VOID) {
		match(ID);
		char* idname = copyString(tokenString);
		if (token == SEMI) {
			t = val_declaration();
			t->attr.name = idname;
			if (tokentype == INT)
				t->type = Integer;
			else if (tokentype == VOID)
				t->type = Void;
		}
		else if (token == LMPAREN) {
			t = arrval_declaration();
			t->attr.arr.name = idname;
			if (tokentype == INT)
				t->type = IntegerArray;
			else if (tokentype == VOID)
				t->type = Void;
		}
		else if (token == LPAREN) {
			t = fun_declaration();
			t->attr.name = idname;
			if (tokentype == INT)
				t->type = Integer;
			else if (tokentype == VOID)
				t->type = Void;
		}
		else {
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			fprintf(listing, "        ");
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* val_declaration -> type_specifier ID ; */
TreeNode* val_declaration() {
	TreeNode* t = newDeclNode(VarK);
	match(SEMI);
	return t;
}

/* valarr_declaration -> type_specifier ID [ NUM ] ; */
TreeNode* arrval_declaration() {
	TreeNode* t = newDeclNode(ArrVarK);
	match(LMPAREN);
	match(NUM);
	t->attr.arr.size = atoi(tokenString);
	match(RMPAREN);
	match(SEMI);
	return t;
}

/* fun_declaration -> type_specifier ID ( params ) compound_stmt */
TreeNode* fun_declaration() {
	TreeNode* t = newDeclNode(FuncK);
	match(LPAREN);
	t->child[0] = params();
	match(RPAREN);
	t->child[1] = compound_stmt();
	return t;
}

/* params -> param_list | void */
TreeNode* params() {
	TreeNode* t = NULL;
	if (token == VOID) {
		t = newParamNode(NonArrParamK);
		match(VOID);
	}
	else if (token == INT) {
		t = param_list();
	}
	return t;
}

/* param_list -> param_list , param | param */
TreeNode* param_list() {
	TreeNode* t = param();
	TreeNode* p = t;
	while (token == COMMA)
	{
		TreeNode* q;
		match(COMMA);
		q = param();
		if (q != NULL) {
			if (t == NULL)
				t = p = q;
			else {
				p->sibling = q;
				p = q;
			}
		}
	}
	return t;
}

/* param -> int ID | int ID [ ] */
TreeNode* param() {
	TreeNode* t = newParamNode(ArrParamK);
	match(INT);
	match(ID);
	if (token == LMPAREN) {
		match(LMPAREN);
		match(RMPAREN);
	}
	return t;
}

/******************************************************/
/*       the primary function of the parser           */
/******************************************************/
/* Function parse returns the newly
 * constructed syntax tree
 */
TreeNode* parse(void)
{
	TreeNode* t;
	token = getToken();
	t = declaration_list(); /* 1. program -> declaration_list */
	if (token != ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}
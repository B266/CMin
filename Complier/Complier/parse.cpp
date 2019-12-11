#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* declaration_list();
static TreeNode* declaration();
static TreeNode* var_declaration();
static TreeNode* arrvar_declaration();
static TreeNode* fun_declaration();
static TreeNode* params();
static TreeNode* param_list();
static TreeNode* param();
static TreeNode* compound_stmt();
static TreeNode* local_declarations();
static TreeNode* statement_list();
static TreeNode* statement();

static void syntaxError(const char* message)
{
	fprintf(listing, "\n>>> ");
	fprintf(listing, "Syntax error at line %d: %s", lineno, message);
	Error = TRUE;
}

/* ƥ�䵱ǰtoken����ƥ�䣬��ȡ��һ��token������ƥ�䣬���������Ϣ */
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
TreeNode* declaration_list() /* declaration_list �����б� */
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
	if (token == INT) {
		match(ID);
		char* idname = copyString(tokenString);
		if (token == SEMI) {
			t = var_declaration();
			t->attr.name = idname;
			t->type = Integer;
		}
		else if (token == LMPAREN) {
			t = arrvar_declaration();
			t->attr.arr.name = idname;
			t->type = IntegerArray;
		}
		else if (token == LPAREN) {
			t = fun_declaration();
			t->attr.name = idname;
			t->type = Integer;
		}
		else {
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			fprintf(listing, "        ");
		}
	}
	else if (token == VOID) {
		match(ID);
		char* idname = copyString(tokenString);
		if (token == SEMI) {
			t = var_declaration();
			t->attr.name = idname;
			t->type = Void;
		}
		else if (token == LMPAREN) {
			t = arrvar_declaration();
			t->attr.arr.name = idname;
			t->type = Void;
		}
		else if (token == LPAREN) {
			t = fun_declaration();
			t->attr.name = idname;
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

/* 4. val_declaration -> type_specifier ID ; */
TreeNode* var_declaration() {
	TreeNode* t = newDeclNode(VarK);
	match(SEMI);
	return t;
}

/* 4'. valarr_declaration -> type_specifier ID [ NUM ] ; */
TreeNode* arrvar_declaration() {
	TreeNode* t = newDeclNode(ArrVarK);
	match(LMPAREN);
	match(NUM);
	t->attr.arr.size = atoi(tokenString);
	match(RMPAREN);
	match(SEMI);
	return t;
}

/* 6. fun_declaration -> type_specifier ID ( params ) compound_stmt */
TreeNode* fun_declaration() {
	TreeNode* t = newDeclNode(FuncK);
	match(LPAREN);
	t->child[0] = params();
	match(RPAREN);
	t->child[1] = compound_stmt();
	return t;
}

/* 7. params -> param_list | void */
TreeNode* params() {
	TreeNode* t = NULL;
	if (token == VOID) {
		t = newParamNode(NonArrParamK);
		t->attr.type = VOID;
		match(VOID);
	}
	else if (token == INT) {
		t = param_list();
	}
	return t;
}

/* 8. param_list -> param_list , param | param */
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

/* 9. param -> int ID | int ID [ ] */
TreeNode* param() {
	TreeNode* t = newParamNode(ArrParamK);
	match(INT);
	match(ID); //param�����һ���ַ���ID
	char* idname = copyString(tokenString);
	if (token == LMPAREN) {
		match(LMPAREN);
		match(RMPAREN); //param�����һ���ַ���
		t->attr.arr.name = idname;
		t->type = IntegerArray;
	}
	else {
		t->attr.name = idname;
		t->type = Integer;
	}
	return t;
}

/* 10. compound_stmt -> { local_declarations statement_list } */
TreeNode* compound_stmt() {
	TreeNode* t = newStmtNode(CompK);
	match(LLPAREN);
	t->child[0] = local_declarations();
	t->child[1] = statement_list();
	match(LRPAREN);
	return t;
}

/* 11. local_declarations -> local_declarations var_declaration | empty */
TreeNode* local_declarations() {
	TreeNode* t = NULL; //����Ϊ��
	TreeNode* p = t;
	while (token == INT)
	{
		match(INT);
		match(ID);
		char* idname = copyString(tokenString);
		TreeNode* q;
		if (token == SEMI) {
			q = var_declaration();
			q->attr.name = idname;
			q->type = Integer;
		}
		else if (token == LMPAREN) {
			q = arrvar_declaration();
			q->attr.arr.name = idname;
			q->type = IntegerArray;
		}
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

/* 12. statement_list -> statement_list statement | empty */
TreeNode* statement_list() {
	TreeNode* t = NULL;
	TreeNode* p = t;
	while (token != LRPAREN) {
		TreeNode* q;

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
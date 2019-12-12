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
static TreeNode* expression_stmt();
static TreeNode* selection_stmt();
static TreeNode* iteration_stmt();
static TreeNode* return_stmt();
static TreeNode* expression();
static TreeNode* var();
static TreeNode* simple_expression();

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

/* 3. declaration -> var_declaration | fun_declaration */
TreeNode* declaration() 
{
	TreeNode* t = NULL;
	TreeNode* p = t;
	TokenType tokentype = token;
	if (token == INT) {
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
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
	}
	else if (token == VOID) {
		match(VOID);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
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
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* 4. var_declaration -> type_specifier ID ; */
TreeNode* var_declaration() {
	TreeNode* t = newDeclNode(VarK);
	match(SEMI);
	return t;
}

/* 4'. arrvar_declaration -> type_specifier ID [ NUM ] ; */
TreeNode* arrvar_declaration() {
	TreeNode* t = newDeclNode(ArrVarK);
	match(LMPAREN);
	if (token == NUM) {
		t->attr.arr.size = atoi(tokenString);
	}
	match(NUM);
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
	if (t != NULL && token == ID) {
		char* idname = copyString(tokenString);
		match(ID); //param的最后一个字符是ID
		if (token == LMPAREN) {
			match(LMPAREN);
			match(RMPAREN); //param的最后一个字符是
			t->attr.arr.name = idname;
			t->type = IntegerArray;
		}
		else {
			t->attr.name = idname;
			t->type = Integer;
		}
	}
	
	return t;
}

/* 10. compound_stmt -> { local_declarations statement_list } */
TreeNode* compound_stmt() {
	TreeNode* t = newStmtNode(CompK);
	match(LLPAREN);
	t->child[0] = local_declarations(); //复合语句节点的第一个孩子为局部声明节点
	t->child[1] = statement_list(); //复合语句节点的第二个孩子为语句列表
	match(LRPAREN);
	return t;
}

/* 11. local_declarations -> local_declarations var_declaration | empty */
TreeNode* local_declarations() {
	TreeNode* t = NULL; //可能为空
	TreeNode* p = t;
	while (token == INT)
	{
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
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
		
	}
	return t;
}

/* 12. statement_list -> statement_list statement | empty */
TreeNode* statement_list() {
	TreeNode* t = NULL; //可能为空
	TreeNode* p = t;
	while (token == ID || token == LLPAREN || token == IF || 
		token == WHILE || token == RETURN || token == SEMI ||
		token == LPAREN || token == NUM) 
	{
		TreeNode* q;
		q = statement();
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

/* 13. statement -> expression_stmt
				   | compound_stmt
				   | selection_stmt
				   | iteration_stmt
				   | return_stmt
*/
TreeNode* statement() {
	TreeNode* t = NULL;
	if (token == ID || token == SEMI || 
		token == LPAREN || token == NUM) {
		t = expression_stmt();
	}
	else if (token == LLPAREN) {
		t = compound_stmt();
	}
	else if (token == IF) {
		t = selection_stmt();
	}
	else if (token == WHILE) {
		t = iteration_stmt();
	}
	else if (token == RETURN) {
		t = return_stmt();
	}
	return t;
}

/* 14. expression_stmt -> expression ; | ; */
TreeNode* expression_stmt() {
	TreeNode* t = NULL;
	if (token == ID || token == LPAREN || token == NUM) {
		t = expression();
		match(SEMI);
	}
	else if (token == SEMI)
		match(SEMI);
	return t;
}

/* 15. selection_stmt -> if ( expression ) statement
					    | if ( expression ) statement else statement 
*/
TreeNode* selection_stmt() {
	TreeNode* t = newStmtNode(IfK);
	match(IF);
	match(LPAREN);
	t->child[0] = expression();
	match(RPAREN);
	t->child[1] = statement();
	if (token == ELSE) {
		match(ELSE);
		t->child[2] = statement();
	}
	return t;
}

/* 16. iteration_stmt -> while ( expression ) statement */
TreeNode* iteration_stmt() {
	TreeNode* t = newStmtNode(IterK);
	match(WHILE);
	match(LPAREN);
	t->child[0] = expression();
	match(RPAREN);
	t->child[1] = statement();
	return t;
}

/* 17. return_stmt -> return ; | return expression ; */
TreeNode* return_stmt() {
	TreeNode* t = newStmtNode(RetK);
	match(RETURN);
	if (token != SEMI) {
		t->child[0] = expression();
		match(SEMI);
	}
	else {
		match(SEMI);
	}
	return t;
}

/* 18. expression -> var = expression | simple_expression */
TreeNode* expression() {
	TreeNode* t = NULL;
	return t;
}

/* 19. var -> ID | ID [ expression ] */
TreeNode* var() {

}

/* 20. simple_expression -> additive_expression relop additive_expression
						   | additive_expression 
*/
TreeNode* simple_expression() {

}

/* 21. relop -> <= | < | > | >= | == | != */


/* 22. additive_expression -> additive_expression addop term | term */


/* 23. addop -> + | - */


/* 24. term -> term mulop factor | factor */


/* 25. mulop -> * | / */


/* 26. factor -> ( expression ) | var | call | NUM */


/* 27. call -> ID ( args ) */


/* 28. args -> arg_list | empty */


/* 29. arg_list -> arg_list , expression | expression */



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
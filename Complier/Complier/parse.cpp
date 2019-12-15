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
static TreeNode* selection_stmt();
static TreeNode* iteration_stmt();
static TreeNode* return_stmt();
static TreeNode* assignment_stmt();
static TreeNode* expression();
static TreeNode* additive_expression();
static TreeNode* term();
static TreeNode* factor();
static TreeNode* args();
static TreeNode* arg_list();

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
	TreeNode* q = NULL;
	if (token == INT) {
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
			if (token == SEMI) {
				t = var_declaration();
				t->attr.name = idname;
				q = newTypeNode(TypeNameK);
				q->type = Integer;
				t->child[0] = q;
			}
			else if (token == LMPAREN) {
				t = arrvar_declaration();
				t->attr.arr.name = idname;
				q = newTypeNode(TypeNameK);
				q->type = Integer;
				t->child[0] = q;
			}
			else if (token == LPAREN) {
				t = fun_declaration();
				t->attr.name = idname;
				q = newTypeNode(TypeNameK);
				q->type = Integer;
				t->child[0] = q;
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
				q = newTypeNode(TypeNameK);
				q->type = Void;
				t->child[0] = q;
			}
			else if (token == LMPAREN) {
				t = arrvar_declaration();
				t->attr.arr.name = idname;
				q = newTypeNode(TypeNameK);
				q->type = Void;
				t->child[0] = q;
			}
			else if (token == LPAREN) {
				t = fun_declaration();
				t->attr.name = idname;
				q = newTypeNode(TypeNameK);
				q->type = Void;
				t->child[0] = q;
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
	t->child[1] = params();
	match(RPAREN);
	t->child[2] = compound_stmt();
	return t;
}

/* 7. params -> param_list | void */
TreeNode* params() {
	TreeNode* t = NULL;
	if (token == VOID) {
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
	TreeNode* t = NULL;
	TreeNode* p = NULL;
	match(INT);
	if (token == ID) {
		char* idname = copyString(tokenString);
		match(ID); //param的最后一个字符是ID
		if (token == LMPAREN) {
			match(LMPAREN);
			match(RMPAREN); //param的最后一个字符是
			t = newParamNode(ArrParamK);
			t->attr.arr.name = idname;
			p = newTypeNode(TypeNameK);
			p->type = IntegerArray;
			t->child[0] = p;
		}
		else {
			t = newParamNode(NonArrParamK);
			t->attr.name = idname;
			p = newTypeNode(TypeNameK);
			p->type = Integer;
			t->child[0] = p;
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
	match(RLPAREN);
	return t;
}

/* 11. local_declarations -> local_declarations var_declaration | empty */
TreeNode* local_declarations() {
	TreeNode* t = NULL; //可能为空
	TreeNode* p = t;
	TreeNode* r = NULL;
	while (token == INT)
	{
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
			TreeNode* q=NULL;
			if (token == SEMI) {
				q = var_declaration();
				q->attr.name = idname;
				r = newTypeNode(TypeNameK);
				r->type = Integer;
				q->child[0] = r;
			}
			else if (token == LMPAREN) {
				q = arrvar_declaration();
				q->attr.arr.name = idname;
				r = newTypeNode(TypeNameK);
				r->type = IntegerArray;
				q->child[0] = r;
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

/* 13. statement -> assignment_stmt
				   | compound_stmt
				   | selection_stmt
				   | iteration_stmt
				   | return_stmt
*/
TreeNode* statement() {
	TreeNode* t = NULL;
	if (token == ID) {
		t = assignment_stmt();
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

/* 15. selection_stmt -> if ( expression ) statement
					    | if ( expression ) statement else statement 
*/
TreeNode* selection_stmt() {
	TreeNode* t = newStmtNode(IfK);
	match(IF);
	match(LPAREN);
	if (t != NULL) t->child[0] = expression();
	match(RPAREN);
	if (t != NULL) t->child[1] = statement();
	if (token == ELSE) {
		match(ELSE);
		if (t != NULL) t->child[2] = statement();
	}
	return t;
}

/* 16. iteration_stmt -> while ( expression ) statement */
TreeNode* iteration_stmt() {
	TreeNode* t = newStmtNode(IterK);
	match(WHILE);
	match(LPAREN);
	if (t != NULL) t->child[0] = expression();
	match(RPAREN);
	if (t != NULL) t->child[1] = statement();
	return t;
}

/* 17. return_stmt -> return ; | return additive_expression ; */
TreeNode* return_stmt() {
	TreeNode* t = newStmtNode(RetK);
	if (t != NULL) {
		match(RETURN);
		if (token != SEMI) {
			t->child[0] = additive_expression();
			match(SEMI);
		}
		else {
			match(SEMI);
		}
	}
	return t;
}

/* 18. assignment_stmt -> ID = additive_expression ; 
						| ID [ additive_expression ] = additive_expression ;
						| ID ( args ) ; */
TreeNode* assignment_stmt() {
	TreeNode* t = NULL;
	char* idname = copyString(tokenString);
	TreeNode* p = newExpNode(IdK);
	if (p != NULL) 
		p->attr.name = idname;
	TreeNode* q = newExpNode(ArrIdK);
	if (q != NULL)
		q->attr.arr.name = idname;
	match(ID);
	if (token == EQUAL) {
		t = newStmtNode(AssignK);
		if (t != NULL) {
			t->child[0] = p;
			TreeNode* r = newTypeNode(TypeNameK);
			r->type = Integer;
			p->child[0] = r;
			match(EQUAL);
			t->child[1] = additive_expression();
			match(SEMI);
		}
	}
	else if (token == LMPAREN) {
		t = newStmtNode(AssignK);
		if (t != NULL) {
			t->child[0] = q;
			match(LMPAREN);
			TreeNode* r = newTypeNode(TypeNameK);
			r->type = IntegerArray;
			q->child[0] = r;
			q->child[1] = additive_expression();
			match(RMPAREN);
			match(EQUAL);
			t->child[1] = additive_expression();
			match(SEMI);
		}
	}
	else if (token == LPAREN) {
		t = newExpNode(CallK);
		t->attr.name = idname;
		if (t != NULL) {
			match(LPAREN);
			TreeNode* r = newTypeNode(TypeNameK);
			r->type = Integer;
			t->child[0] = r;
			t->child[1] = args();
			match(RPAREN);
			match(SEMI);
		}
	}
	return t;
}

/* 19. var -> ID | ID [ additive_expression ] */


/* 20. expression -> additive_expression relop additive_expression */
TreeNode* expression() {
	TreeNode* t = additive_expression();
	if (token == LESSOREQUAL || token == LESSTHAN ||
		token == GREATERTHAN || token == GREATEROREQUAL ||
		token == EEQUAL || token == UNEQUAL) {
		TreeNode* p = newExpNode(OpK);
		if (p != NULL) {
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t != NULL) {
			t->child[1] = additive_expression();
		}
	}
	return t;
}

/* 21. relop -> <= | < | > | >= | == | != */


/* 22. additive_expression -> additive_expression addop term | term */
TreeNode* additive_expression() {
	TreeNode* t = term();
	TreeNode* p = t;
	while (token == PLUS || token == MINUS)
	{
		TreeNode* q = newExpNode(OpK);
		if (q != NULL) {
			q->attr.op = token;
			match(token);
			q->child[0] = t;
		}
		if (q != NULL) q->child[1] = term();
		t = q;
	}
	return t;
}

/* 23. addop -> + | - */


/* 24. term -> term mulop factor | factor */
TreeNode* term() {
	TreeNode* t = factor();
	TreeNode* p = t;
	while (token == MULTIPLY || token == DIVIDE)
	{
		TreeNode* q = newExpNode(OpK);
		if (q != NULL) {
			q->attr.op = token;
			match(token);
			q->child[0] = t;
		}
		if (q != NULL) q->child[1] = factor();
		t = q;
	}
	return t;
}

/* 25. mulop -> * | / */


/* 26. factor -> ( additive_expression ) | var | call | NUM */
TreeNode* factor() {
	TreeNode* t = NULL;
	if (token == LPAREN) {
		match(LPAREN);
		t = additive_expression();
		match(RPAREN);
	}
	else if (token == ID) {
		char* idname = copyString(tokenString);
		TreeNode* p = newExpNode(IdK);
		if (p != NULL)
			p->attr.name = idname;	
		TreeNode* q = newExpNode(ArrIdK);
		if (q != NULL)
			q->attr.arr.name = idname;
		TreeNode* r = newExpNode(CallK);
		if (r != NULL)
			r->attr.name = idname;
		match(ID);
		if (token == LMPAREN) {
			match(LMPAREN);
			t = q;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = IntegerArray;
			t->child[0] = w;
			t->child[1] = additive_expression();
			match(RMPAREN);
		}
		else if (token == LPAREN) {
			match(LPAREN);
			t = r;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
			t->child[1] = args();
			match(RPAREN);
		}
		else {
			t = p;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
		}
			
	}
	else if (token == NUM) {
		t = newExpNode(ConstK);
		t->attr.val = atoi(copyString(tokenString));
		TreeNode* w = newTypeNode(TypeNameK);
		w->type = Integer;
		t->child[0] = w;
		match(NUM);
	}
	return t;
}


/* 27. call -> ID ( args ) */


/* 28. args -> arg_list | empty */
TreeNode* args() {
	TreeNode* t = arg_list();
	return t;
}

/* 29. arg_list -> arg_list , additive_expression | additive_expression */
TreeNode* arg_list() {
	TreeNode* t = additive_expression();
	TreeNode* p = t;
	while (token == COMMA)
	{
		TreeNode* q;
		match(COMMA);
		q = additive_expression();
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
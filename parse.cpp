#include"globals.h"
#include"util.h"
#include"scan.h"
#include"parse.h"


/*
DeclNode:
	FuncK: 
		attr.name = ID
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer
		child[1] = ParamNode | NULL
		child[2] = StmtNode(CompK) | NULL
		sibling = DeclNode | NULL
	VarK: 
		attr.name = ID
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer		
		sibling = DeclNode | NULL
	ArrVarK: 
		attr.arr.name = ID
		attr.arr.size = NUM
		type = Void | Integer
		child[0] = TypeNode(TypeNameK) : type = Void | Integer
		sibling = DeclNode | NULL

ParamNode:
	ArrParamK: 
		attr.arr.name = ID
		type = IntegerArray
		child[0] = TypeNode(TypeNameK) : type = IntegerArray
		sibling = ParamNode | NULL
	NonArrParamK:
		attr.name = ID
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = ParamNode | NULL

StmtNode:
	AssignK:
		child[0] = ExpNode(IdK)
		child[1] = ExpNode
		sibling = StmtNode | NULL
	CompK:
		child[0] = DeclNode( VarK | ArrVarK ) | NULL
		child[1] = StmtNode | NULL
		sibling = NULL
	IfK:
		child[0] = ExpNode(OpK)
		child[1] = StmtNode
		child[2] = StmtNode | NULL
		sibling = StmtNode | NULL
	IterK:
		child[0] = ExpNode(OpK)
		child[1] = StmtNode
		sibling = StmtNode | NULL
	RetK:
		child[0] = ExpNode | NULL

ExpNode:
	OpK:
		attr.op = + - * / < <= >= > == !=
		child[0] = ExpNode
		child[1] = ExpNode
		sibling = NULL
	ConstK:
		attr.val = NUM
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = NULL
	IdK:
		attr.name = ID
		type = Integer
		child[0] = TypeNode(TypeNameK) : type = Integer
		sibling = NULL
	ArrIdK:
		attr.arr.name = ID
		type = IntegerArray
		child[0] = TypeNode(TypeNameK) : type = IntegerArray
		child[1] = ExpNode
		sibling = NULL
	CallK:
		attr.name = ID
		type = Integer | Void
		child[0] = TypeNode(TypeNameK) : type = Integer | Void
		child[1] = ExpNode | NULL
		sibling = NULL

TypeNode:
	TypeNameK:
		type = ExpType

*/

static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
static TreeNode* declaration_list(); //声明列表
static TreeNode* declaration(); //声明
static TreeNode* var_declaration(); //整型变量声明
static TreeNode* arrvar_declaration(); //整型数组变量声明
static TreeNode* fun_declaration(); //函数声明
static TreeNode* params(); //函数内参数
static TreeNode* param_list(); //参数列表
static TreeNode* param(); //参数
static TreeNode* compound_stmt(); //复合语句
static TreeNode* local_declarations(); //局部变量声明
static TreeNode* statement_list(); //语句列表
static TreeNode* statement(); //语句
static TreeNode* selection_stmt(); //if语句
static TreeNode* iteration_stmt(); //while语句
static TreeNode* return_stmt(); //return语句
static TreeNode* assignment_stmt(); //赋值语句
static TreeNode* expression(); //表达式
static TreeNode* additive_expression(); //算数表达式
static TreeNode* term();
static TreeNode* factor();
static TreeNode* args(); //函数参数
static TreeNode* arg_list(); //函数参数列表

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

/* 3. declaration -> var_declaration | arrvar_declaration | fun_declaration */
TreeNode* declaration() 
{
	TreeNode* t = NULL;
	TreeNode* p = t;
	if (token == INT) {
		match(INT);
		if (token == ID) {
			char* idname = copyString(tokenString);
			match(ID);
			if (token == SEMI) {
				t = var_declaration();
				t->attr.name = idname;
				t->type = Integer;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = Integer;
				t->child[0] = q;
			}
			else if (token == LMPAREN) {
				t = arrvar_declaration();
				t->attr.arr.name = idname;
				t->type = IntegerArray;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = IntegerArray;
				t->child[0] = q;
			}
			else if (token == LPAREN) {
				t = fun_declaration();
				t->attr.name = idname;
				t->type = Integer;
				TreeNode* q = newTypeNode(TypeNameK);
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
				t->type = Void;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = Void;
				t->child[0] = q;
			}
			else if (token == LMPAREN) {
				t = arrvar_declaration();
				t->attr.arr.name = idname;
				t->type = Void;
				TreeNode* q = newTypeNode(TypeNameK);
				q->type = Void;
				t->child[0] = q;
			}
			else if (token == LPAREN) {
				t = fun_declaration();
				t->attr.name = idname;
				t->type = Void;
				TreeNode* q = newTypeNode(TypeNameK);
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

/* 5. arrvar_declaration -> type_specifier ID [ NUM ] ; */
TreeNode* arrvar_declaration() {
	TreeNode* t = newDeclNode(ArrVarK);
	match(LMPAREN);
	if (token == NUM) {
		t->attr.arr.size = atoi(tokenString);
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
	t->child[2]->isInFuncCom = true;
	return t;
}

/* 7. params -> param_list | void | empty */
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
		match(COMMA);
		TreeNode* q;
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
	match(INT);
	if (token == ID) {
		char* idname = copyString(tokenString);
		match(ID); //param的最后一个字符是ID
		if (token == LMPAREN) {
			match(LMPAREN);
			match(RMPAREN); //param的最后一个字符是']'
			t = newParamNode(ArrParamK);
			t->attr.arr.name = idname;
			t->type = IntegerArray;
			TreeNode* p = newTypeNode(TypeNameK);
			p->type = IntegerArray;
			t->child[0] = p;
		}
		else {
			t = newParamNode(NonArrParamK);
			t->attr.name = idname;
			t->type = Integer;
			TreeNode* p = newTypeNode(TypeNameK);
			p->type = Integer;
			t->child[0] = p;
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
				q->type = Integer;
				r = newTypeNode(TypeNameK);
				r->type = Integer;
				q->child[0] = r;
			}
			else if (token == LMPAREN) {
				q = arrvar_declaration();
				q->attr.arr.name = idname;
				q->type = IntegerArray;
				r = newTypeNode(TypeNameK);
				r->type = IntegerArray;
				q->child[0] = r;
			}
			else {
				syntaxError("unexpected token -> ");
				printToken(token, tokenString);
				token = getToken();
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
		else {
			syntaxError("unexpected token -> ");
			printToken(token, tokenString);
			token = getToken();
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
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
						| ID ( args ) ; 
*/
TreeNode* assignment_stmt() {
	TreeNode* t = NULL;
	char* idname = copyString(tokenString);
	match(ID);
	if (token == EQUAL || token == LMPAREN) {
		t = newStmtNode(AssignK);
		if (t != NULL) {
			
			if (token == LMPAREN) {
				TreeNode* q = newExpNode(ArrIdK);
				if (q != NULL) {
					q->attr.arr.name = idname;
					q->type = Integer;
					t->child[0] = q;
					TreeNode* r = newTypeNode(TypeNameK);
					r->type = Integer;
					q->child[0] = r;
					match(LMPAREN);
					q->child[1] = additive_expression();
					match(RMPAREN);
				}
				
			}
			else
			{
				TreeNode* p = newExpNode(IdK);
				if (p != NULL) {
					p->attr.name = idname;
					p->type = Integer;
					t->child[0] = p;
					TreeNode* r = newTypeNode(TypeNameK);
					r->type = Integer;
					p->child[0] = r;
				}
				
			}
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
			r->type = Void;
			t->child[0] = r;
			t->child[1] = args();
			t->type = Void;
			match(RPAREN);
			match(SEMI);
		}
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}

/* 19. var -> ID | ID [ additive_expression ] | ID [ ] */


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
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
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
			if (token == RMPAREN) {
				t = q;
				t->type = IntegerArray;
				TreeNode* w = newTypeNode(TypeNameK);
				w->type = IntegerArray;
				t->child[0] = w;
			}
			else {
				t = q;
				t->type = Integer;
				TreeNode* w = newTypeNode(TypeNameK);
				w->type = Integer;
				t->child[0] = w;
				t->child[1] = additive_expression();
			}
			match(RMPAREN);
		}
		else if (token == LPAREN) {
			match(LPAREN);
			t = r;
			t->type = Integer;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
			t->child[1] = args();
			match(RPAREN);
		}
		else {
			t = p;
			t->type = Integer;
			TreeNode* w = newTypeNode(TypeNameK);
			w->type = Integer;
			t->child[0] = w;
		}
			
	}
	else if (token == NUM) {
		t = newExpNode(ConstK);
		t->attr.val = atoi(copyString(tokenString));
		t->type = Integer;
		TreeNode* w = newTypeNode(TypeNameK);
		w->type = Integer;
		t->child[0] = w;
		match(NUM);
	}
	else {
		syntaxError("unexpected token -> ");
		printToken(token, tokenString);
		token = getToken();
	}
	return t;
}


/* 27. call -> ID ( args ) */


/* 28. args -> arg_list | empty */
TreeNode* args() {
	TreeNode* t = NULL;
	if (token == RPAREN)
		return t;
	else {
		t = arg_list();
		return t;
	}
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
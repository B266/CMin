#include"globals.h"
#include"symtab.h"
#include"analyze.h"
#include"util.h"

static char* funcName;
static int preserveLastScope = FALSE;


 /* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
static void traverse(TreeNode* t,
	void (*preProc)(TreeNode*),
	void (*postProc)(TreeNode*))
{
	if (t != NULL)
	{
		preProc(t);
		{
			int i;
			for (i = 0; i < MAXCHILDREN; i++)
				traverse(t->child[i], preProc, postProc);

		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

static void insertIOFunc(void)
{
	TreeNode* func;
	TreeNode* typeSpec;
	TreeNode* param;
	TreeNode* compStmt;

	func = newDeclNode(FuncK);

	typeSpec = newTypeNode(TypeNameK);
	typeSpec->attr.type = INT;
	func->type = Integer;

	compStmt = newStmtNode(CompK);
	compStmt->child[0] = NULL;	// no local var
	compStmt->child[1] = NULL;	// no stmt

	func->lineno = 0;
	strcpy(func->attr.name, "input");
	func->child[0] = typeSpec;
	func->child[1] = NULL;	// no param
	func->child[2] = compStmt;

	char tempName[] = "input";
	st_insert(tempName, -1, addLocation(), func);

	func = newDeclNode(FuncK);

	typeSpec = newTypeNode(TypeNameK);

}

/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode* t)
{
	if (t == NULL)return;
	else return;
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case AssignK:
		case ReadK:
			/* not yet in table, so treat as new definition */
			if (st_lookup(t->attr.name) == -1)
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* already in table, so ignore location
				   add line number of use only */
				st_insert(t->attr.name, t->lineno, 0);
			break;
		default:
			break;
		}
		break;
	case ExpK:
		switch (t->kind.exp)
		{
		case IdK:
			/* not yet in table, so treat as new definition */
			if (st_lookup(t->attr.name) == -1)
				st_insert(t->attr.name, t->lineno, location++);
			else
				/* already in table, so ignore location
				   add line number of use only */
				st_insert(t->attr.name, t->lineno, 0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

}

/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode* syntaxTree)
{
	traverse(syntaxTree, insertNode, nullProc);
	if (TraceAnalyze)
	{
		fprintf(listing, "\nSymbol table:\n\n");
		printSymTab(listing);
	}
}

static void typeError(TreeNode* t, const char* message)
{
	fprintf(listing, "Type error at line %d: %s\n", t->lineno, message);
	Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode* t)
{
	switch (t->nodeKind)
	{
	case ExpK:
		switch (t->kind.exp)
		{
		case OpK:
			if ((t->child[0]->type != Integer) ||
				(t->child[1]->type != Integer))
				typeError(t, "Op applied to non-integer");
			if ((t->attr.op == EQ) || (t->attr.op == LT))
				t->type = Boolean;
			else
				t->type = Integer;
			break;
		case ConstK:
		case IdK:
			t->type = Integer;
			break;
		default:
			break;
		}
		break;
	case StmtK:
		switch (t->kind.stmt)
		{
		case IfK:
			if (t->child[0]->type == Integer)
				typeError(t->child[0], "if test is not Boolean");
			break;
		case AssignK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "assignment of non-integet value");
			break;
		case WriteK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "write of non-integer value");
			break;
		case RepeatK:
			if (t->child[1]->type == Integer)
				typeError(t->child[1], "repeat test is not Boolean");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode* syntaxTree)
{
	traverse(syntaxTree, nullProc, checkNode);
}
# CminComplier

## grammer
 1. program -> declaration_list
 2. declaration_list -> declaration_list declaration | declaration
 3. declaration -> var_declaration | arrvar_declaration | fun_declaration
 4. var_declaration -> type_specifier ID ;
 4. arrvar_declaration -> type_specifier ID [ NUM ] ;
 6. fun_declaration -> type_specifier ID ( params ) compound_stmt
 7. params -> param_list | void
 8. param_list -> param_list , param | param
 9. param -> int ID | int ID [ ]
 10. compound_stmt -> { local_declarations statement_list }
 11. local_declarations -> local_declarations var_declaration 
                          | local declarations arrvar_declaration
                          | empty
 12. statement_list -> statement_list statement | empty
 13. statement -> assignment_stmt
				             | compound_stmt
				             | selection_stmt
				             | iteration_stmt
				             | return_stmt
 14. assignment_stmt -> ID = additive_expression ; 
						                 | ID [ additive_expression ] = additive_expression ;
						                 | ID ( args ) ; 
 15. selection_stmt -> if ( expression ) statement
					                 | if ( expression ) statement else statement
 16. iteration_stmt -> while ( expression ) statement
 17. return_stmt -> return ; | return additive_expression ;

 19. var -> ID | ID [ additive_expression ]
 20. expression -> additive_expression relop additive_expression
 21. relop -> <= | < | > | >= | == | !=
 22. additive_expression -> additive_expression addop term | term
 23. addop -> + | -
 24. term -> term mulop factor | factor
 25. mulop -> * | /
 26. factor -> ( additive_expression ) | var | call | NUM
 27. call -> ID ( args )
 28. args -> arg_list | empty
 29. arg_list -> arg_list , additive_expression | additive_expression

## to do
 1. read and study [the repo](https://github.com/isairz/cminus) 
 2. rewrite the whole project 
 3. support char type
 
## log 
 day 4 finish whole project , think some idea to imporve

 ## reference 
 1. Compiler Construction: Principles and Practice - Kenneth C.Louden
 2. Parsing Techniques - Dick Grune; Ceriel J.H. Jacobs

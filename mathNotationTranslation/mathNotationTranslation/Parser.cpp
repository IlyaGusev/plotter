#include "Parser.h"
#include "arithmeticNode.h"
#include "variableNode.h"
#include "lex.yy.cc"

Parser::Parser() { prev.reset(); }
Parser::~Parser() {}
//Parser::Parser(string input, string output) {
//	if (input == "mathml")
//		fileType[0] = mathml;
//	if (input == "openmath")
//		fileType[0] = openmath;
//	if (input == "tex")
//		fileType[0] = tex;	
//
//	if (output == "mathml")
//		fileType[1] = mathml;
//	if (output == "openmath")
//		fileType[1] = openmath;
//	if (output == "tex")
//		fileType[1] = tex;
//}
//
//int Parser::getType(int id) {
//	return fileType[id];
//}

void Parser::addNode(){}

void Parser::arithmeticOperation(istream& is, int oper, unique_ptr<Node>& position)
{
	position = unique_ptr<arithmeticNode>(new arithmeticNode());
	unique_ptr<arithmeticNode> pDerived(static_cast<arithmeticNode*>(position.release()));
	pDerived->operation = oper;
	position = move(pDerived);
	position.get()->left = move(prev);
	processText(is, position->right);
	position.get()->right = move(prev);
	prev = move(position);
}

bool Parser::processText(istream& is, unique_ptr <Node>& position) {
	int tok = lexer->yylex(&is, &cout);
	switch (tok) {
	case ID:
	case NUMBER:
		position.reset(new variableNode(value));
		prev = move(position);
		return false;
	case LOPER:
	case ROPER:
	case LNUM:
	case RNUM:
	case LID:
	case RID:
		processText(is, position);
		return false;
	case 0:
		return true;
	default:
		arithmeticOperation(is, tok, position);
		return false;
	}
	return true;
}
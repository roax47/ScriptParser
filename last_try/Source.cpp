#include<assert.h>
#include<cctype>
#include<stdlib.h>     
#include<stdio.h>
#include<iostream>
#define NONE 100
#define VAR 1
#define INT 2
#define MUL 3
#define DIV 4
#define SUB 5
#define ADD 6
#define MOD 7
#define EOE 8
#define U 9
#define GE 10
#define GT 11
#define LE 12
#define LT 13
#define AND 14
#define OR 15
#define NE 16
#define ASI 17
#define NEG 18
#define COM 19
#define LBRA 20
#define RBRA 21
class Token {
public:
	int type;
	char* value;
	int Priority;
	char* var_integer;
	Token()
	{
		this->type = NULL;
		this->value = NULL;
		this->Priority = NULL;
		this->var_integer = NULL;
	}
	Token(int type_, char* value_, int priority)
	{
		this->type = type_;
		this->value = new char[strlen(value_) + 1];
		this->value = strcpy(value, value_);
		this->Priority = priority;
		this->var_integer = "";
	}
	bool isOperand()
	{
		if (this->type == INT || this->type == VAR) return true;
		else return false;
	}
	int getPriority()
	{
		return this->Priority;
	}
	~Token()
	{
		if (type) {
			//      free(type);
		}
	}
};
class Stack {
public:
	Stack();
	~Stack();
	void push(Token*&);
	Token* pop();
	bool is_empty();
	void clear();
	Token* getPeak();
private:
	class Node {
	public:
		Node(Token*& t, Node* c);
		Token* contain;
		Node* next;
	};
	Node* peak;
};
Stack::Stack()
{
	peak = NULL;
}
Stack::~Stack()
{
	clear();
}
bool Stack::is_empty()
{
	return peak == NULL;
}
void Stack::clear()
{
	while (!is_empty())
		pop();
}
Stack::Node::Node(Token*& t, Node* c) : next(c)
{
	contain = t;
}
void Stack::push(Token*& t)
{
	peak = new Node(t, peak);
	//assert(peak);
}
class QueueToken
{
private:
	struct Node {
		Token* token;
		Node *link;
	};
	Node* root;
	Node* rear;
public:
	QueueToken()
	{
		root = NULL;
		rear = NULL;
	}
	void QueueToken::insert(Token* tok)
	{
		Node *n = new Node();
		n->token = tok;
		n->link = NULL;
		if (root == NULL)
		{
			root = n;
		}
		else
		{
			rear->link = n;
		}
		rear = n;
	}
	Token* QueueToken::enQueue()
	{
		Node *n = new Node();
		Token* ret = new Token();
		if (root == NULL) return new Token();
		else
		{
			n = root;
			ret = n->token;
			root = root->link;
			free(n);
			return ret;
		}
	}
	bool QueueToken::isEmpty()
	{
		if (root == NULL) return true;
		else return false;
	}
};
Token* Stack::pop()
{
	assert(peak);
	Node c(*peak);
	delete peak;
	peak = c.next;
	return c.contain;
}
Token* Stack::getPeak()
{
	Node c(*peak);
	return c.contain;
}
class ListToken
{
private:
	struct Node {
		Token* token;
		Node *next;
	};
	struct Node* GetNewNode(char* name) {
		struct Node* newNode = new Node;
		newNode->token = new Token(VAR, name, NONE);
		newNode->token->var_integer = "Nul";
		newNode->next = NULL;
		return newNode;
	}
	Node* root;
public:
	ListToken()
	{
		root = NULL;
	}
	int ListToken::hash(char* ascii)
	{
		int hash = 0;
		int i = 0;
		while (ascii[i] != NULL)
		{
			hash += ascii[i];
			i++;
		}
		hash = hash % 52;
		return hash;
	}
	Token* ListToken::add(char* name)
	{
		Node* temp = root;
		Node* newNode = GetNewNode(name);
		if (root == NULL)
		{
			root = newNode;
			return newNode->token;
		}
		if (!strcmp(temp->token->value, name))
		{
			return temp->token;
		}
		while (temp->next != NULL)
		{
			if (!strcmp(temp->next->token->value, name)) return temp->next->token;
			temp = temp->next;
		}
		temp->next = newNode;
		return newNode->token;
	}
	Token* ListToken::setInteger(char* name, char* new_interger)
	{
		Node* temp = root;
		if (temp == NULL) return new Token();
		if (!strcmp(temp->token->value, name))
		{
			temp->token->var_integer = new_interger;
			return root->token;
		}
		while (temp->next != NULL)
		{
			if (!strcmp(temp->next->token->value, name))
			{
				temp->next->token->var_integer = new_interger;
				return temp->next->token;
			}
			temp = temp->next;
		}
		return new Token();
	}
	char* ListToken::getfinalValue(char* name)
	{
		Node* temp = root;
		char* ret = NULL;
		if (root == NULL) return "Nul";
		if (!strcmp(temp->token->value, name))
		{
			ret = temp->token->var_integer;
			return ret;
		}
		while (temp->next != NULL)
		{
			if (!strcmp(temp->next->token->value, name))
			{
				ret = temp->next->token->var_integer;
				return ret;
			}
			temp = temp->next;
		}
		return"Nul";
	}
	int strcmp(const char *str1, const char *str2)
	{
		int s1;
		int s2;
		do {
			s1 = *str1++;
			s2 = *str2++;
			if (s1 == 0)
				break;
		} while (s1 == s2);
		return (s1 < s2) ? -1 : (s1 > s2);
	}
};
class Lexer {
private:
	char* text;
	int pos;
	char Curr_Char;
	Token* curr_token;
	bool isChopped;
	int* chop;
	int L_brackets;
	int R_brackets;
	ListToken *list;
public:
	bool if_found;
	bool while_found;
	bool insideScope;
	Lexer()
	{
		this->text = NULL;
		this->pos = 0;
	}
	Lexer(char* text, int *current_pos, ListToken *curr_list)
	{
		this->text = text;
		this->pos = 0;
		this->Curr_Char = text[0];
		this->if_found = false;
		this->while_found = false;
		this->chop = current_pos;
		this->L_brackets = 0;
		this->R_brackets = 0;
		this->list = curr_list;
	}
	int getPos()
	{
		return pos;
	}
	void setPos(int npos)
	{
		this->pos = npos;
	}
	void advance()
	{
		this->pos += 1;
		this->Curr_Char = this->text[this->pos];
	}
	void skip_whitespace()
	{
		while (this->Curr_Char != NULL && isspace(this->Curr_Char))
			this->advance();
	}
	char* integer() {
	}
	void skip_expresion(char* text, int *current_pos)
	{
		this->text = text;
		this->Curr_Char = text[0];
		this->chop = current_pos;
		int left_scope = 1;
		int right_scope = 0;
		while (left_scope != right_scope)
		{
			if (this->Curr_Char == '{') left_scope++;
			if (this->Curr_Char == '}') right_scope++;
			this->advance();
		}
		*chop += pos + 1;
	}
	Token* get_next_token()
	{
		while (this->Curr_Char != NULL) {
			if (isspace(this->Curr_Char)) {
				this->skip_whitespace();
			}
			else break;
		}
		if (this->Curr_Char == '?')
		{
			*chop += pos + 1;
			if_found = true;
			return new Token(EOE, "Nul", NONE);
		}
		if (this->Curr_Char == '@')
		{
			*chop += pos + 1;
			while_found = true;
			return new Token(EOE, "Nul", NONE);
		}
		if (this->Curr_Char == '{')
		{
			*chop += pos + 1;
			return new Token(EOE, "Nul", NONE);
		}
		if (this->Curr_Char == '}')
		{
			insideScope = false;
			*chop += pos + 1;
			return new Token(EOE, "Nul", NONE);
		}
		if (isalpha(this->Curr_Char))
		{
			char* result = (char*)malloc(2 * sizeof(char));
			int count = 0;
			int n = 2;
			if ((curr_token->type == INT || curr_token->type == VAR))
			{
				while (L_brackets != R_brackets)
				{
					this->pos -= 1;
					if (this->pos > 119) Curr_Char = NULL;
					else this->Curr_Char = this->text[this->pos];
					if (Curr_Char == '(')
					{
						L_brackets--;
					}
				}
				isChopped = true;
				*chop += pos;
				return new Token(EOE, "Nul", NONE);
			}
			while (this->Curr_Char != NULL && isalpha(this->Curr_Char))
			{
				count++;
				result[count - 1] = Curr_Char;
				if (count == n)
				{
					n *= 2;
					result = (char*)realloc(result, n * sizeof(char));
				}
				if (result == NULL) {
					free(result);
					exit(1);
				}
				this->advance();
			}
			result[count] = '\0';
			if (result[0] == 'N' && result[1] == 'u' && result[2] == 'l'&& result[3] == '\0') return curr_token = new Token(INT, "Nul", NONE);
			else
			{
				int hash = list->hash(result);
				curr_token = list[hash].add(result);
				return curr_token;
			}
		}
		if (isdigit(this->Curr_Char))
		{
			if ((curr_token->type == INT || curr_token->type == VAR))
			{
				while (L_brackets != R_brackets)
				{
					this->pos -= 1;
					if (this->pos > 119) Curr_Char = NULL;
					else this->Curr_Char = this->text[this->pos];
					if (Curr_Char == '(')
					{
						L_brackets--;
					}
				}
				isChopped = true;
				*chop += pos;
				return new Token(EOE, "Nul", NONE);
			}
			char* result = (char*)malloc(2 * sizeof(char));
			int count = 0;
			int n = 2;
			while (this->Curr_Char != NULL && isdigit(this->Curr_Char))
			{
				count++;
				result[count - 1] = Curr_Char;
				if (count == n)
				{
					n *= 2;
					result = (char*)realloc(result, n * sizeof(char));
				}
				if (result == NULL)
				{
					free(result);
					exit(1);
				}
				this->advance();
			}
			result[count] = '\0';
			return  curr_token = new Token(INT, result, NONE);
		}
		if (this->Curr_Char == '*')
		{
			this->advance();
			return curr_token = new  Token(MUL, "*", 1);;
		}
		if (this->Curr_Char == '*')
		{
			this->advance();
			return curr_token = new  Token(MUL, "*", 1);
		}
		if (this->Curr_Char == '%')
		{
			this->advance();
			return curr_token = new  Token(MOD, "%", 1);
		}
		if (this->Curr_Char == '/')
		{
			this->advance();
			return curr_token = new  Token(DIV, "/", 1);
		}
		if (this->Curr_Char == '+')
		{
			this->advance();
			return curr_token = new  Token(ADD, "+", 2);
		}
		if (this->Curr_Char == '-')
		{
			this->advance();
			if (curr_token->type != VAR && curr_token->type != INT)
			{
				return curr_token = new  Token(U, "-", 0);
			}
			return curr_token = new Token(SUB, "-", 2);
		}
		if (this->Curr_Char == '>')
		{
			this->advance();
			if (Curr_Char == '=')
			{
				this->advance();
				return curr_token = new Token(GE, ">=", 3);
			}
			else return curr_token = new Token(GT, ">", 3);
		}
		if (this->Curr_Char == '<')
		{
			this->advance();
			if (Curr_Char == '=')
			{
				this->advance();
				return curr_token = new Token(LE, "<=", 3);
			}
			else return curr_token = new Token(LT, "<", 3);
		}
		if (this->Curr_Char == '!')
		{
			this->advance();
			if (Curr_Char == '=')
			{
				this->advance();
				return curr_token = new Token(NE, "!=", 4);
			}
			else return curr_token = new Token(NEG, "!", 0);
		}
		if (this->Curr_Char == '=')
		{
			this->advance();
			if (Curr_Char == '=')
			{
				this->advance();
				return curr_token = new Token(COM, "==", 4);
			}
			else return curr_token = new Token(ASI, "=", 7);
		}
		if (this->Curr_Char == '&')
		{
			this->advance();
			return curr_token = new Token(AND, "&", 5);
		}
		if (this->Curr_Char == '|')
		{
			this->advance();
			return curr_token = new Token(OR, "|", 6);
		}
		if (this->Curr_Char == '(')
		{
			if (curr_token->type == VAR || curr_token->type == INT)
			{
				*chop += pos + 1;
				return new Token(EOE, "Nul", NONE);
			}
			this->advance();
			L_brackets++;
			return new Token(LBRA, "(", NONE);
		}
		if (this->Curr_Char == ')')
		{
			this->advance();
			R_brackets++;
			return new Token(RBRA, ")", NONE);
		}
		*chop += pos;
		return new Token(EOE, "Nul", NONE);
	}
	char* getText()
	{
		return text;
	}
	void updateLexer(char* new_text, int *current_pos)
	{
		this->text = new_text;
		this->pos = 0;
		this->Curr_Char = new_text[0];
		this->if_found = false;
		this->while_found = false;
		this->chop = current_pos;
		this->curr_token = new Token();
		this->L_brackets = 0;
		this->R_brackets = 0;
	}
};
class Interpreter
{
public:
	int operations;
	Interpreter(int limit, char* text)
	{
		this->operations = 0;
		this->Op_limit = limit;
		this->text = text;
	}
	void interpret()
	{
		current_pos = 0;
		lexer = Lexer(text, &current_pos, list);
		while (text[current_pos] != '\0')
		{
			lexer.updateLexer(text + current_pos, &current_pos);
			que = ONPCONV(lexer.getText());
			ONPCalc(que);
			if (operations == Op_limit) break;
			if (lexer.if_found == true)
			{
				if_statement();
				if (operations == Op_limit) break;
			}
			if (lexer.while_found == true)
			{
				while_statement();
				if (operations == Op_limit) break;
			}
		}
		printf("%d\n", operations);
	}
	void print_variable(char** var, int var_size)
	{
		int hash = 0;
		for (int i = 0; i <= var_size; i++)
		{
			hash = list[0].hash(var[i]);
			printf("%s %s\n", var[i], list[hash].getfinalValue(var[i]));
		}
	}
private:
	Lexer lexer;
	Token* curr_token;
	char* text;
	int Op_limit;
	int current_pos;
	ListToken list[52];
	QueueToken que;
	void if_statement() {
		operations++;
		lexer.if_found = false;
		lexer.updateLexer(text + current_pos, &current_pos);
		que = ONPCONV(lexer.getText());
		if (operations == Op_limit) return;
		if (strcmp(ONPCalc(que), "Nul") != 0)
		{
			lexer.insideScope = true;
			while (lexer.insideScope)
			{
				if (operations == Op_limit) return;
				lexer.updateLexer(text + current_pos, &current_pos);
				que = ONPCONV(lexer.getText());
				ONPCalc(que);
				if (lexer.if_found == true)
				{
					if_statement();
					lexer.insideScope = true;
				}
				if (lexer.while_found == true)
				{
					while_statement();
					lexer.insideScope = true;
				}
				if (operations == Op_limit) return;
			}
		}
		else
		{
			lexer.updateLexer(text + current_pos, &current_pos);
			lexer.skip_expresion(text + current_pos, &current_pos);
		}
	}
	void while_statement() {
		operations++;
		lexer.while_found = false;
		int get_back = current_pos;
		char* back_text = text + get_back;
		bool loop = false;
		int tmp = 0;
		lexer.updateLexer(text + current_pos, &current_pos);
		QueueToken while_ = ONPCONV(lexer.getText());
		int while_start = current_pos;
		if (operations == Op_limit) return;
		while (strcmp(ONPCalc(while_), "Nul") != 0)
		{
			operations++;
			loop = true;
			current_pos = while_start;
			lexer.insideScope = true;
			while (lexer.insideScope)
			{
				if (operations == Op_limit) return;
				lexer.updateLexer(text + current_pos, &current_pos);
				que = ONPCONV(lexer.getText());
				tmp = current_pos;
				ONPCalc(que);
				if (lexer.if_found == true)
				{
					if_statement();
					lexer.insideScope = true;
				}
				if (lexer.while_found == true)
				{
					while_statement();
					lexer.insideScope = true;
				}
				if (operations == Op_limit) return;
			}
			tmp = current_pos;
			lexer.updateLexer(back_text, &current_pos);
			while_ = ONPCONV(lexer.getText());
			current_pos = get_back;
		}
		if (loop == false)
		{
			lexer.updateLexer(text + current_pos, &current_pos);
			lexer.skip_expresion(text + current_pos, &current_pos);
		}
		else {
			current_pos = tmp;
			lexer.updateLexer(text + tmp, &current_pos);
		}
	}
	QueueToken ONPCONV(char* expresion) {
		Stack stack;
		QueueToken new_queue;
		Token* item = new Token();
		while (item->type != EOE)
		{
			item = lexer.get_next_token();
			if (item->type == EOE)
			{
				break;
			}
			if (item->isOperand())
			{
				new_queue.insert(item);
			}
			else if (item->type == LBRA)
			{
				stack.push(item);
			}
			else if (item->type == RBRA)
			{
				while (!stack.is_empty())
				{
					item = stack.pop();
					if (item->type == LBRA) break;
					else new_queue.insert(item);
				}
			}
			else
			{
				int priority = item->getPriority();
				while (!stack.is_empty() && ((item->type != ASI && item->type != NEG && item->type != U && stack.getPeak()->getPriority() <= item->getPriority())
					|| (item->type == ASI&&  item->type == NEG && item->type == U && stack.getPeak()->getPriority()>item->getPriority())))
				{
					new_queue.insert(stack.pop());
				}
				stack.push(item);
			}
		}
		if (item->type == EOE)
		{
			delete item;
		}
		while (!stack.is_empty())
		{
			item = stack.pop();
			if (item->type == LBRA) break;
			new_queue.insert(item);
		}
		return new_queue;
	}
	char* ONPCalc(QueueToken queue)
	{
		Stack stack;
		Token* item = new Token();
		Token* op1 = new Token();
		Token* op2 = new Token();
		Token* calc = new Token();
		Token* unary = new Token();
		char* result;
		while (!queue.isEmpty())
		{
			item = queue.enQueue();
			if (item->isOperand())
			{
				stack.push(item);
			}
			else if (item->type == NEG || item->type == U)
			{
				if (item->type == U)
				{
					op1 = stack.pop();
					unary = unary_operator(op1);
					stack.push(unary);
					if (operations == Op_limit) break;
				}
				if (item->type == NEG)
				{
					op1 = stack.pop();
					unary = neg_operator(op1);
					stack.push(unary);
					if (operations == Op_limit) break;
				}
				if (operations == Op_limit) break;
			}
			else
			{
				op1 = stack.pop();
				op2 = stack.pop();
				calc = do_calculations(item, op1, op2);
				stack.push(calc);
				if (operations == Op_limit) break;
			}
		}
		if (!stack.is_empty())
		{
			item = stack.pop();
			if (item->type == VAR) return item->var_integer;
			else return item->value;
		}
		return "Nul";
	}
	Token* neg_operator(Token* op1)
	{
		operations++;
		if (op1->type == VAR)
		{
			if (strcmp(op1->var_integer, "Nul") != 0) return new Token(INT, "Nul", NONE);
			else return new Token(INT, "0", NONE);
		}
		else
		{
			if (strcmp(op1->value, "Nul") != 0) return new Token(INT, "Nul", NONE);
			else return new Token(INT, "0", NONE);
		}
	}
	Token* unary_operator(Token* op1)
	{
		Token* temp;
		int new_val = 0;
		int digits = 0;
		if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op1->value, "Nul") == 0) return new Token(INT, "Nul", NONE);
		else
		{
			if (op1->type == VAR) {
				operations++;
				new_val = atoi(op1->var_integer);
			}
			if (op1->type == INT) new_val = atoi(op1->value);
			char* result = NULL;
			new_val = -new_val;
			digits = checkDigits(new_val);
			if (new_val < 0)
			{
				digits = checkDigits(-new_val);
				result = (char*)malloc(digits*sizeof(char) + sizeof(char) + 1);
			}
			else result = (char*)malloc(digits*sizeof(char) + 1);
			itoa(new_val, result, 10);
			temp = new Token(INT, result, NONE);
			if (result)
			{
				free(result);
			}
			return temp;
		}
	}
	Token* do_calculations(Token* item, Token* op1, Token* op2)
	{
		operations++;
		if (op1->type == INT && strcmp(op1->value, "Nul") == 0)
		{
			op1->var_integer = "Nul";
			op1->type = VAR;
		}
		if (op2->type == INT && strcmp(op2->value, "Nul") == 0)
		{
			op2->var_integer = "Nul";
			op2->type = VAR;
		}
		int num1 = atoi(op2->value);
		if (op2->type == VAR) num1 = atoi(op2->var_integer);
		int num2 = atoi(op1->value);
		if (op1->type == VAR) num2 = atoi(op1->var_integer);
		Token* asi = new Token();
		char * result = NULL;
		int digits = 0;
		if (item->type == ASI)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				int hash = list[0].hash(op2->value);
				asi = list[hash].setInteger(op2->value, op1->var_integer);
				return asi;
			}
			if (op2->type == VAR)
			{
				int hash = list[0].hash(op2->value);
				asi = list[hash].setInteger(op2->value, op1->value);
				return asi;
			}
			if (op1->type == VAR)
			{
				int hash = list[0].hash(op1->value);
				asi = list[hash].setInteger(op1->value, op2->value);
				return asi;
			}
		}
		if (item->type == OR)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") != 0 || strcmp(op2->var_integer, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") != 0 || strcmp(op2->value, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op1->value, "Nul") != 0 || strcmp(op2->var_integer, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (strcmp(op1->value, "Nul") != 0 || strcmp(op2->value, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == AND)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") != 0 && strcmp(op2->var_integer, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") != 0 && strcmp(op2->value, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op1->value, "Nul") != 0 && strcmp(op2->var_integer, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (strcmp(op1->value, "Nul") != 0 && strcmp(op2->value, "Nul") != 0) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == NE)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->var_integer) != atoi(op2->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->var_integer) != atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->value) != atoi(op2->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op1->value) != atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == COM)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->var_integer) == atoi(op2->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->var_integer) == atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op1->value) == atoi(op2->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op1->value) == atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == GT)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) > atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->value)>atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer)>atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op2->value)>atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == LT)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) < atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->value) < atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) < atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op2->value) < atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == GE)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) >= atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->value) >= atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) >= atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op2->value) >= atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (item->type == LE)
		{
			if (op1->type == VAR && op2->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) <= atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op1->type == VAR)
			{
				if (strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->value) <= atoi(op1->var_integer)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			if (op2->type == VAR)
			{
				if (strcmp(op2->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
				if (atoi(op2->var_integer) <= atoi(op1->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
			else
			{
				if (atoi(op2->value) <= atoi(op2->value)) return new Token(INT, "0", NONE);
				else return new Token(INT, "Nul", NONE);
			}
		}
		if (strcmp(op1->value, "Nul") == 0 || strcmp(op2->value, "Nul") == 0 || strcmp(op2->var_integer, "Nul") == 0 || strcmp(op1->var_integer, "Nul") == 0) return new Token(INT, "Nul", NONE);
		if (item->type == MUL) {
			int res = num2*num1;
			digits = checkDigits(res);
			result = (char*)malloc(digits*sizeof(char));
			itoa(res, result, 10);
			return new Token(INT, result, NONE);
		}
		if (item->type == DIV)
		{
			if (num2 == 0) return new Token(INT, "Nul", NONE);
			int res = num1 / num2;
			digits = checkDigits(res);
			result = (char*)malloc(digits*sizeof(char));
			itoa(res, result, 10);
			return new Token(INT, result, NONE);
		}
		if (item->type == MOD)
		{
			int res = num1 % num2;
			digits = checkDigits(res);
			result = (char*)malloc(digits*sizeof(char));
			itoa(res, result, 10);
			return new Token(INT, result, NONE);
		}
		if (item->type == ADD)
		{
			int res = num1 + num2;
			digits = checkDigits(res);
			result = (char*)malloc(digits*sizeof(char));
			itoa(res, result, 10);
			return new Token(INT, result, NONE);
		}
		if (item->type == SUB)
		{
			int res = num1 - num2;
			digits = checkDigits(res);
			result = (char*)malloc(digits*sizeof(char));
			itoa(res, result, 10);
			return new Token(INT, result, NONE);
		}
		return new Token();
	}
	int checkDigits(int n)
	{
		if (n < 100000) {
			if (n < 100) {
				if (n < 10)
					return 1;
				else
					return 2;
			}
			else {
				if (n < 1000)
					return 3;
				else {
					if (n < 10000)
						return 4;
					else
						return 5;
				}
			}
		}
		else {
			if (n < 10000000) {
				if (n < 1000000)
					return 6;
				else
					return 7;
			}
			else {
				if (n < 100000000)
					return 8;
				else {
					if (n < 1000000000)
						return 9;
					else
						return 10;
				}
			}
		}
	}
};
int main()
{
	char *text = NULL;
	char **var_to_read = NULL;
	int text_size = 100;
	int var_size = 20;
	text = (char*)malloc(text_size * sizeof(*text));
	var_to_read = (char**)malloc(var_size * sizeof(*var_to_read));
	for (int i = 0; i < var_size; i++)
	{
		var_to_read[i] = (char*)malloc(2 * sizeof(char));
	}
	int Operations_limit = 0;
	char blank = ' ';
	scanf("%d", &Operations_limit);
	scanf("%c", &blank);
	char temp = ' ';
	int k = 0;
	int i = 0;
	int c = 0;
	int mul_var = 2;
	while (temp != '\n')
	{
		scanf("%c", &temp);
		if (temp == '\n') break;
		c++;
		if (i == var_size) {
			var_size *= 2;
			var_to_read = (char**)realloc(var_to_read, var_size*sizeof(*var_to_read));
			for (int k = i; k < var_size; k++)
			{
				var_to_read[k] = (char*)malloc(2 * sizeof(char));
			}
			if (var_to_read == NULL)
			{
				for (int i = 0; i < var_size; i++)
				{
					free(var_to_read[i]);
				}
				free(var_to_read);
				exit(1);
			}
		}
		if (temp == ' ')
		{
			var_to_read[i][c - 1] = '\0';
			i++;
			c = 0;
			mul_var = 2;
		}
		else {
			if (c == mul_var)
			{
				mul_var *= 2;
				var_to_read[i] = (char*)realloc(var_to_read[i], mul_var*sizeof(char));
			}
			if (var_to_read[i] != NULL) {
				var_to_read[i][c - 1] = temp;
			}
			else {
				free(var_to_read[i]);
				exit(1);
			}
		}
	}
	var_to_read[i][c] = '\0';
	if (var_to_read[i][0] == '\0') i--;
	int count = 0;
	int multi = 1;
	while (scanf("%c", &temp) != EOF) {
		if (temp == '\n') temp = ' ';
		text[count] = temp;
		count++;
		if (count == (multi * text_size)) {
			multi *= 2;
			text = (char*)realloc(text, multi*text_size*sizeof(char));
			if (text == NULL) exit(1);
		}
	}
	text[count] = '\0';
	Interpreter interpreter = Interpreter(Operations_limit, text);
	interpreter.interpret();
	interpreter.print_variable(var_to_read, i);
	for (int i = 0; i < var_size; i++)
	{
		free(var_to_read[i]);
	}
	free(var_to_read);
	free(text);
	return 0;
}

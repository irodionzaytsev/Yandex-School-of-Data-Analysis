#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stack>

class Lexema {
public:
    const static int _num = 0;
    const static int _close_bracket = 1;
    const static int _open_bracket = 2;
    const static int _plus = 3;
    const static int _mult = 4;

    int type;
    int value = -1;

    Lexema (const int itype, const int ivalue) :
            type(itype),
            value(ivalue) {}

    Lexema (const int itype) :
            type(itype) {}

    static int checkType(const char symbol) {
        if (symbol == '(') {
            return _open_bracket;
        }
        if (symbol == ')') {
            return _close_bracket;
        }
        if (symbol == '+') {
            return _plus;
        }
        if (symbol == '*') {
            return _mult;
        }
        return -1;
    }

    void print () {
        if (type == _num) {
            std::cerr << value << " ";
        }
        if (type == _plus) {
            std::cerr << "+" << " ";
        }
        if (type == _mult) {
            std::cerr << "*" << " ";
        }
        if (type == _open_bracket) {
            std::cerr << "(" << " ";
        }
        if (type == _close_bracket) {
            std::cerr << ")" << " ";
        }
    }

};

bool isDigit(const char symbol) {
    return (symbol >= '0' && symbol <= '9');
}

bool isArithmeticSymbol (const char symbol) {

    return (isDigit(symbol) || (Lexema::checkType(symbol) != -1));

    if (symbol >= '0' && symbol <= '9') {
        return true;
    }
    if (symbol == '+' || symbol == '*') {
        return true;
    }
    if (symbol == '(' || symbol == ')') {
        return true;
    }
    return false;
}

std::vector<Lexema> beatIntoLexems(const std::string& expression) {
    std::string reduced_expression;
    for (char symbol : expression) {
        if (isArithmeticSymbol(symbol)) {
            reduced_expression.push_back(symbol);
        }
    }
    reduced_expression = "(" + reduced_expression + ")";
    bool num_mod = false;
    int current_number = 0;
    std::vector<Lexema> lexems;
    for (char symbol : reduced_expression) {
        if (isDigit(symbol)) {
            num_mod = true;
            current_number = (10 * current_number + (symbol - '0'));
            continue;
        } else {
            if (num_mod) {
                num_mod = false;
                lexems.push_back(Lexema(Lexema::_num, current_number));
                current_number = 0;
            }
        }
        lexems.push_back(Lexema(Lexema::checkType(symbol)));
    }
    return lexems;
}

class Expression {
public:
    virtual int getValue() {}
    virtual int getType() {}
    virtual void addExprs(std::unique_ptr<Expression> ileft_expr, std::unique_ptr<Expression> iright_expr) {}
    virtual void print() {}
    explicit Expression() {};
    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;
    ~Expression() = default;
};

class Constant : public Expression {
private:
    int value_;
public:
    int getValue() {
        return value_;
    }

    int getType() {
        return Lexema::_num;
    }

    Constant(Lexema lexema):
            value_(lexema.value) {}

    void print () {
        std::cerr << value_ << "\n";
    }

};

class Sum : public Expression {
private:
    std::unique_ptr<Expression> left_expr;
    std::unique_ptr<Expression> right_expr;

public:
    int getValue() {
        return (left_expr->getValue() + right_expr->getValue());
    }
    int getType() {
        return Lexema::_plus;
    }
    void addExprs(std::unique_ptr<Expression> ileft_expr, std::unique_ptr<Expression> iright_expr) {
        left_expr = std::move(ileft_expr);
        right_expr = std::move(iright_expr);
    }
    void print () {
        std::cerr << "+" << "\n";
    }
};

class Product : public Expression {
private:
    std::unique_ptr<Expression> left_expr;
    std::unique_ptr<Expression> right_expr;

public:
    int getValue() {
        return (left_expr->getValue() * right_expr->getValue());
    }
    int getType() {
        return Lexema::_mult;
    }
    void addExprs(std::unique_ptr<Expression> ileft_expr, std::unique_ptr<Expression> iright_expr) {
        left_expr = std::move(ileft_expr);
        right_expr = std::move(iright_expr);
    }
    void print () {
        std::cerr << "*" << "\n";
    }
};

class OpenBracket : public Expression {
public:
    int getType() {
        return Lexema::_open_bracket;
    }
    void print () {
        std::cerr << "(" << "\n";
    }
};

class CloseBracket : public Expression {
public:
    int getType() {
        return Lexema::_close_bracket;
    }
    void print () {
        std::cerr << ")" << "\n";
    }
};

std::unique_ptr<Expression> makeExpr(int type) {
    if (type == Lexema::_mult) {
       return std::move(std::unique_ptr<Expression>(new Product));
    }
    if (type == Lexema::_plus) {
        return std::move(std::unique_ptr<Expression>(new Sum()));
    }
    if (type == Lexema::_open_bracket) {
        return std::move(std::unique_ptr<Expression>(new OpenBracket()));
    }
    if (type == Lexema::_close_bracket) {
        return std::move(std::unique_ptr<Expression>(new CloseBracket()));
    }
    return nullptr;
}

std::unique_ptr<Expression> polskaNotazia(const std::vector<Lexema>& lexems) {

    std::stack<std::unique_ptr<Expression>> numbers_stack;
    std::stack<std::unique_ptr<Expression>> operations_stack;

    for (Lexema lexem : lexems) {
        //std::cerr << "current lexem\n";
        //lexem.print();
        //std::cerr << "\n";
        if (lexem.type == Lexema::_num) {
            numbers_stack.push(std::unique_ptr<Expression>(new Constant(lexem)));
        } else {
            std::unique_ptr<Expression> expr = makeExpr(lexem.type);
            if (lexem.type != Lexema::_open_bracket) {
                while (!operations_stack.empty() && operations_stack.top()->getType() >= expr->getType()) {
                    std::unique_ptr<Expression> currentOperation = std::move(operations_stack.top());
                    operations_stack.pop();
                    //std::cerr << "current Operation\n";
                    //currentOperation->print();
                    if (currentOperation->getType() == Lexema::_open_bracket) {
                        break;
                    }
                    std::unique_ptr<Expression> firstArg = std::move(numbers_stack.top());
                    numbers_stack.pop();
                    std::unique_ptr<Expression> secondArg = std::move(numbers_stack.top());
                    numbers_stack.pop();
                    currentOperation->addExprs(std::move(firstArg), std::move(secondArg));
                    numbers_stack.push(std::move(currentOperation));
                }
            }
            if (expr->getType() != Lexema::_close_bracket) {
                operations_stack.push(std::move(expr));
            }
        }
    }
    return std::move(numbers_stack.top());
}

int main() {
    std::string expression;
    std::getline(std::cin, expression);
    std::vector<Lexema> lexems = beatIntoLexems(expression);
    //for (auto& lexema : lexems) {
    //    lexema.print();
    //}
    std::unique_ptr<Expression> result = polskaNotazia(lexems);
    std::cout << result->getValue();
    return 0;
}

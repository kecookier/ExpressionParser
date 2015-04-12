#ifndef PARSER_H__
#define PARSER_H__

#include <string>
#include <vector>
#include <cassert>
#include <stack>
#include <cstdint>

#define ASSERT(expr) assert(expr)

class TokenBase;
class TokenOperator;
struct Element;

// 输入表达式字符串
// 预处理字符串（解析Token）
// 转成后缀形式
// 转成语法树
// 递归求解

typedef std::vector<TokenBase *> TokenVec;
typedef std::stack<TokenOperator *> TokenStack;
typedef std::stack<Element> CalcStack;

class TokenBase
{
public:
    virtual ~TokenBase() {}
    virtual void FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack) = 0;
    virtual void CalculateToken(TokenVec *rpn_vec, CalcStack *stack) = 0;
};

class TokenVar : public TokenBase
{
public:
    TokenVar(const std::string &name) : var_name_(name) { }
    virtual void FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack);
    virtual void CalculateToken(TokenVec *rpn_vec, CalcStack *stack);
    Element GetRuntimeValue(void *param);
protected:
private:
    std::string var_name_;
};

class TokenConstant : public TokenBase
{
public:
    TokenConstant(float value) : value_(value) { }
    virtual void FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack);
    virtual void CalculateToken(TokenVec *rpn_vec, CalcStack *stack);
private:
    float value_;
};

class TokenOperator : public TokenBase
{
public:
    TokenOperator(char op) : op_(op) { ASSERT(CheckValid()); }
    virtual void FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack);
    virtual void CalculateToken(TokenVec *rpn_vec, CalcStack *stack);
    char GetOp() { return op_; }
private:
    inline bool CheckValid()
    {
        return op_ == '+'
            || op_ == '-'
            || op_ == '*'
            || op_ == '/'
            || op_ == '<'
            || op_ == '>'
            || op_ == '('
            || op_ == ')'
            || op_ == '?'
            || op_ == ':';
    }
    inline int32_t GetOpPriority()
    {
        switch (op_)
        {
        case '+':
        case '-':
            return -6;
        case '*':
        case '/':
            return -5;
        case '<':
        case '>':
            return -8;
        case '(':
        case ')':
            return -2;
        case '?':
        case ':':
            return -15;
        default:
            return -100;
        }
    }
    char op_;
};

class Parser
{
public:
    Parser(const std::string &src);
    ~Parser() { }
    void AnalysisToken(TokenVec *infix_vec);
    void InfixToRpn(const TokenVec &infix_vec, TokenVec *rpn_vec);
    float CalculateRpn();
protected:
private:
    bool Init();
    void DebugVector(const TokenVec &vec);

    std::string expr_str_;
    TokenVec rpn_vec_;
};

struct Element
{
    Element(float f) :float_(f) { }
    Element(bool b) :bool_(b) { }
    bool bool_;
    float float_;
};

inline Element operator+(Element &left, Element &right)
{
    return Element(left.float_ + right.float_);
}

inline Element operator-(Element &left, Element &right)
{
    return Element(left.float_ - right.float_);
}

inline Element operator*(Element &left, Element &right)
{
    return Element(left.float_ * right.float_);
}

inline Element operator/(Element &left, Element &right)
{
    return Element(left.float_ / right.float_);
}

inline Element operator>(Element &left, Element &right)
{
    return Element(left.bool_ > right.bool_);
}

inline Element operator<(Element &left, Element &right)
{
    return Element(left.bool_ < right.bool_);
}
#endif // PARSER_H__

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <stack>
#include "parser.h"

Parser::Parser(const std::string &src) : expr_str_(src) 
{ 
    ASSERT(Init());
}

Parser::~Parser()
{
    for (size_t i = 0; i < infix_vec_.size(); ++i)
    {
        delete infix_vec_.at(i);
        infix_vec_.at(i) = nullptr;
    }
}

void Parser::AnalysisToken(TokenVec *infix_vec)
{
    ASSERT(infix_vec);
    for (size_t i = 0; i < expr_str_.length();)
    {
        if (isalpha(expr_str_.at(i)))   // 变量名
        {
            size_t count = 0;
            for (; count + i < expr_str_.length();++count)
            {
                if (!isalpha(expr_str_.at(i+count))&&expr_str_.at(i+count)!='_')
                {
                    break;
                }
            }
            std::string var_str = expr_str_.substr(i, count);
            infix_vec->push_back(new TokenVar(var_str));
            i += count;
        }
        else if (isdigit(expr_str_.at(i)))  // 常量
        {
            size_t count = 0;
            for (; count + i < expr_str_.length();++count)
            {
                if (!isdigit(expr_str_.at(i+count)) && expr_str_.at(i+count) != '.')
                {
                    break;
                }
            }
            std::string float_str = expr_str_.substr(i, count);
            float value = atof(float_str.c_str());
            infix_vec->push_back(new TokenConstant(value));
            i += count;
        }
        else   // 操作符
        {
            infix_vec->push_back(new TokenOperator(expr_str_.at(i)));
            ++i;
        }
    }
}

void Parser::InfixToRpn(const TokenVec &infix_vec, TokenVec *rpn_vec)
{
    ASSERT(rpn_vec);
    TokenStack token_stack;
    for (size_t i = 0; i < infix_vec.size(); ++i)
    {
        infix_vec.at(i)->FillRpnVec(rpn_vec, &token_stack);
    }
    while (!token_stack.empty())
    {
        rpn_vec->push_back(token_stack.top());
        token_stack.pop();
    }
}

float Parser::CalculateRpn()
{
    CalcStack tmp_stack;
    for (size_t i = 0; i < rpn_vec_.size(); ++i)
    {
        rpn_vec_.at(i)->CalculateToken(&rpn_vec_, &tmp_stack);
    }
    return tmp_stack.top().float_;
}

bool Parser::Init()
{
    AnalysisToken(&infix_vec_);
    InfixToRpn(infix_vec_, &rpn_vec_);
    return true;
}

void Parser::DebugVector(const TokenVec &vec)
{
}



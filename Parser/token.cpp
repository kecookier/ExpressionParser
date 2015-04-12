#include <iostream>
#include "parser.h"


void TokenConstant::FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack)
{
    rpn_vec->push_back(this);
}

void TokenConstant::CalculateToken(TokenVec *rpn_vec, CalcStack *stack)
{
    Element elem(value_);
    stack->push(elem);
}

void TokenVar::FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack)
{
    rpn_vec->push_back(this);
}

void TokenVar::CalculateToken(TokenVec *rpn_vec, CalcStack *stack)
{
    stack->push(GetRuntimeValue(nullptr));
}

Element TokenVar::GetRuntimeValue(void *param)
{
    return Element(0.0f);
}

void TokenOperator::FillRpnVec(TokenVec *rpn_vec, TokenStack *token_stack)
{
    if (op_=='(')
    {
        token_stack->push(this);
    }
    else if (op_ == ')')
    {
        while (!token_stack->empty() && token_stack->top()->GetOp() != '(')
        {
            rpn_vec->push_back(token_stack->top());
            token_stack->pop();
        }
        if (token_stack->empty())
        {
            std::cerr << "FillRpnVec failed, bracket not match!!!" << std::endl;
        }
        else
        {
            token_stack->pop();
        }
    }
    else if (op_ == ':')
    {
        while (!token_stack->empty()
            && token_stack->top()->GetOp() != '('
            && token_stack->top()->GetOp() != '?'
            && GetOpPriority() <= token_stack->top()->GetOpPriority())
        {
            rpn_vec->push_back(token_stack->top());
            token_stack->pop();
        }
        // token_stack->push(this);
    }
    else
    {
        while (!token_stack->empty() 
            && token_stack->top()->GetOp() != '('
            && GetOpPriority() <= token_stack->top()->GetOpPriority())
        {
            rpn_vec->push_back(token_stack->top());
            token_stack->pop();
        }
        token_stack->push(this);
    }
}

void TokenOperator::CalculateToken(TokenVec *rpn_vec, CalcStack *stack)
{
    if (op_ == '+')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left + right);
    }
    else if (op_ == '-')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left - right);
    }
    else if (op_ == '*')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left * right);
    }
    else if (op_ == '/')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left / right);
    }
    else if (op_ == '>')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left > right);
    }
    else if (op_ == '<')
    {
        Element left = stack->top();
        stack->pop();
        Element right = stack->top();
        stack->pop();
        stack->push(left < right);
    }
}





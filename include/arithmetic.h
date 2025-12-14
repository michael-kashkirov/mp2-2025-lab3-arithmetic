#pragma once

#include <string>
#include <vector>
#include <map>
#include "stack.h" 

using StringVector = std::vector<std::string>;

class TPostfix
{
private:
    std::string infixExpression;
    StringVector expressionTokens;
    StringVector postfixTokens;
    std::map<std::string, double> varValues;

    bool IsOperator(const std::string& s) const;
    bool IsFunction(const std::string& s) const;
    int GetOperatorPriority(const std::string& op) const;
    bool opPriorityAssociativityCheck(const std::string& op1, const std::string& op2) const;
    void ConvertToPostfix(); 

public:
    TPostfix() = default;

    bool Parse(const std::string& expression, int& errorPosition, std::string& errorMessage);
    double Calculate();

    StringVector GetPostfixTokens() const { return postfixTokens; }
    StringVector GetVariableNames() const;
    
    void SetVariableValue(const std::string& name, double value) { varValues[name] = value; }
    
    void RemoveVariable(const std::string& name); 
};
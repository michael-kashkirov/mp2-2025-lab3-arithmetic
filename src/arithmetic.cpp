#include "arithmetic.h"
#include <cctype>


bool TPostfix::IsOperator(const std::string& s) const {
    return s == "+" || s == "-" || s == "*" || s == "/" || s == "^";
}

bool TPostfix::IsFunction(const std::string& s) const {
    return s == "sin" || s == "cos" || s == "sqrt";
}

int TPostfix::GetOperatorPriority(const std::string& op) const {
    if (IsFunction(op)) return 4;
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0; 
}

bool TPostfix::opPriorityAssociativityCheck(const std::string& op1, const std::string& op2) const {
    if (op2 == "^") {
        return GetOperatorPriority(op1) > GetOperatorPriority(op2);
    }
    return GetOperatorPriority(op1) >= GetOperatorPriority(op2);
}


bool TPostfix::Parse(const std::string& expression, int& errorPosition, std::string& errorMessage) {
    infixExpression = expression;
    expressionTokens.clear();
    postfixTokens.clear();
    
    std::string processedExpression = "";
    bool expectOperand = true; 

    for (char c : expression) {
        if (std::isspace(c)) continue;

        if (c == '+' || c == '-') {
            if (expectOperand) {
                if (c == '-') {
                    processedExpression += "0-";
                } else {
                    processedExpression += "0+";
                }
            } else {
                processedExpression += c;
                expectOperand = true;
            }
        } else if (c == '(') {
            processedExpression += c;
            expectOperand = true; 
        } else if (c == ')') {
            processedExpression += c;
            expectOperand = false; 
        } else if (c == '*' || c == '/' || c == '^') {
            processedExpression += c;
            expectOperand = true; 
        } else if (std::isdigit(c) || std::isalpha(c) || c == '.') {
            processedExpression += c;
            expectOperand = false; 
        } else {
            errorPosition = processedExpression.length(); 
            errorMessage = "Invalid character found."; 
            return false;
        }
    }
    
    infixExpression = processedExpression; 
    
    TStack<int> bracketStack; 
    
    for (size_t i = 0; i < infixExpression.length(); ++i) {
        char c = infixExpression[i];

        if (std::isspace(c)) continue;

        if (c == '(') {
            bracketStack.Push(i); 
            expressionTokens.push_back("(");
        } 
        
        else if (c == ')') {
            
            if (bracketStack.IsEmpty()) {
                errorPosition = i;
                errorMessage = "Unmatched closing parenthesis."; 
                return false;
            }
            bracketStack.Pop();
            expressionTokens.push_back(")");
        }
       
        else if (IsOperator(std::string(1, c))) {
            expressionTokens.push_back(std::string(1, c));
        }

        else if (std::isdigit(c) || (c == '.' && i + 1 < infixExpression.length() && (std::isdigit(infixExpression[i+1]) || std::isalpha(infixExpression[i+1])))) {
            std::string numStr;
            while (i < infixExpression.length() && (std::isdigit(infixExpression[i]) || infixExpression[i] == '.')) {
                numStr += infixExpression[i];
                i++;
            }
            expressionTokens.push_back(numStr);
            i--; 
        }

        else if (std::isalpha(c)) {
            std::string name;
            while (i < infixExpression.length() && (std::isalnum(infixExpression[i]))) {
                name += infixExpression[i];
                i++;
            }
            
            if (!IsFunction(name) && varValues.find(name) == varValues.end()) {
                 varValues[name] = 0.0; 
            }
            
            expressionTokens.push_back(name);
            i--; 
        }

        else {
            errorPosition = i;
            errorMessage = "Invalid character found."; 
            return false;
        }
    }

    if (!bracketStack.IsEmpty()) {
        errorPosition = bracketStack.Top();
        errorMessage = "Unmatched opening parenthesis."; 
        return false;
    }
    
    ConvertToPostfix();
    return true;
}


void TPostfix::ConvertToPostfix() {
    postfixTokens.clear();
    TStack<std::string> operatorStack; 

    for (const std::string& token : expressionTokens) {
        if (!IsOperator(token) && !IsFunction(token) && token != "(" && token != ")") {
            postfixTokens.push_back(token);
        } else if (token == "(") {
            operatorStack.Push(token);
        } else if (token == ")") {
            while (operatorStack.Top() != "(") {
                postfixTokens.push_back(operatorStack.Pop());
            }
            operatorStack.Pop(); 
            if (!operatorStack.IsEmpty() && IsFunction(operatorStack.Top())) {
                 postfixTokens.push_back(operatorStack.Pop());
            }
        } else {
            while (!operatorStack.IsEmpty() && operatorStack.Top() != "(" && 
                   opPriorityAssociativityCheck(operatorStack.Top(), token)) 
            {
                postfixTokens.push_back(operatorStack.Pop());
            }
            operatorStack.Push(token);
        }
    }

    while (!operatorStack.IsEmpty()) {
        postfixTokens.push_back(operatorStack.Pop());
    }
}


double TPostfix::Calculate() {
    TStack<double> calculationStack;
    
    for (const std::string& token : postfixTokens) {
        if (!IsOperator(token) && !IsFunction(token)) {
            try {
                calculationStack.Push(std::stod(token));
            } 
            catch (const std::invalid_argument&) {
                if (varValues.count(token)) {
                    calculationStack.Push(varValues.at(token));
                } 
                else {
                    throw std::runtime_error("Unknown variable or function: " + token);
                }
            }
        } 
        else if (IsOperator(token)) {
            double operand2, operand1;
            try {
                operand2 = calculationStack.Pop();
                operand1 = calculationStack.Pop();
            } catch (const std::out_of_range&) {
                 throw std::runtime_error("Invalid expression structure or calculation error.");
            } catch (const std::runtime_error&) {
                 throw std::runtime_error("Invalid expression structure or calculation error.");
            }
            
            if (token == "+") calculationStack.Push(operand1 + operand2);
            else if (token == "-") calculationStack.Push(operand1 - operand2);
            else if (token == "*") calculationStack.Push(operand1 * operand2);
            else if (token == "/") {
                if (operand2 == 0) throw std::runtime_error("Division by zero.");
                calculationStack.Push(operand1 / operand2);
            }
            else if (token == "^") {
                calculationStack.Push(std::pow(operand1, operand2));
            }
        } else if (IsFunction(token)) {
            double operand;
            try {
                operand = calculationStack.Pop();
            } catch (const std::out_of_range&) {
                throw std::runtime_error("Invalid expression structure or calculation error.");
            } catch (const std::runtime_error&) {
                throw std::runtime_error("Invalid expression structure or calculation error.");
            }
            
            if (token == "sin") calculationStack.Push(std::sin(operand));
            else if (token == "cos") calculationStack.Push(std::cos(operand));
            else if (token == "sqrt") {
                 if (operand < 0) throw std::runtime_error("Square root of negative number.");
                 calculationStack.Push(std::sqrt(operand));
            }
        }
    }

    if (calculationStack.GetCount() != 1) {
        throw std::runtime_error("Invalid expression structure or calculation error.");
    }
    return calculationStack.Pop();
} 

StringVector TPostfix::GetVariableNames() const {
    StringVector names;
    for (const auto& pair : varValues) {
        if (!IsFunction(pair.first)) { 
             names.push_back(pair.first);
        }
    }
    return names;
}

void TPostfix::RemoveVariable(const std::string& name) {
    varValues.erase(name);
}
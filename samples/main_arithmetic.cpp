#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <limits>
#include <sstream>
#include "arithmetic.h"

using namespace std;

using StringVector = std::vector<std::string>; 

void processExpression(TPostfix& calculator); 
void convertToPostfix(TPostfix& calculator);
void printParseError(const std::string& expression, int pos, const std::string& msg); 

void printParseError(const std::string& expression, int pos, const std::string& msg) {
    cerr << "--- Ошибка анализа ---" << endl;
    cerr << "Выражение: " << expression << endl;
    
    if (msg == "Unmatched opening parenthesis.") {
        cerr << "Ошибка: Несоответствующая открывающая скобка." << endl;
    } else if (msg == "Unmatched closing parenthesis.") {
        cerr << "Ошибка: Несоответствующая закрывающая скобка." << endl;
    } else if (msg == "Invalid character found.") {
        cerr << "Ошибка: Найден недопустимый символ." << endl;
    } else {
        cerr << "Ошибка: " << msg << endl;
    }

    cerr << "Позиция: " << pos << endl;
    
    for(int i = 0; i < pos; ++i) {
        cerr << (isspace(expression[i]) ? expression[i] : ' ');
    }
    cerr << "^" << endl;
    cerr << "-------------------" << endl;
}

void convertToPostfix(TPostfix& calculator) {
    string expression;
    cout << "\nВведите инфиксное выражение для конвертации: ";
    getline(cin, expression);

    if (expression.empty()) {
        cout << "Выражение не введено!" << endl;
        return;
    }

    StringVector allVars = calculator.GetVariableNames();
    for (const auto& var : allVars) {
        if (var != "pi") {
            calculator.RemoveVariable(var);
        }
    }

    int errorPos = -1;
    string errorMessage;
    
    if (!calculator.Parse(expression, errorPos, errorMessage)) {
        printParseError(expression, errorPos, errorMessage);
        return;
    }
    
    cout << "\n--- Конвертация ---" << endl;
    cout << "Инфиксная форма: " << expression << endl;
    cout << "Постфиксная форма: ";
    
    for(const auto& token : calculator.GetPostfixTokens()) {
        cout << token << " ";
    }
    cout << endl;
    cout << "-------------------" << endl;
}


void processExpression(TPostfix& calculator) {
    string expression;
    cout << "\nПоддерживаемые операции: +, -, *, /, ^, sin(), cos(), sqrt()" << endl;
    cout << "Введите выражение: ";
    getline(cin, expression);

    if (expression.empty()) {
        cout << "Выражение не введено!" << endl;
        return;
    }

    StringVector allVars = calculator.GetVariableNames();
    for (const auto& var : allVars) {
        if (var != "pi") {
            calculator.RemoveVariable(var);
        }
    }

    int errorPos = -1;
    string errorMessage;
    
    if (!calculator.Parse(expression, errorPos, errorMessage)) {
        printParseError(expression, errorPos, errorMessage);
        return;
    }

    StringVector vars = calculator.GetVariableNames(); 
    bool vars_found = false;

    for (const auto& var : vars) {
        if (var == "pi") continue; 
        
        vars_found = true;
        double value;
        cout << "Введите значение для переменной '" << var << "': ";
        
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cerr << "Неверный ввод. Присвоено значение 0." << endl;
            value = 0.0; 
        }
        
        calculator.SetVariableValue(var, value);
    }
    
    if (vars_found) {
        cout << "\nЗначения переменных успешно заданы." << endl;
    }
    
    cout << "Токены постфиксной записи: ";
    for(const auto& token : calculator.GetPostfixTokens()) {
        cout << token << " ";
    }
    cout << endl;

    try {
        double result = calculator.Calculate();
        cout << "\n--- РЕЗУЛЬТАТ ---" << endl;
        cout << "Выражение: " << expression << endl;
        cout << "Результат: " << result << endl;
        cout << "----------------" << endl;
    }
    catch (const runtime_error& e) {
        string error_msg = e.what();
        
        if (error_msg.find("Division by zero") != string::npos) {
            cerr << "\nОшибка расчета: Деление на ноль." << endl;
        } else if (error_msg.find("Square root of negative number") != string::npos) {
             cerr << "\nОшибка расчета: Квадратный корень из отрицательного числа." << endl;
        } else if (error_msg.find("Unknown variable or function") != string::npos) {
             cerr << "\nОшибка расчета: Неизвестная переменная или функция." << endl;
        } else if (error_msg.find("Invalid expression structure") != string::npos) {
             cerr << "\nОшибка расчета: Неверная структура выражения или ошибка вычисления." << endl;
        }
        else {
             cerr << "\nОшибка расчета: " << error_msg << endl;
        }
    }
    catch (...) {
        cerr << "\nВ процессе вычислений произошла неизвестная ошибка." << endl;
    }
}


void showMainMenu() {
    cout << "\n--- КАЛЬКУЛЯТОР АРИФМЕТИЧЕСКИХ ВЫРАЖЕНИЙ ---" << endl;
    cout << "1. Ввести и вычислить новое выражение" << endl;
    cout << "2. Конвертировать в постфиксную форму (без вычисления)" << endl;
    cout << "0. Выход" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "Ваш выбор: ";
}

int main() {
    TPostfix calculator; 
    calculator.SetVariableValue("pi", 3.14159265358979323846);

    int choice;
    
    while (true) {
        showMainMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Неверный ввод. Пожалуйста, введите число." << endl;
            continue;
        }
        
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        switch (choice) {
            case 1:
                processExpression(calculator);
                break;
            case 2:
                convertToPostfix(calculator);
                break;
            case 0:
                cout << "Завершение работы программы. До свидания!" << endl;
                return 0;
            default:
                cout << "Неверный выбор. Пожалуйста, выберите пункт из меню (0, 1, или 2)." << endl;
                break;
        }
    }
    return 0;
}
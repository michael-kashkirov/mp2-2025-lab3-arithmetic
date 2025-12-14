#include "gtest.h"
#include "arithmetic.h"
#include <cmath> 
#include <string>
#include <stdexcept>
#include <vector>

TEST(TArithmeticExpressionTest, CanParseValidExpression) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    EXPECT_TRUE(p.Parse("a + 2*b", pos, msg));
}

TEST(TArithmeticExpressionTest, ParseFailsOnUnmatchedOpeningBracket) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    EXPECT_FALSE(p.Parse("(2 + 3", pos, msg));
    EXPECT_STREQ(msg.c_str(), "Unmatched opening parenthesis.");
}

TEST(TArithmeticExpressionTest, ParseFailsOnUnmatchedClosingBracket) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    EXPECT_FALSE(p.Parse("2 + 3)", pos, msg));
    EXPECT_STREQ(msg.c_str(), "Unmatched closing parenthesis.");
}

TEST(TArithmeticExpressionTest, ParseFailsOnInvalidCharacter) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    EXPECT_FALSE(p.Parse("2 @ 3", pos, msg));
    EXPECT_STREQ(msg.c_str(), "Invalid character found.");
}

TEST(TArithmeticExpressionTest, ConvertToPostfixSimpleSum) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("a + b", pos, msg);
    std::vector<std::string> expected = {"a", "b", "+"};
    EXPECT_EQ(p.GetPostfixTokens(), expected);
}

TEST(TArithmeticExpressionTest, ConvertToPostfixWithPriority) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("a + b * c", pos, msg);
    std::vector<std::string> expected = {"a", "b", "c", "*", "+"};
    EXPECT_EQ(p.GetPostfixTokens(), expected);
}

TEST(TArithmeticExpressionTest, ConvertToPostfixWithBrackets) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("(a + b) * c", pos, msg);
    std::vector<std::string> expected = {"a", "b", "+", "c", "*"};
    EXPECT_EQ(p.GetPostfixTokens(), expected);
}

TEST(TArithmeticExpressionTest, ConvertToPostfixWithFunction) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("sin(x) + 2", pos, msg);
    std::vector<std::string> expected = {"x", "sin", "2", "+"};
    EXPECT_EQ(p.GetPostfixTokens(), expected);
}

TEST(TArithmeticExpressionTest, CalculateSimpleExpression) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("10 + 5", pos, msg);
    EXPECT_NEAR(15.0, p.Calculate(), 1e-6);
}

TEST(TArithmeticExpressionTest, CalculateWithBrackets) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("3 * (5 + 2)", pos, msg);
    EXPECT_NEAR(21.0, p.Calculate(), 1e-6);
}

TEST(TArithmeticExpressionTest, CalculateWithVariables) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("a * (b + 1)", pos, msg);
    p.SetVariableValue("a", 3.0);
    p.SetVariableValue("b", 4.0);
    EXPECT_NEAR(15.0, p.Calculate(), 1e-6);
}

TEST(TArithmeticExpressionTest, CalculateWithFunction) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("sin(pi / 2) + 1", pos, msg);
    p.SetVariableValue("pi", 3.14159265358979323846);
    EXPECT_NEAR(2.0, p.Calculate(), 1e-6);
}

TEST(TArithmeticExpressionTest, CalculateThrowsOnDivisionByZero) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    p.Parse("10 / 0", pos, msg);
    EXPECT_THROW(p.Calculate(), std::runtime_error);
}

TEST(TArithmeticExpressionTest, CalculateThrowsOnUnknownVariable) {
    TPostfix p;
    int pos = -1;
    std::string msg;
    
    p.Parse("1 + z", pos, msg); 
    
    p.RemoveVariable("z"); 
    
    EXPECT_THROW(p.Calculate(), std::runtime_error);
}
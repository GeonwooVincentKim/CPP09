/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 22:08:38 by geonwkim          #+#    #+#             */
/*   Updated: 2025/04/18 22:30:59 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

// コンストラクタ
RPN::RPN() {}

// 入力文字列を解析し、逆ポーランド記法の計算を行うコンストラクタ
RPN::RPN(const std::string &inputStr) {
    std::stringstream ss(inputStr);
    std::string token;
    int num;

    // 入力文字列を空白で分割して処理
    while (ss >> token) {
        // 単一桁の数字の場合、スタックにプッシュ
        if (token.length() == 1 && isdigit(token[0])) {
            num = token[0] - '0';
            _stack.push(num);
        }
        // 各演算子に対応する関数を実行
        else if (token == "+") {
            _executeStack(&RPN::_plus);
        } else if (token == "-") {
            _executeStack(&RPN::_minus);
        } else if (token == "*") {
            _executeStack(&RPN::_multiply);
        } else if (token == "/") {
            _executeStack(&RPN::_divide);
        }
        // 無効なトークンの場合、例外をスロー
        else {
            throw RPN::invalidArgument("Error: invalid token => " + token);
        }
    }
}

// コピーコンストラクタ
RPN::RPN(const RPN &other) {
    *this = other;
}

// 代入演算子オーバーロード
RPN &RPN::operator=(const RPN &other) {
    if (this != &other)
        _stack = other._stack;
    return (*this);
}

// デストラクタ
RPN::~RPN() {}

// 例外クラスのコンストラクタ
RPN::invalidArgument::invalidArgument(const std::string& msg) : std::runtime_error(msg) {}
RPN::emptyStack::emptyStack(const std::string& msg) : std::runtime_error(msg) {}
RPN::divisionByZero::divisionByZero(const std::string& msg) : std::runtime_error(msg) {}

// 計算結果を取得
double RPN::getResult() const {
    if (_stack.empty())
        throw RPN::emptyStack("Error: empty stack");
    else if (_stack.size() > 1)
        throw RPN::invalidArgument("Error: too many operands");
    return (_stack.top());
}

// スタックから2つの値を取り出し、指定された演算を実行
void RPN::_executeStack(double (RPN::*f)(double, double)) {
    if (_stack.size() < 2)
        throw RPN::emptyStack("Error: insufficient operands");
    double a = _stack.top(); _stack.pop();
    double b = _stack.top(); _stack.pop();
    _stack.push((this->*f)(a, b));
}

// 加算
double RPN::_plus(double a, double b) {
    return b + a;
}

// 減算
double RPN::_minus(double a, double b) {
    return b - a;
}

// 乗算
double RPN::_multiply(double a, double b) {
    return b * a;
}

// 除算（ゼロ除算のチェックあり）
double RPN::_divide(double a, double b) {
    if (a == 0)
        throw RPN::divisionByZero("Error: division by zero");
    return b / a;
}

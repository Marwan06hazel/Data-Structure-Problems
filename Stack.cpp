#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

float operation(string s) {

    stack <char> operators;
    vector <string> postfix;
    stack <float> values;

    // transforming infix to postfix expression
    for (int i = 0; i < s.length(); i++) {

        // handling spaces
        if ((s[i]) == ' ') continue;

        // handling multiple-digit numbers
        if (isdigit(s[i]) || s[i] == '.') {
            string fullNum = "";
            while (i < s.length() && (isdigit(s[i]) || s[i] == '.')) {
                fullNum += s[i];
                i++;
            }

            postfix.push_back(fullNum);
            i--;
        }

        else if (s[i] == '+' || s[i] == '-') {

            // handling unary minus 
            int prev = i - 1;
            while (prev >= 0 && s[prev] == ' ') prev--; //ignore spaces before it

            if (s[i] == '-' && (i == 0 || s[prev] == '(' || s[prev] == '+' || s[prev] == '-' || s[prev] == '*' || s[prev] == '/')) {
                postfix.push_back("0"); // convert unary minus to 0 -> -3 = 03-
            }

            while (!operators.empty() && (operators.top() == '*' || operators.top() == '/')) {
                string mark = string(1, operators.top());
                postfix.push_back(mark);
                operators.pop();
            }

            while (!operators.empty() && (operators.top() == '+' || operators.top() == '-')) {
                string mark = string(1, operators.top());
                postfix.push_back(mark);
                operators.pop();
            }

            operators.push(s[i]);
        }

        else if (s[i] == '*' || s[i] == '/') {

            while (!operators.empty() && (operators.top() == '*' || operators.top() == '/')) {
                string mark = string(1, operators.top());
                postfix.push_back(mark);
                operators.pop();
            }

            operators.push(s[i]);
        }

        // handling parentheses
        else if (s[i] == '(') {
            operators.push(s[i]);
        }

        else if (s[i] == ')') {

            while (!operators.empty() && operators.top() != '(') {
                string mark = string(1, operators.top());
                postfix.push_back(mark);
                operators.pop();
            }

            if (!operators.empty() && operators.top() == '(') operators.pop();
        }

    }

    // empty the last remaining operator
    while (!operators.empty()) {
        string mark = string(1, operators.top());
        postfix.push_back(mark);
        operators.pop();
    }

    // evaluating the expression
    for (int i = 0; i < postfix.size(); i++) {
        string x = postfix[i];

        if (isdigit(x[0]) || (x.size() > 1 && x[0] == '-' || x[0] == '.')) { values.push(stof(x)); }

        else {
            float b = values.top();
            values.pop();

            float a = values.top();
            values.pop();

            if (x == "+") values.push(a + b);
            else if (x == "-") values.push(a - b);
            else if (x == "*") values.push(a * b);
            else if (x == "/") values.push(a / b);
        }
    }

    return values.top();
}

int main() {
    string expression;
    getline(cin, expression);
    cout << operation(expression) << endl;

    return 0;
}

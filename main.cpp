#include <exception>
#include <print>
#include <vector>
#include <string>
#include <sstream>

using std::vector, std::exception, std::println, std::string;
using std::stringstream;

const size_t ops_len = 4;

// TODO: exponentiation
char ops[ops_len] = {
    '+',
    '-',
    '*',
    '/',
    /*'(',
    ')',*/
};

bool is_op(char c) {
    for (auto o: ops) {
        if (c == o) return true;
    }
    return false;
}

enum ValueType {
    OPERATOR,
    NUMBER,
};

struct RpnNode {
    ValueType type;
    float num;
    char op;
};

class EqSolver : public vector<RpnNode> {
public:
    EqSolver(){};

    EqSolver(const string& str) {
        this->from_string(str);
    }

    RpnNode& operator[](const size_t i) {
        return this->at(i);
    }

    float solve() {
        float result = 0;
        vector<float> stack;
        //println("Solve");
        for (auto& node: *this) {
            switch (node.type) {
                case NUMBER:
                    //println("NUMBER: {}", node.num);
                    stack.push_back(node.num);
                    break;
                case OPERATOR:
                    //println("OPERATOR: {}", node.op);
                    if (stack.empty()) throw exception("Stack empty");
                    float num1 = stack.back();
                    stack.pop_back();
                    if (stack.empty()) throw exception("Stack empty");
                    float num2 = stack.back();
                    stack.pop_back();
                    switch (node.op) {
                        case '+':
                            stack.push_back(num2 + num1);
                            break;
                        case '-':
                            stack.push_back(num2 - num1);
                            break;
                        case '*':
                            stack.push_back(num2 * num1);
                            break;
                        case '/':
                            stack.push_back(num2 / num1);
                            break;
                    }
            }
        }
        if (stack.size() != 1) throw std::exception("Stack fucked up");
        result = stack[0];
        return result;
    }

    void from_string(const string& str) {
        this->clear();
        vector<string> tokens;
        vector<char> shunting_yard;
        split_string(tokens, str);
        for (auto& token: tokens) {
            if (token.size() == 1 && is_op(token[0])) {
                char op = token[0];
                if (shunting_yard.empty()) {
                    shunting_yard.push_back(op);
                } else {
                    while (!shunting_yard.empty()) {
                        char top = shunting_yard.back();

                        bool op_is_high = (op == '*' || op == '/');
                        bool top_is_high = (top == '*' || top == '/');

                        if ((top_is_high && !op_is_high) || (top_is_high == op_is_high)) {
                            this->push_back(RpnNode{.type=OPERATOR, .op = top});
                            shunting_yard.pop_back();
                        } else {
                            break;
                        }
                    }
                    shunting_yard.push_back(op);
                }
            } else {
                this->push_back(RpnNode{.type=NUMBER, .num = stof(token)});
            }
        }
        while (!shunting_yard.empty()) {
            char op = shunting_yard.back();
            this->push_back(RpnNode{.type=OPERATOR, .op = op});
            shunting_yard.pop_back();
        }
    }

private:
    void split_string(vector<string>& tokens, const string& str) {
        stringstream ss(str);
        string token;
        while (ss >> token) tokens.push_back(token);
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        string program_name = argv[0];
        println(stderr, "Useage: \n{} \"[equation]\"", program_name);
        return 1;
    }
    string equation = argv[1];
    EqSolver s(equation);
    float result = s.solve();
    println("{}\n = {}", equation, result);
    return 0;
}

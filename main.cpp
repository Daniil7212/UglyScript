#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include <fstream>
#include <set>
#include <algorithm>
#include <complex>
#include <variant>
#include <unordered_map>
#define endl "\n"

using ll = long long;
using namespace std;

struct elem {
    string type;
    string value;
};

struct vec {
    string type;
    vector <string> value;
};

unordered_map <string, elem> vars; // переменные (название -> значение)
unordered_map <string, vector <pair<string, vector <elem>>>> funcs; // функции (название -> вектор внутренних функций)
unordered_map <string, int> args; // название функции -> кол-во аргументов
set <string> standard; // названия стандартных функций
unordered_map <string, ll> func_line; // строка на которой инициализируется функция
unordered_map <string, vec> vecs; // название переменной -> вектор в ней хранящийся

// инициализация стандартных функций
void init_standard_funcs() {
    args["print"] = 1; standard.insert("print");
    args["println"] = 1; standard.insert("println");
    args["printf"] = 2; standard.insert("printf");
    args["newl"] = 0; standard.insert("newl");
    args["input"] = 1; standard.insert("input");
    args["inputln"] = 1; standard.insert("inputln");
    args["create"] = 2; standard.insert("create");
    args["set"] = 2; standard.insert("set");
    args["delete"] = 1; standard.insert("delete");
    args["swap"] = 2; standard.insert("swap");
    args["vector"] = 2; standard.insert("vector");
    args["to_int"] = 2; standard.insert("to_int");
    args["to_string"] = 2; standard.insert("to_string");
    args["to_bool"] = 2; standard.insert("to_bool");
    args["to_float"] = 2; standard.insert("to_float");
    args["fast_input"] = 0; standard.insert("fast_input");
    args["pass"] = 0; standard.insert("pass");
    args["exit"] = 0; standard.insert("exit");
    args["plus"] = 3; standard.insert("plus");
    args["minus"] = 3; standard.insert("minus");
    args["multiply"] = 3; standard.insert("multiply");
    args["divide"] = 3; standard.insert("divide");
    args["remainder"] = 3; standard.insert("remainder");
    args["abs"] = 2; standard.insert("abs");
    args["equal"] = 3; standard.insert("equal");
    args["<"] = 3; standard.insert("<");
    args["<="] = 3; standard.insert("<=");
    args[">"] = 3; standard.insert(">");
    args[">="] = 3; standard.insert(">=");
    args["and"] = 3; standard.insert("and");
    args["or"] = 3; standard.insert("or");
    args["xor"] = 3; standard.insert("xor");
    args["not"] = 2; standard.insert("not");
    args["if"] = 3; standard.insert("if");
    args["while"] = 2; standard.insert("while");
    args["for"] = 4; standard.insert("for");
    args["foreach"] = 3; standard.insert("foreach");
    args["push"] = 2; standard.insert("push");
    args["pop"] = 1; standard.insert("pop");
    args["size"] = 2; standard.insert("size");
    args["clear"] = 1; standard.insert("clear");
    args["get"] = 3; standard.insert("get");
    args["insert"] = 3; standard.insert("insert");
    args["change"] = 3; standard.insert("change");
    args["erase"] = 2; standard.insert("erase");
    args["reverse"] = 1; standard.insert("reverse");
    args["sort"] = 1; standard.insert("sort");
    args["find"] = 3; standard.insert("find");
}

// проверка существует ли такая переменная
void check_valid_var(string& namef, ll stroke) {
    if (vars.find(namef) == vars.end()) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "NameError: Variable \"" + namef + "\" not found.");
    }
}

// проверка существует ли такая функция
void check_valid_func(string& namef, ll stroke) {
    if (funcs.find(namef) == funcs.end()) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "NameError: Function \"" + namef + "\" not found.");
    }
}

// проверка существует ли такой вектор
void check_valid_vec(string& name, ll stroke) {
    if (vecs.find(name) == vecs.end()) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "NameError: Vector \"" + name + "\" not found.");
    }
}

// проверка индекса
void check_index(ll index, ll size, ll stroke) {
    if (index >= size or index < 0) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "IndexError: Index \"" + to_string(index) + "\" out of range.");
    }
}

// проверка деления на ноль
void check_division_zero(string& n, ll stroke) {
    if (n == "0") {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "SyntaxError: Division by 0.");
    }
}

// проверка существует ли такой тип данных
void check_type_exist(string& type, ll stroke) {
    if (type != "int" and type != "string" and type != "bool" and type != "float") {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "TypeError: Type of data \"" + type + "\" does not exist.");
    }
}

// проверка одинаковы ли типы данных
void check_same_types(elem& a, elem& b, ll stroke) {
    if (a.type != b.type) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "TypeError: \"" + a.value + "\" and \"" + b.value + "\" must have the same data types.");
    }
}

// проверка верный ли тип данных
void check_type(elem& val, string correct_type, ll stroke) {
    if (val.type != correct_type) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "TypeError: \"" + val.value + "\" is " + val.type + " but must be " + correct_type + ".");
    }
}

// проверка верный ли тип данных у переменной
void check_type_var(string& name, string correct_type, ll stroke) {
    if (vars[name].type != correct_type) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "TypeError: Variable \"" + name + "\" is " + vars[name].type + " but must be " + correct_type + ".");
    }
}

// проверка названия переменной
void check_func_name(string& name, ll stroke) {
    for (const auto& i : standard) {
        if (name == i) {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "NameError: Name \"" + name + "\" is not available for the function.");
        }
    }
    if (name[0] >= '0' and name[0] <= '9' or name[0] == '-' or name[0] == '.') {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "NameError: Name \"" + name + "\" is not available for the function.");
    }
}

// проверка названия переменной
void check_var_name(string& name, ll stroke) {
    if (name[0] >= '0' and name[0] <= '9' or name[0] == '-' or name[0] == '.') {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "NameError: Name \"" + name + "\" is not available for the variable.");
    }
}

// ошибка: функция не поддерживает тип
void error_func_wrong_type(string& namef, string type, ll stroke) {
    throw runtime_error("Line: " + to_string(stroke) + "\n" +
        "TypeError: Function \"" + namef + "\" does not support type \"" + type +"\".");
}

// ошибка: должно быть функцией
void error_must_be_func(string& value, ll stroke) {
    throw runtime_error("Line: " + to_string(stroke) + "\n" +
        "SyntaxError: \"" + value + "\" must be function.");
}

// ошибка: должно быть функцией
void error_cannot_convert(string& value, string type, ll stroke) {
    throw runtime_error("Line: " + to_string(stroke) + "\n" +
        "TypeError: Cannot convert \"" + value + "\" to " + type + ".");
}

// ошибка: неправильный тип данных получен из ввода
void error_input_type(string& value, string type, ll stroke) {
    throw runtime_error("Line: " + to_string(stroke) + "\n" +
        "TypeError: Value \"" + value + "\" from the input is not " + type + ".");
}

// ошибка: неизвестный тип данных в аргументах
void error_arg_type(string& value, ll stroke) {
    throw runtime_error("Line: " + to_string(stroke) + "\n" +
        "TypeError: \"" + value + "\" has an unknown data type.");
}

class Interpreter {
public:
    Interpreter() {
        init_standard_funcs();
    }

    void print(string& namef, vector <elem>& func_args, ll stroke) {
        elem out = func_args[0];
        if (out.type == "variable") {
            check_valid_var(out.value, stroke);
            out = vars[out.value];
        }

        if (out.type == "float") {
            for (auto& i : out.value) {
                if (i == ',') {
                    i = '.';
                }
            }
        }
        if (out.type == "vector") {
            error_func_wrong_type(namef, out.type, stroke);
        }

        cout << out.value;
    }

    void println(string& namef, vector <elem>& func_args, ll stroke) {
        print(namef, func_args, stroke);

        cout << endl;
    }

    void printf(string& namef, vector <elem>& func_args, ll stroke) {
        elem out = func_args[0];
        elem n = func_args[1];
        if (out.type == "variable") {
            check_valid_var(out.value, stroke);
            out = vars[out.value];
        }

        if (n.type == "variable") {
            check_valid_var(n.value, stroke);
            n = vars[out.value];
        }

        check_type(n, "int", stroke);

        if (out.type == "float") {
            string s = out.value;
            string ans = "";
            int k = -1;
            for (int i = 0; (i < s.size()) and (k <= stoll(n.value)); i++) {
                if (s[i] == ',') {
                    s[i] = '.';
                }
                if (s[i] == '.') k = 0;
                ans += s[i];
                if (k != -1) k++;
            }
            cout << ans;
        }
        else {
            error_func_wrong_type(namef, out.type, stroke);
        }
    }

    void newl(string& namef, vector <elem>& func_args, ll stroke) {
        cout << endl;
    }

    void input(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);

        string s;
        cin >> s;
        if (vars[var].type == "int") {
            for (int i = 0; i < s.size(); i++) {
                if (!(s[i] >= '0' && s[i] <= '9') and !(s[i] == '-' and i == 0)) {
                    error_input_type(s, "int", stroke);
                }
            }
        }
        else if (vars[var].type == "float") {
            int cnt = 0;
            for (int i = 0; i < s.size(); i++) {
                if (!(s[i] >= '0' && s[i] <= '9')) {
                    if (s[i] == '-') {
                        if (i != 0) {
                            error_input_type(s, "float", stroke);
                        }
                    }
                    else if (s[i] == '.' and cnt == 0) {
                        if (i != 0 and s[i - 1] == '-') {
                            error_input_type(s, "float", stroke);
                        }
                        s[i] = ',';
                        cnt = 1;
                    }
                    else {
                        error_input_type(s, "float", stroke);
                    }
                }
            }
        }
        else if (vars[var].type == "bool") {
            if (s != "true" and s != "false") {
                error_input_type(s, "bool", stroke);
            }
        }
        else if (vars[var].type == "string") {

        }
        else {
            error_func_wrong_type(namef, vars[var].type, stroke);
        }
        vars[var].value = s;
    }

    void inputln(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);

        string s;
        getline(cin, s);
        if (vars[var].type == "string")
        {
            vars[var].value = s;
        }
        else {
            error_func_wrong_type(namef, vars[var].type, stroke);
        }
    }

    void create(string& namef, vector <elem>& func_args, ll stroke) {
        string type = func_args[0].value;
        string name = func_args[1].value;

        check_var_name(namef, stroke);
        if (vars.find(name) != vars.end()) {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "NameError: Variable \"" + name + "\" has already been initialized.");
        }
        check_type_exist(type, stroke);

        vars[name] = { type, "" };
    }

    void set(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem val = func_args[1];

        check_valid_var(var.value, stroke);
        if (val.type == "variable") {
            check_valid_var(val.value, stroke);
            val = vars[val.value];
        }

        check_type(val, vars[var.value].type, stroke);

        if (val.type == "vector") {
            vecs[var.value].value = vecs[val.value].value;
        }
        else {
            vars[var.value].value = val.value;
        }
    }

    void del(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);
        if (var.type == "vector") {
            vecs.erase(var.value);
        }
        vars.erase(var.value);
    }

    void swap(string& namef, vector <elem>& func_args, ll stroke) {
        elem a = func_args[0];
        elem b = func_args[1];

        if (a.type != "variable" or b.type != "variable") {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "SyntaxError: Function \"" + namef + "\" accepts only variables.");
        }

        check_valid_var(a.value, stroke);
        check_valid_var(b.value, stroke);

        check_same_types(vars[a.value], vars[b.value], stroke);

        std::swap(vars[a.value].value, vars[b.value].value);
    }

    void vec(string& namef, vector <elem>& func_args, ll stroke) {
        string type = func_args[0].value;
        string name = func_args[1].value;

        check_var_name(namef, stroke);
        if (vars.find(name) != vars.end()) {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "NameError: Vector \"" + name + "\" has already been initialized.");
        }
        check_type_exist(type, stroke);

        vars[name] = { "vector", name };
        vecs[name].type = type;
        vecs[name].value = {};
    }

    void to_int(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);

        check_type_var(var.value, "int", stroke);

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid_var(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "bool") {
            if (n.value == "true") {
                vars[var.value].value = "1";
            }
            else if (n.value == "false") {
                vars[var.value].value = "0";
            }
        }
        else if (n.type == "float") {
            string s = "";
            for (const auto& i : n.value) {
                if (i == ',') {
                    vars[var.value].value = s;
                    break;
                }
                s += i;
            }
        }
        else if (n.type == "string") {
            for (const auto& i : n.value) {
                if (!(i >= '0' and i <= '9')) {
                    error_cannot_convert(n.value, "int", stroke);
                }
            }

            vars[var.value] = n;
        }
        else {
            error_func_wrong_type(namef, n.type, stroke);
        }
    }

    void to_str(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);

        check_type_var(var.value, "string", stroke);

        elem num = func_args[1], n;
        if (num.type == "variable") {
            check_valid_var(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "vector") {
            error_func_wrong_type(namef, n.type, stroke);
        }

        vars[var.value] = n;
    }

    void to_bool(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);

        check_type_var(var.value, "bool", stroke);

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid_var(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "int") {
            if (n.value == "0") {
                vars[var.value].value = "false";
            }
            else {
                vars[var.value].value = "true";
            }
        }
        else if (n.type == "string") {
            if (n.value == "true" or n.value == "false") {
                vars[var.value].value = n.value;
            }
            else {
                error_cannot_convert(n.value, "bool", stroke);
            }
        }
        else {
            error_func_wrong_type(namef, n.type, stroke);
        }
    }

    void to_float(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);

        check_type_var(var.value, "float", stroke);

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid_var(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "int") {
            vars[var.value].value = n.value + ".0";
        }
        else if (n.type == "string") {
            int cnt = 0;
            for (auto& i : n.value) {
                if (i == '.') {
                    cnt++;
                    i = ',';
                }
                else if (!(i >= '0' and i <= '9')) {
                    error_cannot_convert(n.value, "float", stroke);
                }
            }
            if (cnt != 1) {
                error_cannot_convert(n.value, "float", stroke);
            }

            vars[var.value].value = n.value;
        }
        else {
            error_func_wrong_type(namef, n.type, stroke);
        }
    }

    void fast_input(string& namef, vector <elem>& func_args, ll stroke) {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
    }

    void plus(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (b.type == "variable") {
            check_valid_var(b.value, stroke);

            check_type_var(b.value, type, stroke);

            b = vars[b.value];
        }
        else {
            check_type(b, type, stroke);
        }

        if (type == "string") {
            vars[var].value = a.value + b.value;
        }
        else if (type == "int") {
            vars[var].value = to_string(stoll(a.value) + stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) + stod(b.value));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void minus(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (b.type == "variable") {
            check_valid_var(b.value, stroke);

            check_type_var(b.value, type, stroke);

            b = vars[b.value];
        }
        else {
            check_type(b, type, stroke);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) - stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) - stod(b.value));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void multiply(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (b.type == "variable") {
            check_valid_var(b.value, stroke);

            check_type_var(b.value, type, stroke);

            b = vars[b.value];
        }
        else {
            check_type(b, type, stroke);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) * stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) * stod(b.value));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void divide(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (b.type == "variable") {
            check_valid_var(b.value, stroke);

            check_type_var(b.value, type, stroke);

            b = vars[b.value];
        }
        else {
            check_type(b, type, stroke);
        }

        check_division_zero(b.value, stroke);

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) / stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) / stod(b.value));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void remainder(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (b.type == "variable") {
            check_valid_var(b.value, stroke);

            check_type_var(b.value, type, stroke);

            b = vars[b.value];
        }
        else {
            check_type(b, type, stroke);
        }

        check_division_zero(b.value, stroke);

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) % stoll(b.value));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void abs(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        check_valid_var(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];

        if (type != "int" and type != "float") {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "TypeError: Variable \"" + var + "\" is " + type + " but must be int or float.");
        }

        if (a.type == "variable") {
            check_valid_var(a.value, stroke);

            check_type_var(a.value, type, stroke);

            a = vars[a.value];
        }
        else {
            check_type(a, type, stroke);
        }

        if (type == "int") {
            vars[var].value = to_string(std::abs(stoll(a.value)));
        }
        if (type == "float") {
            vars[var].value = to_string(std::abs(stod(a.value)));
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void equal(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid_var(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid_var(b.value, stroke);

            b.value = vars[b.value].value;
        }

        check_same_types(a, b, stroke);

        if (a.type == "vector") {
            if (vecs[a.value].value == vecs[b.value].value and vecs[a.value].type == vecs[b.value].type) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.value == b.value) {
            vars[var].value = "true";
        }
        else {
            vars[var].value = "false";
        }
    }

    void less(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid_var(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid_var(b.value, stroke);

            b.value = vars[b.value].value;
        }

        check_same_types(a, b, stroke);

        if (a.type == "string") {
            if (a.value < b.value) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "int") {
            if (stoi(a.value) < stoi(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "float") {
            if (stod(a.value) < stod(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "bool") {
            int a1 = 0, a2 = 0;
            a1 = (a.value == "true");
            a2 = (b.value == "true");
            if (a1 < a2) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void less_or_equal(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid_var(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid_var(b.value, stroke);

            b.value = vars[b.value].value;
        }

        check_same_types(a, b, stroke);

        if (a.type == "string") {
            if (a.value <= b.value) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "int") {
            if (stoi(a.value) <= stoi(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "float") {
            if (stod(a.value) <= stod(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "bool") {
            if (a.value == b.value) {
                vars[var].value = "true";
            }
            else {
                int a1 = 0, a2 = 0;
                a1 = (a.value == "true");
                a2 = (b.value == "true");
                if (a1 > a2) {
                    vars[var].value = "true";
                }
                else {
                    vars[var].value = "false";
                }
            }
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void better(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid_var(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid_var(b.value, stroke);

            b.value = vars[b.value].value;
        }

        check_same_types(a, b, stroke);

        if (a.type == "string") {
            if (a.value > b.value) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "int") {
            if (stoi(a.value) > stoi(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "float") {
            if (stod(a.value) > stod(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "bool") {
            int a1 = 0, a2 = 0;
            a1 = (a.value == "true");
            a2 = (b.value == "true");
            if (a1 > a2) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void better_or_equal(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid_var(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid_var(b.value, stroke);

            b.value = vars[b.value].value;
        }

        check_same_types(a, b, stroke);

        if (a.type == "string") {
            if (a.value >= b.value) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "int") {
            if (stoi(a.value) >= stoi(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "float") {
            if (stod(a.value) > stod(b.value)) {
                vars[var].value = "true";
            }
            else {
                vars[var].value = "false";
            }
        }
        else if (a.type == "bool") {
            if (a.value == b.value) {
                vars[var].value = "true";
            }
            else {
                int a1 = 0, a2 = 0;
                a1 = (a.value == "true");
                a2 = (b.value == "true");
                if (a1 >= a2) {
                    vars[var].value = "true";
                }
                else {
                    vars[var].value = "false";
                }
            }
        }
        else {
            error_func_wrong_type(namef, type, stroke);
        }
    }

    void andand(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        check_valid_var(a.value, stroke);
        string aname = a.value;

        check_valid_var(b.value, stroke);
        string bname = b.value;

        check_type_var(aname, "bool", stroke);
        check_type_var(bname, "bool", stroke);

        a = vars[a.value];
        b = vars[b.value];

        if (b.value == "true" and a.value == "true") {
            vars[var].value = "true";
        }
        else {
            vars[var].value = "false";
        }
    }

    void oror(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        check_valid_var(a.value, stroke);
        string aname = a.value;

        check_valid_var(b.value, stroke);
        string bname = b.value;

        check_type_var(aname, "bool", stroke);
        check_type_var(bname, "bool", stroke);

        a = vars[a.value];
        b = vars[b.value];

        if (b.value == "false" and a.value == "false") {
            vars[var].value = "false";
        }
        else {
            vars[var].value = "true";
        }
    }

    void xorxor(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        check_valid_var(a.value, stroke);
        string aname = a.value;

        check_valid_var(b.value, stroke);
        string bname = b.value;

        check_type_var(aname, "bool", stroke);
        check_type_var(bname, "bool", stroke);

        a = vars[a.value];
        b = vars[b.value];

        if (b.value != a.value) {
            vars[var].value = "true";
        }
        else {
            vars[var].value = "false";
        }
    }

    void notnot(string& namef, vector <elem>& func_args, ll stroke) {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];

        check_valid_var(var, stroke);
        check_type_var(var, "bool", stroke);

        check_valid_var(a.value, stroke);
        string aname = a.value;

        check_type_var(aname, "bool", stroke);

        a = vars[a.value];

        if (a.value == "true") {
            vars[var].value = "false";
        }
        else {
            vars[var].value = "true";
        }
    }

    void ifif(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem f = func_args[1];
        elem g = func_args[2];

        check_valid_var(var.value, stroke);
        string value = var.value;

        check_type_var(value, "bool", stroke);

        if (f.type == "variable") {
            if (f.value != "pass") {
                check_valid_func(f.value, stroke);
            }
        }
        else {
            error_must_be_func(f.value, stroke);
        }

        if (g.type == "variable") {
            if (g.value != "pass") {
                check_valid_func(g.value, stroke);
            }
        }
        else {
            error_must_be_func(g.value, stroke);
        }

        if (vars[value].value == "true") {
            if (f.value != "pass") {
                run(f.value, {}, func_line[f.value]);
            }
        }
        else {
            if (g.value != "pass") {
                run(g.value, {}, func_line[g.value]);
            }
        }
    }

    void whileloop(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem f = func_args[1];

        check_valid_var(var.value, stroke);
        string value = var.value;

        check_type_var(value, "bool", stroke);

        if (f.type == "variable") {
            check_valid_func(f.value, stroke);
        }
        else {
            error_must_be_func(f.value, stroke);
        }

        while (vars[value].value == "true") {
            run(f.value, {}, func_line[f.value]);
        }
    }

    void forfor(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem l = func_args[1];
        elem r = func_args[2];
        elem f = func_args[3];

        check_valid_var(var.value, stroke);
        string value = var.value;
        string type = var.type;

        check_valid_var(var.value, stroke);

        if (l.type == "variable") {
            check_valid_var(l.value, stroke);
            check_type_var(l.value, "int", stroke);

            l = vars[l.value];
        }
        else {
            check_type(l, "int", stroke);
        }

        if (r.type == "variable") {
            check_valid_var(r.value, stroke);
            check_type_var(r.value, "int", stroke);

            r = vars[r.value];
        }
        else {
            check_type(r, "int", stroke);
        }

        if (f.type == "variable") {
            check_valid_func(f.value, stroke);
        }
        else {
            error_must_be_func(f.value, stroke);
        }

        check_type_var(value, "int", stroke);

        for (ll i = stoll(l.value); i <= stoll(r.value); i++) {
            vars[value].value = to_string(i);
            run(f.value, {}, func_line[f.value]);
        }
    }

    void foreach(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem vec = func_args[1];
        elem f = func_args[2];

        check_valid_var(var.value, stroke);
        string value = var.value;
        string type = var.type;

        check_valid_var(var.value, stroke);
        check_valid_var(vec.value, stroke);

        check_valid_vec(vec.value, stroke);

        if (f.type == "variable") {
            check_valid_func(f.value, stroke);
        }
        else {
            error_must_be_func(f.value, stroke);
        }

        check_type_var(value, vecs[vec.value].type, stroke);

        for (const auto& i : vecs[vec.value].value) {
            vars[value].value = i;
            run(f.value, {}, func_line[f.value]);
        }
    }

    void push(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem e = func_args[1];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid_var(e.value, stroke);
            e.type = vars[e.value].type;
            e.value = vars[e.value].value;
        }

        check_valid_vec(value, stroke);

        check_type(e, vecs[value].type, stroke);

        (vecs[value].value).push_back(e.value);
    }

    void pop(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (vars[value].type == "string") {
            (vars[value].value).pop_back();
        }
        else
        {
            check_valid_vec(value, stroke);

            (vecs[value].value).pop_back();
        }
    }

    void size(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        check_valid_var(var.value, stroke);

        check_type_var(var.value, "int", stroke);

        elem num = func_args[1], n;
        if (num.type == "variable") {
            check_valid_var(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "string" or n.type == "int") {
            vars[var.value].value = to_string((n.value).size());
        }
        else if (n.type == "vector") {
            vars[var.value].value = to_string((vecs[n.value].value).size());
        }
        else {
            error_func_wrong_type(namef, n.type, stroke);
        }
    }

    void clear(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (vars[value].type == "string") {
            string s = vars[value].value;
            s.clear();
            vars[value].value = s;
        }
        else {
            check_valid_vec(value, stroke);

            vector <string> v = vecs[value].value;
            v.clear();
            vecs[value].value = v;
        }
    }

    void get(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem vec = func_args[1];
        elem index = func_args[2];

        check_valid_var(var.value, stroke);
        check_valid_var(vec.value, stroke);
        vec = vars[vec.value];

        if (index.type == "variable") {
            check_valid_var(index.value, stroke);
            index = vars[index.value];
        }
        check_type(index, "int", stroke);

        if (vec.type == "string") {
            check_index(stoll(index.value), vec.value.size(), stroke);

            vars[var.value].value = vec.value[stoll(index.value)];
        }
        else {
            check_valid_vec(vec.value, stroke);

            check_index(stoll(index.value), vecs[vec.value].value.size(), stroke);

            vars[var.value].value = (vecs[vec.value].value)[stoll(index.value)];
        }
    }

    void insert(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem ind = func_args[1];
        elem e = func_args[2];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid_var(e.value, stroke);
            e = vars[e.value];
        }

        if (ind.type == "variable") {
            check_valid_var(ind.value, stroke);
            ind = vars[ind.value];
        }

        check_type(ind, "int", stroke);

        if (vars[value].type == "string") {
            check_type(e, "string", stroke);

            check_index(stoll(ind.value), vars[value].value.size(), stroke);

            (vars[value].value).insert(stoll(ind.value), e.value);
        }
        else {
            check_valid_vec(value, stroke);

            check_type(e, vecs[value].type, stroke);

            check_index(stoll(ind.value), vecs[value].value.size(), stroke);

            (vecs[value].value).insert(vecs[value].value.begin() + stoll(ind.value), e.value);
        }
    }

    void change(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem ind = func_args[1];
        elem e = func_args[2];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid_var(e.value, stroke);
            e = vars[e.value];
        }

        if (ind.type == "variable") {
            check_valid_var(ind.value, stroke);
            ind = vars[ind.value];
        }

        check_type(ind, "int", stroke);

        check_valid_vec(value, stroke);

        check_type(e, vecs[value].type, stroke);

        check_index(stoll(ind.value), vecs[value].value.size(), stroke);

        vecs[value].value[stoll(ind.value)] = e.value;
    }

    void erase(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem ind = func_args[1];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (ind.type == "variable") {
            check_valid_var(ind.value, stroke);
            ind = vars[ind.value];
        }

        check_type(ind, "int", stroke);

        if (vars[value].type == "string") {
            check_index(stoll(ind.value), vars[value].value.size(), stroke);

            (vars[value].value).erase(vars[value].value.begin() + stoll(ind.value));
        }
        else {
            check_valid_vec(value, stroke);

            check_index(stoll(ind.value), vecs[value].value.size(), stroke);

            (vecs[value].value).erase(vecs[value].value.begin() + stoll(ind.value));
        }
    }

    void reverse(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (vars[value].type == "string") {
            string s = vars[value].value;
            std::reverse(s.begin(), s.end());
            vars[value].value = s;
        }
        else {
            check_valid_vec(value, stroke);

            vector <string> v = vecs[value].value;
            std::reverse(v.begin(), v.end());
            vecs[value].value = v;
        }
    }

    void sort(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];

        check_valid_var(var.value, stroke);
        string value = var.value;

        if (vars[value].type == "string") {
            string s = vars[value].value;
            std::sort(s.begin(), s.end());
            vars[value].value = s;
        }
        else {
            check_valid_vec(value, stroke);

            if (vecs[value].type == "int")
            {
                vector <string> v;
                vector <ll> nn;
                for (const auto& i : vecs[value].value) {
                    nn.push_back(stoll(i));
                }
                std::sort(nn.begin(), nn.end());
                for (const auto& i : nn) {
                    v.push_back(to_string(i));
                }
                vecs[value].value = v;
            }
            else if (vecs[value].type == "float") {
                vector <string> v;
                vector <ll> nn;
                for (const auto& i : vecs[value].value) {
                    nn.push_back(stod(i));
                }
                std::sort(nn.begin(), nn.end());
                for (const auto& i : nn) {
                    v.push_back(to_string(i));
                }
                vecs[value].value = v;
            }
            else {
                vector <string> v = vecs[value].value;
                std::sort(v.begin(), v.end());
                vecs[value].value = v;
            }
        }
    }

    void find(string& namef, vector <elem>& func_args, ll stroke) {
        elem var = func_args[0];
        elem vec = func_args[1];
        elem el = func_args[2];

        check_valid_var(var.value, stroke);
        check_valid_var(vec.value, stroke);
        vec = vars[vec.value];

        if (el.type == "variable") {
            check_valid_var(el.value, stroke);
            el = vars[el.value];
        }

        check_type_var(var.value, "int", stroke);

        if (vec.type == "string") {
            check_type(el, "string", stroke);

            auto pos = vec.value.find(el.value);
            if (pos != string::npos) {
                vars[var.value].value = to_string(pos);
            }
            else {
                vars[var.value].value = "-1";
            }
        }
        else {
            check_valid_vec(vec.value, stroke);
            ::vec vect = vecs[vec.value];

            check_type(el, vect.type, stroke);

            auto pos = std::find(vect.value.begin(), vect.value.end(), el.value);
            if (pos != vect.value.end()) {
                vars[var.value].value = std::to_string(pos - vect.value.begin());
            }
            else {
                vars[var.value].value = "-1";
            }
        }
    }

    // Запуск функций
    void run(string namef, vector <elem> func_args, ll stroke) {

        if (args[namef] != func_args.size()) {
            throw runtime_error("Line: " + to_string(stroke) + "\n" +
                "ArgumentError: Function \"" + namef + "\" takes " + to_string(args[namef]) + " arguments but was given " + to_string(func_args.size()) + ".");
        }

        if (namef == "print") {
            print(namef, func_args, stroke);
        }
        else if (namef == "println") {
            println(namef, func_args, stroke);
        }
        else if (namef == "printf") {
            printf(namef, func_args, stroke);
        }
        else if (namef == "newl") {
            newl(namef, func_args, stroke);
        }
        else if (namef == "input") {
            input(namef, func_args, stroke);
        }
        else if (namef == "inputln") {
            inputln(namef, func_args, stroke);
        }
        else if (namef == "create") {
            create(namef, func_args, stroke);
        }
        else if (namef == "set") {
            set(namef, func_args, stroke);
        }
        else if (namef == "delete") {
            del(namef, func_args, stroke);
        }
        else if (namef == "swap") {
            swap(namef, func_args, stroke);
        }
        else if (namef == "vector") {
            vec(namef, func_args, stroke);
        }
        else if (namef == "to_int") {
            to_int(namef, func_args, stroke);
        }
        else if (namef == "to_string") {
            to_str(namef, func_args, stroke);
        }
        else if (namef == "to_bool") {
            to_bool(namef, func_args, stroke);
        }
        else if (namef == "to_float") {
            to_float(namef, func_args, stroke);
        }
        else if (namef == "fast_input") {
            fast_input(namef, func_args, stroke);
        }
        else if (namef == "pass") {

        }
        else if (namef == "exit") {
            exit(0);
        }
        else if (namef == "plus") {
            plus(namef, func_args, stroke);
        }
        else if (namef == "minus") {
            minus(namef, func_args, stroke);
        }
        else if (namef == "multiply") {
            multiply(namef, func_args, stroke);
        }
        else if (namef == "divide") {
            divide(namef, func_args, stroke);
        }
        else if (namef == "remainder") {
            remainder(namef, func_args, stroke);
        }
        else if (namef == "abs") {
            abs(namef, func_args, stroke);
        }
        else if (namef == "equal") {
            equal(namef, func_args, stroke);
        }
        else if (namef == "<") {
            less(namef, func_args, stroke);
        }
        else if (namef == "<=") {
            less_or_equal(namef, func_args, stroke);
        }
        else if (namef == ">") {
            better(namef, func_args, stroke);
        }
        else if (namef == ">=") {
            better_or_equal(namef, func_args, stroke);
        }
        else if (namef == "and") {
            andand(namef, func_args, stroke);
        }
        else if (namef == "or") {
            oror(namef, func_args, stroke);
        }
        else if (namef == "xor") {
            xorxor(namef, func_args, stroke);
        }
        else if (namef == "not") {
            notnot(namef, func_args, stroke);
        }
        else if (namef == "if") {
            ifif(namef, func_args, stroke);
        }
        else if (namef == "while") {
            whileloop(namef, func_args, stroke);
        }
        else if (namef == "for") {
            forfor(namef, func_args, stroke);
        }
        else if (namef == "foreach") {
            foreach(namef, func_args, stroke);
        }
        else if (namef == "push") {
            push(namef, func_args, stroke);
        }
        else if (namef == "pop") {
            pop(namef, func_args, stroke);
        }
        else if (namef == "size") {
            size(namef, func_args, stroke);
        }
        else if (namef == "clear") {
            clear(namef, func_args, stroke);
        }
        else if (namef == "get") {
            get(namef, func_args, stroke);
        }
        else if (namef == "insert") {
            insert(namef, func_args, stroke);
        }
        else if (namef == "change") {
            change(namef, func_args, stroke);
        }
        else if (namef == "erase") {
            erase(namef, func_args, stroke);
        }
        else if (namef == "reverse") {
            reverse(namef, func_args, stroke);
        }
        else if (namef == "sort") {
            sort(namef, func_args, stroke);
        }
        else if (namef == "find") {
            find(namef, func_args, stroke);
        }
        else {
            ll i = 1;
            for (auto& e : funcs[namef]) {
                if (standard.find(e.first) != standard.end()) {
                    run(e.first, e.second, stroke + i);
                }
                else {
                    run(e.first, e.second, func_line[e.first]);
                }
                i++;
            }
        }
    }
};

// Превращает строку в вектор аргументов
vector <elem> string_to_args(string& str, ll stroke) {
    if (str == "") {
        return {};
    }

    str += " ";
    vector <elem> ans;
    string type = "", value = "";
    bool is_string = false;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ' and !is_string) {
            if (type == "variable") {
                if (value == "true" or value == "false") {
                    ans.push_back({"bool", value});
                }
                else {
                    type = "int";
                    for (char k : value) {
                        if (!(k >= '0' and k <= '9') and k != '.' and k != '-') {
                            type = "variable";
                            break;
                        }
                    }
                    if (type == "int") {
                        int pnt = 0;
                        for (int k = 0; k < value.size(); k++) {
                            if (value[k] == '-' and k != 0) {
                                error_arg_type(value, stroke);
                            }
                            if (value[k] == '.') {
                                if (k != 0) {
                                    pnt = 1;
                                }
                                else {
                                    error_arg_type(value, stroke);
                                }
                            }
                        }
                        if (pnt == 1) {
                            type = "float";
                        }
                    }
                }
            }

            ans.push_back({type, value});

            type = "";
            value = "";
        }
        else if (type == "") {
            if (str[i] == '"') {
                type = "string";
                is_string = true;
            }
            else {
                type = "variable";
                value += str[i];
            }
        }
        else if (str[i] == '"') {
            is_string = false;
        }
        else {
            value += str[i];
        }
    }
    if (is_string) {
        throw runtime_error("Line: " + to_string(stroke) + "\n" +
            "SyntaxError: The \" was not closed");
    }
    return ans;
}

int main() {
    setlocale(LC_ALL, "");
    ifstream fin("input.txt");

    Interpreter interpreter;
    bool is_func = false;
    string namef;
    ll stroke = 0;
    while (true) {
        stroke++;
        string s;
        getline(fin, s);

        if (s == "") continue;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != ' ') {
                s = s.substr(i, s.size());
                break;
            }
            else if (i == s.size() - 1)
            {
                s = "";
            }
        }
        if (s == "") continue;

        vector <string> str;
        string el = "";
        for (const auto& i : (s + " ")) {
            if (i == ' ' and el != "") {
                str.push_back(el);
                el = "";
            }
            else {
                el += i;
            }
        }

        if (str[0] == "#") {
            continue;
        }

        if (str[0] == "func") {
            if (is_func == true) {
                throw runtime_error("Line: " + to_string(stroke) + "\n" +
                    "SyntaxError: You can't make a function inside a function.");
            }

            is_func = true;
            namef = str[1];
            check_func_name(namef, stroke);
            args[namef] = 0;
            func_line[namef] = stroke;
            continue;
        }

        if (str[0] == "endf") {
            is_func = false;
        }

        if (is_func) {
            if (args.find(str[0]) == args.end()) {
                throw runtime_error("Line: " + to_string(stroke) + "\n" +
                    "NameError: Function \"" + str[0] + "\" not found.");
            }
            string args_str;
            for (int i = 0; i < str.size(); i++) {
                if (i != 0) {
                    if (i == str.size() - 1) {
                        args_str += str[i];
                    }
                    else {
                        args_str += str[i] + " ";
                    }
                }
            }
            vector <elem> f_args = string_to_args(args_str, stroke);
            funcs[namef].emplace_back(str[0], f_args);
        }

        if (fin.eof()) {
            break;
        }
    }
    fin.close();

    if (is_func) {
        throw runtime_error("SyntaxError: Functions must have a \"endf\".");
    }
    if (func_line.find("main") == func_line.end()) {
        throw runtime_error("SyntaxError: Program must have a \"main\" function.");
    }

    interpreter.run("main", {}, func_line["main"]);
}

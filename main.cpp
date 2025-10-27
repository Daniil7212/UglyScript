#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include <fstream>
#include <set>
#define ll long long
#define endl "\n"

using namespace std;

struct elem {
    string type;
    string value;
};

struct vec {
    string type;
    vector <string> value;
};

map <string, elem> vars; // переменные (название -> значение)
map <string, vector <pair<string, vector <elem>>>> funcs; // функции (название -> вектор внутренних функций)
map <string, int> args; // название функции -> кол-во аргументов
set <string> standard; // названия стандартных функций
map <string, ll> func_line; // строка на которой инициализируется функция
map <string, vec> vecs; // название переменной -> вектор в ней хранящийся

// инициализация стандартных функций
void init_standard_funcs() {
    args["print"] = 1; standard.insert("print");
    args["println"] = 1; standard.insert("println");
    args["newl"] = 0; standard.insert("newl");
    args["input"] = 1; standard.insert("input");
    args["create"] = 2; standard.insert("create");
    args["set"] = 2; standard.insert("set");
    args["delete"] = 1; standard.insert("delete");
    args["vector"] = 2; standard.insert("vector");
    args["to_int"] = 2; standard.insert("to_int");
    args["to_string"] = 2; standard.insert("to_string");
    args["to_bool"] = 2; standard.insert("to_bool");
    args["to_float"] = 2; standard.insert("to_float");
    args["size"] = 2; standard.insert("size");
    args["pass"] = 0; standard.insert("pass");
    args["exit"] = 0; standard.insert("exit");
    args["plus"] = 3; standard.insert("plus");
    args["minus"] = 3; standard.insert("minus");
    args["multiply"] = 3; standard.insert("multiply");
    args["divide"] = 3; standard.insert("divide");
    args["remainder"] = 3; standard.insert("remainder");
    args["equal"] = 3; standard.insert("equal");
    args["not"] = 2; standard.insert("not");
    args["<"] = 3; standard.insert("<");
    args["<="] = 3; standard.insert("<=");
    args[">"] = 3; standard.insert(">");
    args[">="] = 3; standard.insert(">=");
    args["and"] = 3; standard.insert("and");
    args["or"] = 3; standard.insert("or");
    args["if"] = 3; standard.insert("if");
    args["while"] = 2; standard.insert("while");
    args["push"] = 2; standard.insert("push");
    args["pop"] = 1; standard.insert("pop");
    args["get"] = 3; standard.insert("get");
    args["insert"] = 3; standard.insert("insert");
    args["change"] = 3; standard.insert("change");
    args["erase"] = 2; standard.insert("erase");
}

// проверка существует ли такая переменная
void check_valid(string var, ll stroke) {
    if (vars.find(var) == vars.end()) {
        cout << "Line: " << stroke << endl;
        cout << "NameError: Variable \"" << var << "\" not found." << endl;
        exit(0);
    }
}

// Запуск функций
void run(string namef, vector <elem> func_args, ll stroke) {
    if (args[namef] != func_args.size()) {
        cout << "Line: " << stroke << endl;
        cout << "ArgumentError: Function \"" << namef << "\" takes " << args[namef] << " arguments but was given " << func_args.size() << "." << endl;
        exit(0);
    }

    if (namef == "print") {
        elem out = func_args[0];
        if (out.type == "variable") {
            check_valid(out.value, stroke);
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
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: Function \"" << namef << "\" does not support \"" << out.type <<"\"." << endl;
            exit(0);
        }

        cout << out.value;
    }
    else if (namef == "println") {
        elem out = func_args[0];
        if (out.type == "variable") {
            check_valid(out.value, stroke);
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
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: Function \"" << namef << "\" does not support \"" << out.type <<"\"." << endl;
            exit(0);
        }

        cout << out.value << endl;
    }
    else if (namef == "newl") {
        cout << endl;
    }
    else if (namef == "input") {
        string var = func_args[0].value;
        check_valid(var, stroke);

        string s;
        cin >> s;
        if (vars[var].type == "int") {
            for (auto i : s) {
                if (!(i >= '0' && i <= '9')) {
                    cout << "Line: " << stroke << endl;
                    cout << "TypeError: \"" << s << "\" is not int." << endl;
                    exit(0);
                }
            }
        }
        else if (vars[var].type == "float") {
            int cnt = 0;
            for (auto& i : s) {
                if (!(i >= '0' && i <= '9')) {
                    if (i != '.') {
                        cout << "Line: " << stroke << endl;
                        cout << "TypeError: \"" << s << "\" is not float." << endl;
                        exit(0);
                    }
                    else {
                        i = ',';
                        cnt++;
                    }
                }
            }
            if (cnt != 1) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: \"" << s << "\" is not float." << endl;
                exit(0);
            }
        }
        else if (vars[var].type == "bool") {
            if (s != "true" and s != "false") {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: \"" << s << "\" is not bool." << endl;
                exit(0);
            }
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: Function \"" << namef << "\" does not support \"" << vars[var].type <<"\"." << endl;
            exit(0);
        }
        vars[var].value = s;
    }
    else if (namef == "create") {
        string type = func_args[0].value;
        string name = func_args[1].value;
        if (vars.find(name) != vars.end()) {
            cout << "Line: " << stroke << endl;
            cout << "NameError: Variable \"" << name << "\" has already been initialized." << endl;
            exit(0);
        }
        if (type != "int" and type != "string" and type != "bool" and type != "float") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Type of data \"" << type << "\" does not exist." << endl;
            exit(0);
        }
        vars[name] = { type, "" };
    }
    else if (namef == "set") {
        elem var = func_args[0];
        elem val = func_args[1];

        check_valid(var.value, stroke);
        if (val.type == "variable") {
            check_valid(val.value, stroke);
            val = vars[val.value];
        }
        if (val.type != vars[var.value].type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << val.value << "\" is " << val.type << " but must be " << vars[var.value].type << "." << endl;
            exit(0);
        }

        if (val.type == "vector") {
            vecs[var.value] = vecs[val.value];
        }
        else {
            vars[var.value].value = val.value;
        }
    }
    else if (namef == "delete") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (var.type == "vector") {
            vecs.erase(var.value);
        }
        vars.erase(var.value);
    }
    else if (namef == "vector") {
        string type = func_args[0].value;
        string name = func_args[1].value;
        if (vars.find(name) != vars.end()) {
            cout << "Line: " << stroke << endl;
            cout << "NameError: Variable \"" << name << "\" has already been initialized." << endl;
            exit(0);
        }
        if (type != "int" and type != "string" and type != "bool" and type != "float") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Type of data \"" << type << "\" does not exist." << endl;
            exit(0);
        }
        vars[name] = { "vector", name };
        vecs[name].type = type;
        vecs[name].value = {};
    }
    else if (namef == "to_int") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (vars[var.value].type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be int." << endl;
            exit(0);
        }

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid(num.value, stroke);
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
            for (auto i : n.value) {
                if (i == ',') {
                    vars[var.value].value = s;
                    break;
                }
                s += i;
            }
        }
        else if (n.type == "string") {
            for (auto i : n.value) {
                if (!(i >= '0' and i <= '9')) {
                    cout << "Line: " << stroke << endl;
                    cout << "TypeError: Cannot convert \"" << n.value << "\" to int." << endl;
                    exit(0);
                }
            }

            vars[var.value] = n;
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << n.type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "to_string") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (vars[var.value].type != "string") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be string." << endl;
            exit(0);
        }

        elem num = func_args[1], n;
        if (num.type == "variable") {
            check_valid(num.value, stroke);
            n = vars[num.value];
        }
        else {
            n = num;
        }

        if (n.type == "vector") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << n.type << "\"." << endl;
            exit(0);
        }

        vars[var.value] = n;
    }
    else if (namef == "to_bool") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (vars[var.value].type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be bool." << endl;
            exit(0);
        }

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid(num.value, stroke);
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
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Cannot convert \"" << n.value << "\" to bool." << endl;
                exit(0);
            }
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << n.type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "to_float") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (vars[var.value].type != "float") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be float." << endl;
            exit(0);
        }

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            check_valid(num.value, stroke);
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
                    cout << "Line: " << stroke << endl;
                    cout << "TypeError: Cannot convert \"" << n.value << "\" to float." << endl;
                    exit(0);
                }
            }
            if (cnt != 1) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Cannot convert \"" << n.value << "\" to float." << endl;
                exit(0);
            }

            vars[var.value].value = n.value;
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << n.type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "size") {
        elem var = func_args[0];
        check_valid(var.value, stroke);
        if (vars[var.value].type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be int." << endl;
            exit(0);
        }

        elem num = func_args[1], n;
        if (num.type == "variable") {
            check_valid(num.value, stroke);
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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << n.type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "pass") {
        
    }
    else if (namef == "exit") {
        exit(0);
    }
    else if (namef == "plus") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "minus") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) - stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) - stod(b.value));
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "multiply") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) * stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) * stod(b.value));
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "divide") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.value == "0") {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: Division by 0." << endl;
            exit(0);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) / stoll(b.value));
        }
        else if (type == "float") {
            vars[var].value = to_string(stod(a.value) / stod(b.value));
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "remainder") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        string type = vars[var].type;

        elem a = func_args[1];
        elem b = func_args[2];

        if (type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be int." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.value == "0") {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: Division by 0." << endl;
            exit(0);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) % stoll(b.value));
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "equal") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);

            b.value = vars[b.value].value;
        }

        if (a.type != b.type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" and \"" << b.value << "\" must have the same data types." << type << "." << endl;
            exit(0);
        }

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
    else if (namef == "not") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }

        if (a.type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" must be bool." << type << "." << endl;
            exit(0);
        }

        if (a.value == "true") {
            vars[var].value = "false";
        }
        else {
            vars[var].value = "true";
        }
    }
    else if (namef == "<") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);

            b.value = vars[b.value].value;
        }

        if (a.type != b.type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" and \"" << b.value << "\" must have the same data types." << type << "." << endl;
            exit(0);
        }

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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "<=") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);

            b.value = vars[b.value].value;
        }

        if (a.type != b.type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" and \"" << b.value << "\" must have the same data types." << type << "." << endl;
            exit(0);
        }

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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == ">") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);

            b.value = vars[b.value].value;
        }

        if (a.type != b.type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" and \"" << b.value << "\" must have the same data types." << type << "." << endl;
            exit(0);
        }

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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == ">=") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            check_valid(a.value, stroke);

            a.value = vars[a.value].value;
        }
        if (b.type == "variable") {
            b.type = vars[b.value].type;

            check_valid(b.value, stroke);

            b.value = vars[b.value].value;
        }

        if (a.type != b.type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" and \"" << b.value << "\" must have the same data types." << type << "." << endl;
            exit(0);
        }

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
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Function \"" << namef << "\" does not support \"" << type << "\"." << endl;
            exit(0);
        }
    }
    else if (namef == "and") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];
        string atype;
        string avalue;
        string btype;
        string bvalue;

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            atype = vars[a.value].type;

            check_valid(a.value, stroke);

            avalue = vars[a.value].value;
        }
        if (b.type == "variable") {
            btype = vars[b.value].type;

            check_valid(b.value, stroke);

            bvalue = vars[b.value].value;
        }

        if (atype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" must be bool." << endl;
            exit(0);
        }
        if (btype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << b.value << "\" must be bool." << endl;
            exit(0);
        }

        if (bvalue == "true" and avalue == "true") {
            vars[var].value = "true";
        }
        else {
            vars[var].value = "false";
        }
    }
    else if (namef == "or") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];
        string atype;
        string avalue;
        string btype;
        string bvalue;

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            atype = vars[a.value].type;

            check_valid(a.value, stroke);

            avalue = vars[a.value].value;
        }
        if (b.type == "variable") {
            btype = vars[b.value].type;

            check_valid(b.value, stroke);

            bvalue = vars[b.value].value;
        }

        if (atype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" must be bool." << endl;
            exit(0);
        }
        if (btype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << b.value << "\" must be bool." << endl;
            exit(0);
        }

        if (bvalue == "false" and avalue == "false") {
            vars[var].value = "false";
        }
        else {
            vars[var].value = "true";
        }
    }
    else if (namef == "xor") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];
        string atype;
        string avalue;
        string btype;
        string bvalue;

        check_valid(var, stroke);
        if (type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            atype = vars[a.value].type;

            check_valid(a.value, stroke);

            avalue = vars[a.value].value;
        }
        if (b.type == "variable") {
            btype = vars[b.value].type;

            check_valid(b.value, stroke);

            bvalue = vars[b.value].value;
        }

        if (atype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << a.value << "\" must be bool." << endl;
            exit(0);
        }
        if (btype != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << b.value << "\" must be bool." << endl;
            exit(0);
        }

        if (bvalue == avalue) {
            vars[var].value = "false";
        }
        else {
            vars[var].value = "true";
        }
    }
    else if (namef == "if") {
        elem var = func_args[0];
        elem f = func_args[1];
        elem g = func_args[2];

        check_valid(var.value, stroke);
        string value = var.value;
        string type = var.type;


        if (vars[value].type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << value << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (f.type == "variable") {
            if (funcs.find(f.value) == funcs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Function \"" << f.value << "\" not found." << endl;
                exit(0);
            }
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: \"" << f.value << "\" must be function." << endl;
            exit(0);
        }

        if (g.type == "variable") {
            if (funcs.find(g.value) == funcs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Function \"" << g.value << "\" not found." << endl;
                exit(0);
            }
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: \"" << g.value << "\" must be function." << endl;
            exit(0);
        }

        if (vars[value].value == "true") {
            run(f.value, {}, func_line[f.value]);
        }
        else {
            run(g.value, {}, func_line[g.value]);
        }
    }
    else if (namef == "while") {
        elem var = func_args[0];
        elem f = func_args[1];

        check_valid(var.value, stroke);
        string value = var.value;
        string type = var.type;


        if (vars[value].type != "bool") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: Variable \"" << value << "\" is " << type << " but must be bool." << endl;
            exit(0);
        }

        if (f.type == "variable") {
            if (funcs.find(f.value) == funcs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Function \"" << f.value << "\" not found." << endl;
                exit(0);
            }
        }
        else {
            cout << "Line: " << stroke << endl;
            cout << "SyntaxError: \"" << f.value << "\" must be function." << endl;
            exit(0);
        }

        while (vars[value].value == "true") {
            run(f.value, {}, func_line[f.value]);
        }
    }
    else if (namef == "push") {
        elem var = func_args[0];
        elem e = func_args[1];

        check_valid(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid(e.value, stroke);
            e.type = vars[e.value].type;
            e.value = vars[e.value].value;
        }

        if (vecs.find(value) == vecs.end()) {
            cout << "Line: " << stroke << endl;
            cout << "NameError: Vector \"" << value << "\" not found." << endl;
            exit(0);
        }

        if (e.type != vecs[value].type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << e.value << "\" is " << e.type << " but must be "<< vecs[value].type << "." << endl;
            exit(0);
        }

        (vecs[value].value).push_back(e.value);
    }
    else if (namef == "pop")
    {
        elem var = func_args[0];

        check_valid(var.value, stroke);
        string value = var.value;

        if (vars[value].type == "string") {
            (vars[value].value).pop_back();
        }
        else
        {
            if (vecs.find(value) == vecs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Vector \"" << value << "\" not found." << endl;
                exit(0);
            }

            (vecs[value].value).pop_back();
        }
    }
    else if (namef == "get") {
        elem var = func_args[0];
        elem vec = func_args[1];
        elem index = func_args[2];

        check_valid(var.value, stroke);
        check_valid(vec.value, stroke);
        vec = vars[vec.value];

        if (index.type == "variable") {
            check_valid(index.value, stroke);
            index = vars[index.value];
        }
        if (index.type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << index.value << "\" is " << index.type << " but must be int." << endl;
            exit(0);
        }

        if (vec.type == "string") {
            if (stoll(index.value) >= vec.value.size() or stoll(index.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << index.value << "\" out of range." << endl;
                exit(0);
            }

            vars[var.value].value = vec.value[stoll(index.value)];
        }
        else {
            if (vecs.find(vec.value) == vecs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Vector \"" << vec.value << "\" not found." << endl;
                exit(0);
            }

            if (stoll(index.value) >= vecs[vec.value].value.size() or stoll(index.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << index.value << "\" out of range." << endl;
                exit(0);
            }

            vars[var.value].value = (vecs[vec.value].value)[stoll(index.value)];
        }
    }
    else if (namef == "insert") {
        elem var = func_args[0];
        elem ind = func_args[1];
        elem e = func_args[2];

        check_valid(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid(e.value, stroke);
            e = vars[e.value];
        }

        if (ind.type == "variable") {
            check_valid(ind.value, stroke);
            ind = vars[ind.value];
        }

        if (ind.type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << ind.value << "\" is " << ind.type << " but must be int." << endl;
            exit(0);
        }

        if (vars[value].type == "string") {
            if (e.type != "string") {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: \"" << e.value << "\" is " << e.type << " but must be string." << endl;
                exit(0);
            }

            if (stoll(ind.value) >= vars[value].value.size() or stoll(ind.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << ind.value << "\" out of range." << endl;
                exit(0);
            }

            (vars[value].value).insert(stoll(ind.value), e.value);
        }
        else {
            if (vecs.find(value) == vecs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Vector \"" << value << "\" not found." << endl;
                exit(0);
            }

            if (e.type != vecs[value].type) {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: \"" << e.value << "\" is " << e.type << " but must be "<< vecs[value].type << "." << endl;
                exit(0);
            }

            if (stoll(ind.value) >= vecs[value].value.size() or stoll(ind.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << ind.value << "\" out of range." << endl;
                exit(0);
            }

            (vecs[value].value).insert(vecs[value].value.begin() + stoll(ind.value), e.value);
        }
    }
    else if (namef == "change") {
        elem var = func_args[0];
        elem ind = func_args[1];
        elem e = func_args[2];

        check_valid(var.value, stroke);
        string value = var.value;

        if (e.type == "variable") {
            check_valid(e.value, stroke);
            e = vars[e.value];
        }

        if (ind.type == "variable") {
            check_valid(ind.value, stroke);
            ind = vars[ind.value];
        }

        if (ind.type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << ind.value << "\" is " << ind.type << " but must be int." << endl;
            exit(0);
        }

        if (vecs.find(value) == vecs.end()) {
            cout << "Line: " << stroke << endl;
            cout << "NameError: Vector \"" << value << "\" not found." << endl;
            exit(0);
        }

        if (e.type != vecs[value].type) {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << e.value << "\" is " << e.type << " but must be "<< vecs[value].type << "." << endl;
            exit(0);
        }

        if (stoll(ind.value) >= vecs[value].value.size() or stoll(ind.value) < 0) {
            cout << "Line: " << stroke << endl;
            cout << "IndexError: Index \"" << ind.value << "\" out of range." << endl;
            exit(0);
        }

        vecs[value].value[stoll(ind.value)] = e.value;
    }
    else if (namef == "erase") {
        elem var = func_args[0];
        elem ind = func_args[1];

        check_valid(var.value, stroke);
        string value = var.value;

        if (ind.type == "variable") {
            check_valid(ind.value, stroke);
            ind = vars[ind.value];
        }

        if (ind.type != "int") {
            cout << "Line: " << stroke << endl;
            cout << "TypeError: \"" << ind.value << "\" is " << ind.type << " but must be int." << endl;
            exit(0);
        }

        if (vars[value].type == "string") {
            if (stoll(ind.value) >= vars[value].value.size() or stoll(ind.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << ind.value << "\" out of range." << endl;
                exit(0);
            }

            (vars[value].value).erase(vars[value].value.begin() + stoll(ind.value));
        }
        else {
            if (vecs.find(value) == vecs.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Vector \"" << value << "\" not found." << endl;
                exit(0);
            }

            if (stoll(ind.value) >= vecs[value].value.size() or stoll(ind.value) < 0) {
                cout << "Line: " << stroke << endl;
                cout << "IndexError: Index \"" << ind.value << "\" out of range." << endl;
                exit(0);
            }

            (vecs[value].value).erase(vecs[value].value.begin() + stoll(ind.value));
        }
    }
    else {
        ll i = 1;
        for (auto e : funcs[namef]) {
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

// Превращает строку в вектор аргументов
vector <elem> string_to_args(string str, ll stroke) {
    if (str == "") {
        return {};
    }

    str += " ";
    vector <elem> ans;
    string type = "", value = "";
    bool is_string = false;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ' and !is_string) {
            if (type == "variable" and (value == "true" or value == "false")) {
                ans.push_back({ "bool", value });
            }
            else if (type == "int") {
                int cnt = 0;
                for (auto& k : value) {
                    if (!((k >= '0' and k <= '9') or k == '.')) {
                        cout << "Line: " << stroke << endl;
                        cout << "TypeError: Incorrect argument format." << endl;
                        exit(0);
                    }
                    if (k == '.') {
                        cnt++;
                        k = ',';
                    }
                }
                if (cnt == 0) {
                    ans.push_back({ "int", value });
                }
                else if (cnt == 1) {
                    ans.push_back({ "float", value });
                }
                else {
                    cout << "Line: " << stroke << endl;
                    cout << "TypeError: Incorrect argument format." << endl;
                    exit(0);
                }

            }
            else {
                ans.push_back({ type, value });
            }

            type = "";
            value = "";
        }
        else if (type == "") {
            if (str[i] == '"') {
                type = "string";
                is_string = true;
            }
            else if (str[i] >= '0' and str[i] <= '9') {
                type = "int";
                value += str[i];
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
        cout << "Line: " << stroke << endl;
        cout << "SyntaxError: The \" was not closed";
        exit(0);
    }
    return ans;
}

int main() {
    setlocale(LC_ALL, "");
    ifstream fin("input.txt");

    init_standard_funcs();
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
        for (auto i : (s + " ")) {
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
                cout << "Line: " << stroke << endl;
                cout << "SyntaxError: You can't make a function inside a function.";
                exit(0);
            }

            is_func = true;
            namef = str[1];
            args[namef] = 0;
            func_line[namef] = stroke;
            continue;
        }

        if (str[0] == "endf") {
            is_func = false;
        }

        if (is_func) {
            if (args.find(str[0]) == args.end()) {
                cout << "Line: " << stroke << endl;
                cout << "NameError: Function \"" << str[0] << "\" not found.";
                return 0;
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
        cout << "SyntaxError: Functions must have a \"endf\"." << endl;
        return 0;
    }
    if (func_line.find("main") == func_line.end()) {
        cout << "SyntaxError: Program must have a \"main\" function." << endl;
        return 0;
    }

    run("main", {}, func_line["main"]);
}

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include <fstream>
#define ll long long
#define endl "\n"

using namespace std;

struct elem {
    string type;
    string value;
};

map <string, elem> vars;
map <string, vector <pair<string, vector <elem>>>> funcs;
map <string, vector <ll>> strokes;
map <string, int> args;

void init_common_functions() {
    args["print"] = 1;
    args["println"] = 1;
    args["input"] = 1;
    args["create"] = 2;
    args["set"] = 2;
    args["delete"] = 1;
    args["to_int"] = 2;
    args["to_string"] = 2;
    args["plus"] = 3;
    args["minus"] = 3;
    args["multiply"] = 3;
    args["divide"] = 3;
    args["remainder"] = 3;
}

void run(string namef, vector <elem> func_args, ll stroke) {
    if (args[namef] != func_args.size()) {
        cout << "Stroke: " << stroke << endl;
        cout << "ArgumentError: Function \"" << namef << "\" takes " << args[namef] << " arguments but was given " << func_args.size() << "." << endl;
        exit(0);
    }

    if (namef == "print") {
        string out = func_args[0].value;
        if (func_args[0].type == "variable") {
            if (vars.find(out) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << out << "\" not found." << endl;
                exit(0);
            }
            out = vars[out].value;
        }
        cout << out;
    }
    else if (namef == "println") {
        string out = func_args[0].value;
        if (func_args[0].type == "variable") {
            if (vars.find(out) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << out << "\" not found." << endl;
                exit(0);
            }
            out = vars[out].value;
        }
        cout << out << endl;
    }
    else if (namef == "input") {
        string var = func_args[0].value;
        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        cin >> vars[var].value;
    }
    else if (namef == "create") {
        string type = func_args[0].value;
        string name = func_args[1].value;
        if (type != "int" and type != "string") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Type of data \"" << type << "\" does not exist." << endl;
            exit(0);
        }
        vars[name] = { type, "" };
    }
    else if (namef == "set") {
        elem var = func_args[0];
        elem val = func_args[1];
        if (vars.find(var.value) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var.value << "\" not found." << endl;
            exit(0);
        }
        if (func_args[1].type == "variable") {
            if (vars.find(val.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << val.value << "\" not found." << endl;
                exit(0);
            }
            val = vars[val.value];
        }
        if (val.type != vars[var.value].type) {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: \"" << val.value << "\" is " << val.type << " but must be " << vars[var.value].type << "." << endl;
            exit(0);
        }
        vars[var.value].value = val.value;
    }
    else if (namef == "delete") {
        string var = func_args[0].value;
        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        vars.erase(var);
    }
    else if (namef == "to_int") {
        elem var = func_args[0];
        if (vars.find(var.value) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var.value << "\" not found." << endl;
            exit(0);
        }
        if (vars[var.value].type != "int") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be int." << endl;
            exit(0);
        }

        elem num = func_args[1];
        elem n;
        if (num.type == "variable") {
            if (vars.find(num.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << num.value << "\" not found." << endl;
                exit(0);
            }
            n = vars[num.value];
        }
        else {
            n = num;
        }

        for (auto i : n.value) {
            if (!(i >= '0' and i <= '9')) {
                cout << "Stroke: " << stroke << endl;
                cout << "SyntaxError: Cannot convert \"" << n.value << "\" to int." << endl;
                exit(0);
            }
        }
        vars[var.value] = n;
    }
    else if (namef == "to_string") {
        elem var = func_args[0];
        if (vars.find(var.value) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var.value << "\" not found." << endl;
            exit(0);
        }
        if (vars[var.value].type != "string") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var.value << "\" is " << vars[var.value].type << " but must be string." << endl;
            exit(0);
        }

        elem num = func_args[1], n;
        if (num.type == "variable") {
            if (vars.find(num.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << num.value << "\" not found." << endl;
                exit(0);
            }
            n = vars[num.value];
        }
        else {
            n = num;
        }

        vars[var.value] = n;
    }
    else if (namef == "plus") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            if (vars.find(a.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << a.value << "\" not found." << endl;
                exit(0);
            }
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            if (vars.find(b.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << b.value << "\" not found." << endl;
                exit(0);
            }
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
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
    }
    else if (namef == "minus") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        if (type != "int") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be int." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            if (vars.find(a.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << a.value << "\" not found." << endl;
                exit(0);
            }
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            if (vars.find(b.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << b.value << "\" not found." << endl;
                exit(0);
            }
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) - stoll(b.value));
        }
    }
    else if (namef == "multiply") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        if (type != "int") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be int." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            if (vars.find(a.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << a.value << "\" not found." << endl;
                exit(0);
            }
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            if (vars.find(b.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << b.value << "\" not found." << endl;
                exit(0);
            }
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) * stoll(b.value));
        }
    }
    else if (namef == "divide") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        if (type != "int") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be int." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            if (vars.find(a.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << a.value << "\" not found." << endl;
                exit(0);
            }
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            if (vars.find(b.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << b.value << "\" not found." << endl;
                exit(0);
            }
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.value == "0") {
            cout << "Stroke: " << stroke << endl;
            cout << "SyntaxError: Division by 0." << endl;
            exit(0);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) / stoll(b.value));
        }
    }
    else if (namef == "remainder") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        if (vars.find(var) == vars.end()) {
            cout << "Stroke: " << stroke << endl;
            cout << "NameError: Variable \"" << var << "\" not found." << endl;
            exit(0);
        }
        if (type != "int") {
            cout << "Stroke: " << stroke << endl;
            cout << "TypeError: Variable \"" << var << "\" is " << type << " but must be int." << endl;
            exit(0);
        }

        if (a.type == "variable") {
            a.type = vars[a.value].type;

            if (vars.find(a.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << a.value << "\" not found." << endl;
                exit(0);
            }
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << a.value << "\" is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }

            a.value = vars[a.value].value;
        }
        else {
            if (a.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << a.value << " is " << a.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.type == "variable") {
            b.type = vars[b.value].type;

            if (vars.find(b.value) == vars.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Variable \"" << b.value << "\" not found." << endl;
                exit(0);
            }
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: Variable \"" << b.value << "\" is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }

            b.value = vars[b.value].value;
        }
        else {
            if (b.type != type) {
                cout << "Stroke: " << stroke << endl;
                cout << "TypeError: " << b.value << " is " << b.type << " but must be " << type << "." << endl;
                exit(0);
            }
        }

        if (b.value == "0") {
            cout << "Stroke: " << stroke << endl;
            cout << "SyntaxError: Division by 0." << endl;
            exit(0);
        }

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) % stoll(b.value));
        }
    }
    else {
        ll i = 0;
        for (auto [name, arguments] : funcs[namef]) {
            run(name, arguments, strokes[namef][i]);
            i++;
        }
    }
}

vector <elem> string_to_args(string str, ll stroke) {
    str += " ";
    vector <elem> ans;
    string type = "", value = "";
    bool is_string = false;
    for (int i = 0; i < str.size(); i++) {
        if (i == 0) continue;
        if (str[i] == ' ' and !is_string) {
            ans.push_back({ type, value });
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
        cout << "Stroke: " << stroke << endl;
        cout << "SyntaxError: The \" was not closed";
        exit(0);
    }
    return ans;
}

int main() {
    setlocale(LC_ALL, "");
    ifstream fin("input.txt");

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init_common_functions();
    bool is_func = false;
    string namef;
    ll stroke = 0;
    ll main_stroke = -1;
    while (true) {
        string str;
        fin >> str;
        stroke++;

        if (fin.eof()) {
            break;
        }

        if (str == "#") {
            string comment;
            getline(fin, comment);
            continue;
        }

        if (str == "func") {
            if (is_func == true) {
                cout << "Stroke: " << stroke << endl;
                cout << "SyntaxError: You can't make a function inside a function.";
                exit(0);
            }

            is_func = true;
            fin >> namef;
            args[namef] = 0;
            if (namef == "main") {
                main_stroke = stroke;
            }
            continue;
        }

        if (str == "endf") {
            is_func = false;
            continue;
        }

        if (is_func) {
            if (args.find(str) == args.end()) {
                cout << "Stroke: " << stroke << endl;
                cout << "NameError: Function \"" << str << "\" not found.";
                return 0;
            }
            string args_str;
            getline(fin, args_str);
            vector <elem> f_args = string_to_args(args_str, stroke);
            funcs[namef].emplace_back( str, f_args );
            strokes[namef].push_back(stroke);
        }
    }
    fin.close();

    if (main_stroke == -1) {
        cout << "SyntaxError: Program must have the function \"main\"" << endl;
        return 0;
    }
    run("main", {}, main_stroke);
}

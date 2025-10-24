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

void init_count_args() {
    args["print"] = 1;
    args["println"] = 1;
    args["newl"] = 0;
    args["input"] = 1;
    args["create"] = 2;
    args["set"] = 2;
    args["delete"] = 1;
    args["to_int"] = 2;
    args["to_string"] = 2;
    args["size"] = 2;
    args["pass"] = 0;
    args["plus"] = 3;
    args["minus"] = 3;
    args["multiply"] = 3;
    args["divide"] = 3;
    args["remainder"] = 3;
    args["equal"] = 3;
    args["not"] = 2;
    args["<"] = 3;
    args["<="] = 3;
    args[">"] = 3;
    args[">="] = 3;
}

bool check_valid(string var, ll stroke) {
    if (vars.find(var) == vars.end()) {
        cout << "Line: " << stroke << endl;
        cout << "NameError: Variable \"" << var << "\" not found." << endl;
        exit(0);
    }
}

void run(string namef, vector <elem> func_args, ll stroke) {
    if (args[namef] != func_args.size()) {
        cout << "Line: " << stroke << endl;
        cout << "ArgumentError: Function \"" << namef << "\" takes " << args[namef] << " arguments but was given " << func_args.size() << "." << endl;
        exit(0);
    }

    if (namef == "print") {
        string out = func_args[0].value;
        if (func_args[0].type == "variable") {
            check_valid(out, stroke);
            out = vars[out].value;
        }
        cout << out;
    }
    else if (namef == "println") {
        string out = func_args[0].value;
        if (func_args[0].type == "variable") {
            check_valid(out, stroke);
            out = vars[out].value;
        }
        cout << out << endl;
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
        if (vars[var].type == "bool") {
            if (s != "true" and s != "false") {
                cout << "Line: " << stroke << endl;
                cout << "TypeError: \"" << s << "\" is not bool." << endl;
                exit(0);
            }
        }
        vars[var].value = s;
    }
    else if (namef == "create") {
        string type = func_args[0].value;
        string name = func_args[1].value;
        if (type != "int" and type != "string" and type != "bool") {
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
        vars[var.value].value = val.value;
    }
    else if (namef == "delete") {
        string var = func_args[0].value;
        check_valid(var, stroke);
        vars.erase(var);
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
        else {
            for (auto i : n.value) {
                if (!(i >= '0' and i <= '9')) {
                    cout << "Line: " << stroke << endl;
                    cout << "TypeError: Cannot convert \"" << n.value << "\" to int." << endl;
                    exit(0);
                }
            }

            vars[var.value] = n;
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

        vars[var.value] = n;
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
    }
    else if (namef == "pass") {
        
    }
    else if (namef == "plus") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);

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
    }
    else if (namef == "minus") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
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

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) - stoll(b.value));
        }
    }
    else if (namef == "multiply") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
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

        if (type == "int") {
            vars[var].value = to_string(stoll(a.value) * stoll(b.value));
        }
    }
    else if (namef == "divide") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
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
            vars[var].value = to_string(stoll(a.value) / stoll(b.value));
        }
    }
    else if (namef == "remainder") {
        string var = func_args[0].value;
        string type = vars[var].type;
        elem a = func_args[1];
        elem b = func_args[2];

        check_valid(var, stroke);
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

        if (a.value == b.value) {
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
    }
    else {
        ll i = 0;
        for (auto e : funcs[namef]) {
            run(e.first, e.second, strokes[namef][i]);
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
            if (type == "variable" and (value == "true" or value == "false")) {
                ans.push_back({ "bool", value });
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

    ios_base::sync_with_stdio(false);
    fin.tie(nullptr);
    cin.tie(nullptr);
    cout.tie(nullptr);

    init_count_args();
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
                cout << "Line: " << stroke << endl;
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
                cout << "Line: " << stroke << endl;
                cout << "NameError: Function \"" << str << "\" not found.";
                return 0;
            }
            string args_str;
            getline(fin, args_str);
            vector <elem> f_args = string_to_args(args_str, stroke);
            funcs[namef].emplace_back(str, f_args);
            strokes[namef].push_back(stroke);
        }
    }
    fin.close();

    if (is_func) {
        cout << "SyntaxError: Functions must have a \"endf\"." << endl;
        return 0;
    }
    if (main_stroke == -1) {
        cout << "SyntaxError: Program must have a \"main\" function." << endl;
        return 0;
    }
    run("main", {}, main_stroke);
}

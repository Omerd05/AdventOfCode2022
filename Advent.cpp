#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <deque>
#include <sstream>
#include <stack>
#include <fstream>


using namespace std;
#define print(sol) (cout << "Solution is : " << sol << '\n') 

struct node {
    node() {};
    string name;
    string parent;
    string grandparent;
    vector<node*> sons;
    long long sz;
    node(string name, string parent,string grandparent, long long sz) {
        this->name = name;
        this->parent = parent;
        this->sz = sz;
        this->grandparent = grandparent;
    }
    void addSon(node* son) {
        sons.push_back(son);
    }
};

long long treeSum2(node& v) {
    if (v.sons.size() == 0)return 0;
    long long sum = 0;
    if (v.sz <= 100000 && v.name != "root")sum += v.sz;
    for (auto son : v.sons) {
        sum += treeSum2(*son);
    }
    return sum;
}

long long treeSum(node& v) {
    for (auto son : v.sons) {
        v.sz += treeSum(*son);
    }
    return v.sz;
}

vector<int> nearestBigger(vector<int> v) { //returning the distances
    int n = v.size();
    for (int i = 0; i < n; i++)v[i] *= -1;
    vector<int> ans(n,-1);
    stack<pair<int, int>> s; s.push({ v[n - 1] ,n - 1 });
    for (int i = n - 2; i >= 0; i--) {
        while (s.size() && s.top().first >= v[i]) {
            ans[s.top().second] = i + 1;
            s.pop();
        }
        s.push({ v[i],i });
    }
    while (s.size()) {
        int idx = s.top().second;
        ans[idx] = 1;
        s.pop();
    }
    for (int i = 0; i < n; i++)ans[i]--;

    for (int i = 0; i < n; i++) {
        ans[i] = i - ans[i];
    }
    return ans;
}

void day8V2() { 
    //Main idea: for each row\col, each index is going know its nearest bigger values, to each side. Luckily, i've already solved 
    //such question in CSES (Called nearest smaller values, using monotonic stacks).
    //After this calculation, we'll simply travel over all indexes and will take the maximum over this product.

    ifstream inputFile;
    string line;
    vector<vector<int>> grid;

    inputFile.open("inputFile.txt");

    while (getline(inputFile, line)) {
        vector<int> row;
        for (auto ch : line) {
            row.push_back((int)ch - '0');
        }
        grid.push_back(row);
        row.clear();
    }
    inputFile.close();
    vector<vector<vector<int>>> plane(grid.size(),vector<vector<int>>(grid[0].size(),vector<int>(4,1))); //saving distance in each direction
    int j = 0;
    for (auto row : grid) {
        int n = row.size();
        auto v = nearestBigger(row);
        for (int i = 0; i < v.size(); i++)plane[j][i][0] = v[i];
        reverse(row.begin(),row.end());
        v = nearestBigger(row);
        for (int i = 0; i < v.size(); i++)plane[j][n - 1 - i][1] = v[i];
        j++;
    }
    j = 0;
    while (j < grid[0].size()) {
        int n = grid.size();
        vector<int> temp;
        for (int i = 0; i < grid.size(); i++) {
            temp.push_back(grid[i][j]);
        }
        auto v = nearestBigger(temp);
        for (int i = 0; i < grid.size(); i++) {
            plane[i][j][2] = v[i];
        }
        reverse(temp.begin(), temp.end());
        v = nearestBigger(temp);
        for (int i = 0; i < grid.size(); i++) {
            plane[n-1-i][j][3] = v[i];
        }
        j++;
    }
    int result = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            int temp = 1;
            for (int k = 0; k < 4; k++)temp *= plane[i][j][k];
            result = max(result, temp);
        }
    }
    print(result);
}

void day8() {
    //Main idea : we'll find all visible trees by finding all visible trees of height 9, then of height 8, ... , until 1.
    //We'll maintain a set of all indexes relevant to the calculation, due to the height i.
    //Let's say we are of height i. Then we'll locate all trees of such height i in our set. 
    //A tree of height i is visible if and only if there are no trees in our set ahead of him, in all of the 4 directions - in such case,
    //We'll his index to the set of visible trees.

    ifstream inputFile;
    string line;
    vector<vector<int>> grid;
    set<pair<int, int>> goodTrees;

    inputFile.open("inputFile.txt");

    while (getline(inputFile, line)) {
        vector<int> row;
        for (auto ch : line) {
            row.push_back((int)ch - '0');
        }
        grid.push_back(row);
        row.clear();
    }
    inputFile.close();

    vector<vector<pair<int,int>>> trees(10,vector<pair<int,int>>());
    vector<set<int>> candidatesRows(grid.size(), set<int>());
    vector<set<int>> candidatesColumns(grid[0].size(), set<int>());

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            trees[grid[i][j]].push_back({ i,j });
        }
    }

    for (int i = 9; i >= 0; i--) {
        for (auto tree : trees[i]) {
            candidatesRows[tree.first].insert(tree.second); //In what row am I?
            candidatesColumns[tree.second].insert(tree.first);
        }
        for (int j = 0; j < grid.size(); j++) {
            if (candidatesRows[j].size()) {
                pair<int, int> p = { j,*candidatesRows[j].begin()};
                goodTrees.insert(p);
                auto it = candidatesRows[j].end();
                it--;
                p = { j,*it };
                goodTrees.insert(p);
            }
        }
        for (int j = 0; j < grid[0].size(); j++) {
            if (candidatesColumns[j].size()) {
                pair<int, int> p = { *candidatesColumns[j].begin(),j };
                goodTrees.insert(p);
                auto it = candidatesColumns[j].end();
                it--;
                p = { *it,j };
                goodTrees.insert(p);
            }
        }
    }

    print(goodTrees.size());
}

void day7V2() {
    ifstream inputFile;
    string line;

    long long result = 0;
    inputFile.open("inputFile.txt");

    node root = node("root", "root", "root", 0);
    map<pair<string, string>, node*> m; //It's possible to have 2 directories of the same name with differents parents.
    m[{"root", "root"}] = &root;


    node* current = &root;
    while (getline(inputFile, line)) { // Generating the tree.
        if (line[0] == '$') {
            vector<string> parts;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ' ')) {
                parts.push_back(token);
            }

            if (parts.size() == 3) { // i.e. we're using cd
                if (parts[2] == "..") current = m[{current->parent, current->grandparent}];
                else {
                    if (parts[2] == "/") continue;
                    if (m.count({ parts[2],current->name }) == 0) {
                        m[{parts[2], current->name}] = new node(parts[2], current->name, current->parent, 0);
                    }
                    current = m[{parts[2], current->name}];
                }
            }
        }
        else { // A new son to current.
            vector<string> parts;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ' ')) {
                parts.push_back(token);
            }
            int sz = 0;
            if (parts[0] != "dir") sz = stoi(parts[0]);
            node* son = new node(parts[1], current->name, current->parent, sz); // Create a new son on the heap
            m[{parts[1], current->name}] = son;
            current->addSon(son); // Add the son using the pointer
        }
    }
    inputFile.close();
    treeSum(root);
    long long target = root.sz-40000000;
    long long mini = root.sz;
    for (auto it : m) {
        if (it.second->sz > target && it.second->sons.size()>0) {
            mini = min(it.second->sz, mini);
        }
    }
    print(mini);
}

void day7() {
    ifstream inputFile;
    string line;

    long long result = 0;
    inputFile.open("inputFile.txt");

    node root = node("root", "root" , "root", 0);
    map<pair<string,string>, node*> m; //It's possible to have 2 directories of the same name with differents parents.
    m[{"root", "root"}] = &root;
    

    node* current = &root;
    while (getline(inputFile, line)) { // Generating the tree.
        if (line[0] == '$') {
            vector<string> parts;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ' ')) {
                parts.push_back(token);
            }

            if (parts.size() == 3) { // i.e. we're using cd
                if (parts[2] == "..") current = m[{current->parent, current->grandparent}];
                else {
                    if (parts[2] == "/") continue;
                    if (m.count({parts[2],current->name}) == 0) {
                        m[{parts[2], current->name}] = new node(parts[2], current->name,current->parent, 0);
                    }
                    current = m[{parts[2], current->name}];
                }
            }
        }
        else { // A new son to current.
            vector<string> parts;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ' ')) {
                parts.push_back(token);
            }
            int sz = 0;
            if (parts[0] != "dir") sz = stoi(parts[0]);
            node* son = new node(parts[1], current->name, current->parent, sz); // Create a new son on the heap
            m[{parts[1], current->name}] = son;
            current->addSon(son); // Add the son using the pointer
        }
    }
    inputFile.close();
    treeSum(root);
    print(treeSum2(root));
}

void day6V2() {
    ifstream inputFile;
    string line;
    string input;

    inputFile.open("inputFile.txt");

    while (getline(inputFile, line)) {
        input += line;
    }
    inputFile.close();

    map<char, int> window;
    for (int i = 0; i < 14; i++)window[input[i]]++;
    if (window.size() == 14) { print(14); return; }

    for (int i = 14; i < input.size(); i++) {
        if (window[input[i - 14]] == 1)window.erase(input[i - 14]);
        else {
            window[input[i - 14]]--;
        }
        window[input[i]]++;
        if (window.size() == 14) { print(i + 1); return; }
    }
}

void day6() {
    ifstream inputFile;
    string line;
    string input;

    inputFile.open("inputFile.txt");

    while (getline(inputFile, line)) {
        input += line;
    }
    inputFile.close();

    map<char,int> window;
    for (int i = 0; i < 4; i++)window[input[i]]++;
    if (window.size() == 4) { print(4); return; }

    for (int i = 4; i < input.size(); i++) {
        if (window[input[i - 4]] == 1)window.erase(input[i - 4]);
        else {
            window[input[i - 4]]--;
        }
        window[input[i]]++;
        if (window.size() == 4) { print(i+1); return; }
    }
}

void day5V2() {
    string input = "";
    int i = 0;
    vector<deque<char>> towers(9, deque<char>());
    while (i++ < 8) {
        getline(cin, input);
        int cnt = 0;
        for (int j = 1; j <= input.size(); j++) {
            if (j % 4 == 0) { cnt++; continue; }
            if (j % 2 == 0)towers[(j - 1 - cnt) / 3].push_front(input[(j - 1)]);
        }
    }

    for (int j = 0; j < towers.size(); j++) {
        while (towers[j].back() == ' ') { towers[j].pop_back(); }
    }
    getline(cin, input);
    getline(cin, input);
    input = "";
    while (1) {
        getline(cin, input);
        if (input == "quit")break;

        vector<string> parts;
        string token;
        stringstream ss(input);
        while (getline(ss, token, ' ')) {
            parts.push_back(token);
        }
        int idx1 = stoi(parts[3]);
        int idx2 = stoi(parts[5]);
        int num = stoi(parts[1]);
        idx1--; idx2--;
        i = 0;
        stack<char> s;
        while (i++<num) {
            s.push(towers[idx1].back());
            towers[idx1].pop_back();
        }
        i = 0;
        while (i++ < num) {
            towers[idx2].push_back(s.top()); s.pop();
        }
    }
    system("cls");
    for (auto tower : towers) {
        cout << tower.back();
    }
}

void day5() {
    string input = "";
    int i = 0;
    vector<deque<char>> towers(9, deque<char>());
    while (i++ < 8) {
        getline(cin, input);
        int cnt = 0;
        for (int j = 1; j <= input.size(); j++) {
            if (j % 4 == 0) { cnt++; continue; }
            if (j % 2 == 0)towers[(j-1-cnt) / 3].push_front(input[(j-1)]);
        }
    }

    for (int j = 0; j < towers.size(); j++) {
        while (towers[j].back() == ' ') { towers[j].pop_back(); }
    }
    getline(cin, input);
    getline(cin, input);
    input = "";
    while (1) {
        getline(cin, input);
        if (input == "quit")break;

        vector<string> parts;
        string token;
        stringstream ss(input);
        while (getline(ss, token, ' ')) {
            parts.push_back(token);
        }
        int idx1 = stoi(parts[3]);
        int idx2 = stoi(parts[5]);
        int num = stoi(parts[1]);
        idx1--; idx2--;
        while (num--) {
            towers[idx2].push_back(towers[idx1].back());
            towers[idx1].pop_back();
        }
    }
    system("cls");
    for (auto tower : towers) {
        cout << tower.back();
    }
}

void day4V2() {
    int result = 0;
    int i = 0;
    while (i++ < 1000) {
        char ch;
        int a, b, x, y;
        cin >> a; cin >> b;
        cin >> ch;
        cin >> x; cin >> y;
        b *= -1; y *= -1;
        if ((x <= a && a <= y) || (x <= b && b <= y))result++;
        else if ((a <= x && x <= b) || a <= y && y <= b)result++;
    }
    print(result);
}

void day4(){
    int result = 0;
    int i = 0;
    while (i++ < 1000) {
        char ch;
        int a, b, x, y;
        cin >> a; cin >> b;
        cin >> ch;
        cin >> x; cin >> y;
        b *= -1; y *= -1;
        if ((x <= a && y >= b) || (a <= x && y <= b))result++;
    }
    print(result);
}

void day3V2() {
    //vector<pair<set<int>, set<int>>> backpacks;
    int result = 0;
    string input = "";
    int i = 0;
    set<int> seen;
    map<int,int> score;

    auto translate = [](char ch) {
        if ((int)ch >= 97)return (int)ch - 96;
        return (int)ch - 38;
    };

    while (1) {
        if (i / 3 > (i - 1) / 3) { score.clear(); }
        seen.clear();

        getline(cin, input);
        if (input == "quit")break;

        for (auto ch : input) {
            if (seen.count(translate(ch)) == 0) {
                seen.insert(translate(ch));
                score[translate(ch)] += 1;
            }
        }

        if (i % 3 == 2) {
            for (auto element : seen) {
                if (score[element] == 3) {
                    result += element;
                    break;
                }
            }
        }
        i++;
    }
    print(result);
}

void day3() {
    //vector<pair<set<int>, set<int>>> backpacks;
    int result = 0;
    string input = "";
    while (1) {
        getline(cin, input);
        if (input == "quit")break;
        string half1 = input.substr(0, input.length()/ 2);
        string half2 = input.substr(input.length() / 2);

        pair<set<int>, set<int>> parts;

        auto translate = [](char ch) {
            if ((int)ch >= 97)return (int)ch - 96;
            return (int)ch - 38;
        };

        for (char ch : half1) {
            parts.first.insert(translate(ch));
        }
        for (char ch : half2) {
            parts.second.insert(translate(ch));
        }
        for (auto x : parts.first) {
            if (parts.second.count(x)) {
                result += x;
                continue;
            }
        }
    }
    //for(back)
    print(result);
}

void day2() {
    vector<pair<int, int>> games;
    string input;

    while (true) {
        getline(cin, input);

        if (input == "quit")break;

        games.push_back({ ((int)input[0]) - 64,(int)input[2] - 87});
    }

    for (int i = 0; i < games.size(); i++) {
        if (games[i].second == 3)games[i].second = 2;
        else if (games[i].second == 2)games[i].second = 3;

        games[i].second = games[i].first - games[i].second;

        if (games[i].second <= 0)games[i].second += 3;
    }

    auto winner = [](int a, int b) {
        if (a == b)return 3;
        if (b - a == 1 || b-a == -2) return 6;
        if (b - a == 2 || b - a == -1)return 0;
    };
    

    int result = 0;
    for (auto game : games) {
        result += game.second + winner(game.first, game.second);
    }
    print(result);
}

void day1() {
    vector<vector<int>> snacks;
    vector<int> current;
    string input;

    while (true) {
        getline(cin, input);

        if (input == "quit") {
            break;
        }
        if (input == "") {
            snacks.push_back(current);
            current.clear();
        }
        else {
            current.push_back(stoi(input));
        }
    }

    priority_queue<int,vector<int>,greater<int>> pq;
    for (auto elf : snacks) {
        int sum = 0;
        for (auto v : elf) {
            sum += v;
        }
        pq.push(sum);
        if (pq.size() > 3)pq.pop();
    }
    int result = 0;
    while (!pq.empty()) {
        result += pq.top();
        pq.pop();
    }

    system("cls");
    cout << "The solution is :" << result << '\n';
}

int main()
{
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    day7V2();
}

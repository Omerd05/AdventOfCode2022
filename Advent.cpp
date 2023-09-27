#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <deque>
#include <sstream>
#include <stack>


using namespace std;
#define print(sol) (cout << "Solution is : " << sol << '\n') 

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
    day5V2();
}

#include <bits/stdc++.h>
using namespace std;

struct Student {
    string name;
    char gender; // 'M' or 'F'
    int clazz;
    array<int,9> score{};
    int sum = 0;
    inline int avg() const { return sum / 9; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, Student> mp;
    vector<string> ranking; // names in current ranking order
    unordered_map<string, int> rank_pos; // name -> 1-based rank
    bool started = false;
    bool dirty = false; // true if scores changed since last ranking build

    string cmd;
    while (cin >> cmd) {
        if (cmd == "ADD") {
            string name; char gender; int clazz; 
            array<int,9> sc;
            cin >> name >> gender >> clazz;
            for (int i = 0; i < 9; ++i) cin >> sc[i];
            if (started) {
                cout << "[Error]Cannot add student now." << '\n';
                continue;
            }
            if (mp.find(name) != mp.end()) {
                cout << "[Error]Add failed." << '\n';
                continue;
            }
            Student s;
            s.name = name;
            s.gender = gender;
            s.clazz = clazz;
            s.score = sc;
            s.sum = 0;
            for (int v : sc) s.sum += v;
            mp.emplace(name, std::move(s));
        } else if (cmd == "START") {
            // Build initial ranking and sort
            ranking.clear();
            ranking.reserve(mp.size());
            for (auto &p : mp) ranking.push_back(p.first);
            auto comp = [&](const string &a, const string &b) {
                const Student &sa = mp[a];
                const Student &sb = mp[b];
                int aa = sa.avg();
                int bb = sb.avg();
                if (aa != bb) return aa > bb;
                for (int i = 0; i < 9; ++i) {
                    if (sa.score[i] != sb.score[i]) return sa.score[i] > sb.score[i];
                }
                return sa.name < sb.name;
            };
            sort(ranking.begin(), ranking.end(), comp);
            rank_pos.clear();
            for (size_t i = 0; i < ranking.size(); ++i) rank_pos[ranking[i]] = static_cast<int>(i) + 1;
            started = true;
            dirty = false;
        } else if (cmd == "UPDATE") {
            string name; int code, score;
            cin >> name >> code >> score;
            auto it = mp.find(name);
            if (it == mp.end()) {
                cout << "[Error]Update failed." << '\n';
            } else {
                // update sum and score
                Student &s = it->second;
                s.sum -= s.score[code];
                s.score[code] = score;
                s.sum += s.score[code];
                dirty = true;
            }
        } else if (cmd == "FLUSH") {
            // Re-sort ranking according to current data
            if (!started) continue; // before start, no ranking exists
            if (!dirty) continue;   // no changes since last ranking
            auto comp = [&](const string &a, const string &b) {
                const Student &sa = mp[a];
                const Student &sb = mp[b];
                int aa = sa.avg();
                int bb = sb.avg();
                if (aa != bb) return aa > bb;
                for (int i = 0; i < 9; ++i) {
                    if (sa.score[i] != sb.score[i]) return sa.score[i] > sb.score[i];
                }
                return sa.name < sb.name;
            };
            sort(ranking.begin(), ranking.end(), comp);
            rank_pos.clear();
            for (size_t i = 0; i < ranking.size(); ++i) rank_pos[ranking[i]] = static_cast<int>(i) + 1;
            dirty = false;
        } else if (cmd == "PRINTLIST") {
            if (!started) continue; // nothing to print before start
            int r = 1;
            for (const string &name : ranking) {
                const Student &s = mp[name];
                cout << r << ' ' << s.name << ' ' << (s.gender == 'M' ? "male" : "female")
                     << ' ' << s.clazz << ' ' << s.avg() << '\n';
                ++r;
            }
        } else if (cmd == "QUERY") {
            string name; cin >> name;
            auto it = mp.find(name);
            if (it == mp.end()) {
                cout << "[Error]Query failed." << '\n';
            } else {
                if (!started) {
                    // As per problem, QUERY appears after START; return error defensively.
                    cout << "[Error]Query failed." << '\n';
                } else {
                    int pos = rank_pos[name];
                    cout << "STUDENT " << name << " NOW AT RANKING " << pos << '\n';
                }
            }
        } else if (cmd == "END") {
            break;
        } else {
            // Ignore unknown commands (shouldn't happen as per problem guarantees)
        }
    }
    return 0;
}

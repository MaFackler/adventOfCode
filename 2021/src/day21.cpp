#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h> 
#include <map>

using std::string;
using std::cout;
using std::vector;
using std::tuple;
using std::map;

static map<tuple<size_t, size_t, size_t, size_t>, tuple<size_t, size_t>> cache;

struct Player {
    size_t pos = 0;
    size_t score = 0;

    Player(size_t pos) : pos(pos) {};
};


tuple<size_t, size_t> count_universe(size_t p1, size_t p2, size_t s1, size_t s2) {
    if (s1 >= 21) {
        return tuple<size_t, size_t>(1, 0);
    }
    if (s2 >= 21) {
        return tuple<size_t, size_t>(0, 1);
    }
    auto to_search = std::make_tuple(p1, p2, s1, s2);
    if (cache.find(to_search) != cache.end())  {
        return cache[to_search];
    }
    auto wins = tuple<size_t, size_t>(0, 0);
    for (size_t d1 = 1; d1 <= 3; ++d1) {
        for (size_t d2 = 1; d2 <= 3; ++d2) {
            for (size_t d3 = 1; d3 <= 3; ++d3) {
                size_t pnew = p1 + d1 + d2 + d3;
                pnew = pnew % 10;
                size_t snew = s1 + pnew + 1;
                auto wins_of_player = count_universe(p2, pnew, s2, snew);
                wins = tuple<size_t, size_t>(std::get<1>(wins_of_player) + std::get<0>(wins),
                                       std::get<0>(wins_of_player) + std::get<1>(wins));
            }
        }
    }
    cache[to_search] = wins;
    return wins;
}

int main() {
    std::ifstream fp("data/input_day21.txt");
    string line;

    vector<Player> players;

    if (fp.is_open()) {
        int player = -1;
        int pos = -1;
        while (std::getline(fp, line)) {
            sscanf(line.c_str(), "Player %d starting position: %d", &player, &pos);
            players.emplace_back(pos - 1);
            assert(players.size() == player);
        }
    }

    auto res2 = count_universe(players[0].pos, players[1].pos, 0, 0);

    int player_index = 0;
    int dice_counter = 0;
    int move_distance = 0;
    int roll_count = -1;

    for (size_t i = 0; i < 1000; ++i) {
        Player &p = players[player_index];
        move_distance += (i + 1);
        dice_counter++;

        if (dice_counter == 3) {
            p.pos += move_distance;
            p.pos = p.pos % 10;
            p.score += p.pos + 1;
            //std::cout << "Player " << player_index + 1 << " score update " << p.score << std::endl;
            move_distance = 0;
            dice_counter = 0;
            player_index++;
            if (player_index >= players.size()) {
                player_index = 0;
            }
            if (p.score >= 1000) {
                roll_count = (i + 1);
                break;
            }
        }
    }
    assert(roll_count >= 0);
    auto looser = std::min_element(players.cbegin(),
                                   players.cend(),
                                   [](const Player &a, const Player &b) { return a.score < b.score; });
    size_t min_score = (*looser).score;
    size_t res = roll_count * min_score;

    //for (Player &p: players) {
    //    std::cout << "Player score is " << p.score << std::endl;
    //}

    std::cout << res << "\n";
    auto res22 = std::max(std::get<0>(res2), std::get<1>(res2));
    std::cout << res22 << "\n";

}

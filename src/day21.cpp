#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h> 

using std::string;
using std::cout;
using std::vector;

struct Player {
    int pos = 0;
    int score = 0;

    Player(int pos) : pos(pos) {};
};

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
    int min_score = (*looser).score;
    int res = roll_count * min_score;

    //for (Player &p: players) {
    //    std::cout << "Player score is " << p.score << std::endl;
    //}

    std::cout << res << "\n";

}

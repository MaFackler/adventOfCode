#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <tuple>

using std::string;
using std::cout;
using std::set;
using std::tuple;

typedef std::tuple<int, int, int> v3;

int main() {
    std::ifstream fp("data/input_day22.txt");
    string line;
    string on(8, 0);
    int xmin = 0;
    int xmax = 0;
    int ymin = 0;
    int ymax = 0;
    int zmin = 0;
    int zmax = 0;

    set<v3> cubes;
    if (fp.is_open()) {
        while (std::getline(fp, line)) {
            //cout << line << "\n";
            sscanf(line.c_str(), "%s x=%d..%d,y=%d..%d,z=%d..%d", 
                   on.c_str(),
                   &xmin, &xmax,
                   &ymin, &ymax,
                   &zmin, &zmax);

            if (abs(xmin) > 50 || abs(xmax) > 50
                || abs(ymin) > 50 || abs(ymax) > 50
                || abs(zmin) > 50 || abs(zmax) > 50)
                continue;
            for (int x = xmin; x <= xmax; ++x) {
                for (int y = ymin; y <= ymax; ++y) {
                    for (int z = zmin; z <= zmax; ++z) {
                        if (on.compare(0, 2, "on") == 0) {
                            cubes.insert(v3(x, y, z));
                        } else {
                            cubes.erase(v3(x, y, z));
                        }
                    }
                }
            }

        }
    }

    cout << cubes.size() << "\n";
}

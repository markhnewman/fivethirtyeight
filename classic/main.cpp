#include <iostream>
#include <random>
#include <chrono>

using namespace std;

minstd_rand gen;
bernoulli_distribution moonDist(0.4);
bernoulli_distribution doubDist(0.2);
bernoulli_distribution tateDist(0.1);

int moonwalkers() {
    int outs = 0;
    int walks = 0;
    while (outs < 3) {
        if (moonDist(gen))
            ++walks;
        else
            ++outs;
    }
    return max(walks - 3, 0);
}

int doubloons() {
    int outs = 0;
    int dbls = 0;
    while (outs < 3) {
        if (doubDist(gen))
            ++dbls;
        else
            ++outs;
    }
    return max(dbls - 1, 0);
}

int taters() {
    int outs = 0;
    int runs = 0;
    while (outs < 3) {
        if (tateDist(gen))
            ++runs;
        else
            ++outs;
    }
    return runs;
}

double matchup(int (*teamA)(), int (*teamB)(), size_t count) {
    size_t aWin = 0;
    for (size_t i = 0; i < count; ++i) {
        int aScore = 0;
        int bScore = 0;
        for (size_t j = 0; j < 9; ++j) {
            aScore += (*teamA)();
            bScore += (*teamB)();
        }
        while (aScore == bScore) {
            aScore += (*teamA)();
            bScore += (*teamB)();
        }
        if (aScore > bScore)
            ++aWin;
    }
    return (double)aWin / (double)count;
}

double percent(double x) {
    return 0.01 * floor(10000.0 * x);
}

int main(int argc, char * argv[]) {
    size_t count = 10000;
    if (argc == 2)
        count = atoll(argv[1]);

    gen.seed((uint_fast32_t)chrono::system_clock::now().time_since_epoch().count());

    double moonVdoub = matchup(moonwalkers, doubloons, count);
    double doubVtate = matchup(doubloons, taters, count);
    double tateVmoon = matchup(taters, moonwalkers, count);

    cout << "Moonwalkers v Doubloons - " << percent(moonVdoub) << " v " << percent(1.0 - moonVdoub) << endl;
    cout << "Doubloons v Taters      - " << percent(doubVtate) << " v " << percent(1.0 - doubVtate) << endl;
    cout << "Taters v Moonwalkers    - " << percent(tateVmoon) << " v " << percent(1.0 - tateVmoon) << endl;
    cout << endl;
    cout << "Moonwalkers - " << percent(0.5 + 0.5 * (moonVdoub - tateVmoon)) << endl;
    cout << "Doubloons   - " << percent(0.5 + 0.5 * (doubVtate - moonVdoub)) << endl;
    cout << "Taters      - " << percent(0.5 + 0.5 * (tateVmoon - doubVtate)) << endl;

    return 0;
}

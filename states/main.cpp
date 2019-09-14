#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define JUMPS(x, y) jumps[(x) + 26 * (y)]

void visit(int iFrom, int iTo, const string &iHist, size_t &iLongest, size_t &iLongestCount, int *iCount, int *iJumps) {
    int count[26];
    int jumps[260];
    memcpy(count, iCount, sizeof(count));
    memcpy(jumps, iJumps, sizeof(jumps));

    int pos = JUMPS(iFrom, iTo);
    string hist = iHist + (char)('A' + pos);
    count[iFrom] -= 1;
    JUMPS(iFrom, iTo) = JUMPS(iFrom, count[iFrom]);

    if (count[pos] == 0) {
        if (hist.size() > iLongest) {
            iLongest = hist.size();
            iLongestCount = 1;
            cout << iLongest << " = " << hist << endl;
        } else if (hist.size() == iLongest) {
            iLongestCount += 1;
        }
    } else {
        for (int i = 0; i < count[pos]; ++i)
            visit(pos, i, hist, iLongest, iLongestCount, count, jumps);
    }
}

int main(int argc, char * argv[]) {
    int count[26];
    int jumps[260];
    memset(count, 0, sizeof(count));

    ifstream file("states.txt", ifstream::binary);
    char start = '\0';
    while (!file.eof()) {
        char c;
        file.get(c);
        if (c >= 'A' && c <= 'Z') {
            if (start) {
                int startID = start - 'A';
                int endID = c - 'A';
                JUMPS(startID, count[startID]) = endID;
                count[startID] += 1;
                start = '\0';
            } else {
                start = c;
            }
        }
    }

    size_t longest = 0;
    size_t longestCount = 0;
    for (int i = 0; i < 26; ++i) {
        string hist(1, (char)('A' + i));
        for (int j = 0; j < count[i]; ++j)
            visit(i, j, hist, longest, longestCount, count, jumps);
    }
    cout << "x " << longestCount << endl;

    return 0;
}

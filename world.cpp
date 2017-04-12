#include "world.h"
#include <stdlib.h>

bool flipCoin(int chance, int maxChance) {
    return rand() % maxChance < chance;
}

World::World() { }

// init
World::World(const vector<vector<char> > &arr) {
    array(arr);
}

// get array
const vector<vector<char> > &World::array() const {
    return arr;
}

// set array
const vector<vector<char> > &World::array(const vector<vector<char> > &in) {
    arr = in;
    w = in.size();
    h = w ? in[0].size() : 0;
    return in;
}

// increase size and randomness
void World::expand(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        vector<vector<char> > res(w*2 - 1, vector<char>(h*2 - 1));
        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
                res[x*2][y*2] = arr[x][y];
        array(res);
        for (int x = 1; x < w; x += 2)
            for (int y = 1; y < h; y += 2)
                arr[x][y] = flipCoin(arr[x - 1][y - 1] +
                                     arr[x + 1][y - 1] +
                                     arr[x + 1][y + 1] +
                                     arr[x - 1][y + 1], 4);
        for (int y = 1; y < h; y += 2)
            arr[0][y] = arr[w - 1][y] = 0;

        for (int x = 1; x < w; x += 2) {
            arr[x][0] = 0;
            arr[x][h - 1] = flipCoin(arr[x - 1][h - 1] +
                                     arr[x + 1][h - 1] +
                                     arr[x][h - 2] + 1, 4);
        }
        for (int x = 1; x < w - 1; ++x)
            for (int y = (x&1)?2:3; y < h - 1; y += 2)
                arr[x][y] = flipCoin(arr[x - 1][y] +
                                     arr[x + 1][y] +
                                     arr[x][y - 1] +
                                     arr[x][y + 1], 4);
    }
}

// apply cellular automata
// 00000000 00000000 00000000 00000000
// <survival  rules> <  birth rules  >
// 00000000 00001100 00000000 00001000 - conways life
// 00000001 11101000 00000001 11010000 - smoothing 1 (B4678/S35678)
// 00000001 11110000 00000001 11100000 - smoothing 2 (B5678/S45678)
void World::cellular(int rules, int iterations) {
    vector<vector<char> > res(w, vector<char>(h));
    int init[2] {1, 1 << 16};
    for (int i = 0; i < iterations; ++i) {
        int temp;
        for (int x = 1; x < w - 1; ++x)
            for (int y = 1; y < h - 1; ++y) {
                temp = init[arr[x][y]];
                temp <<= arr[x - 1][y - 1];
                temp <<= arr[x][y - 1];
                temp <<= arr[x + 1][y - 1];
                temp <<= arr[x + 1][y];
                temp <<= arr[x + 1][y + 1];
                temp <<= arr[x][y + 1];
                temp <<= arr[x - 1][y + 1];
                temp <<= arr[x - 1][y];
                res[x][y] = (temp & rules) != 0;
            }
        for(int x = 1; x < w - 1; ++x) {
            temp = init[arr[x][0]];
            temp <<= arr[x - 1][0];
            temp <<= arr[x + 1][0];
            temp <<= arr[x + 1][1];
            temp <<= arr[x][1];
            temp <<= arr[x - 1][1];
            res[x][0] = (temp & rules) != 0;

            temp = init[arr[x][h - 1]];
            temp <<= arr[x - 1][h - 1];
            temp <<= arr[x + 1][h - 1];
            temp <<= arr[x + 1][h - 2];
            temp <<= arr[x][h - 2];
            temp <<= arr[x - 1][h - 2];
            res[x][h - 1] = (temp & rules) != 0;
        }
        for(int y = 1; y < h - 1; ++y) {
            temp = init[arr[0][y]];
            temp <<= arr[0][y - 1];
            temp <<= arr[0][y + 1];
            temp <<= arr[1][y + 1];
            temp <<= arr[1][y];
            temp <<= arr[1][y - 1];
            res[0][y] = (temp & rules) != 0;

            temp = init[arr[w - 1][y]];
            temp <<= arr[w - 1][y - 1];
            temp <<= arr[w - 1][y + 1];
            temp <<= arr[w - 2][y + 1];
            temp <<= arr[w - 2][y]; ;
            temp <<= arr[w - 2][y - 1];
            res[w - 1][y] = (temp & rules) != 0;
        }
        temp = init[arr[0][0]];
        temp <<= arr[0][1];
        temp <<= arr[1][1];
        temp <<= arr[1][0];
        res[0][0] = (temp & rules) != 0;

        temp = init[arr[0][h - 1]];
        temp <<= arr[0][h - 2];
        temp <<= arr[1][h - 2];
        temp <<= arr[1][h - 1];
        res[0][h - 1] = (temp & rules) != 0;

        temp = init[arr[w - 1][h - 1]];
        temp <<= arr[w - 1][h - 2];
        temp <<= arr[w - 2][h - 2];
        temp <<= arr[w - 2][h - 1];
        res[w - 1][h - 1] = (temp & rules) != 0;

        temp = init[arr[w - 1][0]];
        temp <<= arr[w - 1][1];
        temp <<= arr[w - 2][1];
        temp <<= arr[w - 2][0];
        res[w - 1][0] = (temp & rules) != 0;

        std::swap(arr, res);
    }
}

// nearest neighbour
void World::rescale(int w, int h) {
    vector<vector<char> > res(w, vector<char>(h));
    for (int x = 0; x < w; ++x)
        for(int y = 0; y < h; ++y)
            res[x][y] = arr[x * this->w / w][y * this->h / h];
    array(res);
}

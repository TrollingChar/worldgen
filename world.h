#ifndef WORLD_H
#define WORLD_H

#include <vector>

using namespace std;


class World {
    vector<vector<char> > arr;
public:
    int w, h;
    World();
    World(const vector<vector<char> > &arr);

    const vector<vector<char> > &array() const;
    const vector<vector<char> > &array(const vector<vector<char> > &in);

    void expand(int iterations = 1);
    void cellular(int rules, int iterations = 10);
    void rescale(int w, int h);
};

#endif // WORLD_H

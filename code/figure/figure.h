#ifndef FIGURE_H
#define FIGURE_H

#include "rand.h"
#include <vector>
#include <queue>
#include <utility>
#include <QString>
using namespace std;

class Figure
{
private:
    bool** arr;
    int pathsNum;
    int rows, cols; // The number of rows and columns of the figure
    // Paths of figure, Each path is represented by coordinates from the start point to the end point.
    vector<vector<pair<int, int>>> paths;    

    // Generate a path from a random point to 'end' and push the path to private member 'paths'.
    bool dfs(pair<int, int>& start, pair<int, int>& end, vector<pair<int, int>>& path);
    pair<int, int> genStart(pair<int, int>& end);
    bool adj(int direction, pair<int, int>& location);
    void initArr();
public:
    /*
     * Generate a figure with 'pathsNum' paths, each path has the same end point.
    */
    explicit Figure(int _rows = 8, int _cols = 12, int pathsNum = 1);
    bool exportFigure();
    bool notEmptyLocation(pair<int, int> location){return arr[location.first][location.second];}
    void setLocation(int row, int col, bool flag){arr[row][col] = flag;}
    bool** getArr(){return arr;}
    pair<int, int> getLoc(int pathNO, int index);
    int getRows(){return rows;}
    int getCols(){return cols;}
    vector<pair<int, int>>& getPath(int pathNO){return paths[pathNO];}
    int getPathLength(int pathNO){return paths[pathNO].size();}
    int getPathsNum(){return paths.size();}
    pair<int, int> getEnd();
    pair<int, int> getStart(int pathNO){return paths[pathNO][0];}
    bool onPath(pair<int, int>& location);
    void print();
    void show(int i);
    void reset();
    void genPaths();
    bool genUserDefinedPath(QString& figurePath);

};

inline pair<int, int> Figure::getLoc(int pathNO, int index)
{
    if(index >= paths[pathNO].size())
        return make_pair(-1, -1);
    else
        return paths[pathNO][index];

}

inline pair<int, int> Figure::getEnd()
{
    if(paths.size() > 0)
        return paths[0][paths[0].size()-1];
    return make_pair(-1, -1);
}







#endif // FIGURE_H

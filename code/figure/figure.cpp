#include "figure.h"
#include <string.h>
#include <queue>
#include <iostream>
#include <fstream>
#include <QDebug>
using namespace std;

#define RESET_ARRAY for(int i = 0; i < rows; ++i) \
    memset(arr[i], 0, sizeof(bool)*cols)

Figure::Figure(int _rows, int _cols, int _pathsNum)
{
    rows = _rows;
    cols = _cols;
    pathsNum = _pathsNum;
    arr = new bool*[rows];
    for(int i = 0; i < rows; ++i)
    {
        arr[i] = new bool[cols];
        memset(arr[i], 0, sizeof(bool) * cols);
    }
}

bool Figure::exportFigure()
{
    ofstream fout("./figure.txt");
    if(fout.is_open())
    {
        fout << "/*\n";
        fout << "the first line are two integers: rows and columns of figure\n";
        fout << "the second line: the number of paths\n";
        fout << "Starting  from the third line, each two lines represents a path:\n";
        fout << "   The first line is a integer: length of a path\n";
        fout << "   The second line are row and column of each location on the path\n";
        fout << "notes: All integers that are in the same row are seperated by space.\n";
        fout << "All comments should be included with /**/ at the beginning of the file.\n";
        fout << "The row and column coordiante of each location on the figure start at 0.\n";
        fout << "Each path has the same end.\n";
        fout << "*/\n";

        fout << rows << " " << cols << "\n";
        fout << paths.size() << "\n";
        fout << "\n";
        for(int i = 0; i < paths.size(); ++i)
        {
            int len = paths[i].size();
            fout << len << "\n";
            for(int j = 0; j <len; ++j)
            {
                fout << paths[i][j].first << " " << paths[i][j].second << " ";
            }
            fout << "\n";
        }
        fout.close();
        return true;
    }
    else
        qDebug() << "Failing to export figure.";
    return false;
}

bool Figure::onPath(pair<int, int> &location)
{
    int pathsNum = paths.size();
    for(int i = 0; i < pathsNum; ++i)
    {
        int num = paths[i].size();
        for(int j = 0; j < num; ++j)
            if(location == paths[i][j])
                return true;
    }
    return false;
}



bool Figure::adj(int direction, pair<int, int>& location)
{
    int row, col;
    // up
    if(direction == 0)
    {
        row = location.first - 1;
        col = location.second;
    }
    // down
    else if(direction == 1)
    {
        row = location.first + 1;
        col = location.second;
    }
    // left
    else if(direction == 2)
    {
        row = location.first;
        col = location.second - 1;
    }
    // right
    else
    {
        row = location.first;
        col = location.second + 1;
    }
    if(row < 0 || row >= rows || col < 0 || col >= cols || arr[row][col])
        return false;
    location.first = row;
    location.second = col;
    return true;
}


/*
 * Randomly generate a start point and perform bfs from this point.
 * Set the value of each accessed location location to 1.
void Figure::bfs()
{
    pair<int, int> start(myRand(rows), myRand(cols));
    queue<pair<int, int>> q;
    addAdj(start, q);
    while(!q.empty())
    {
        pair<int, int> location = q.front();
        q.pop();
        if(randBool())
            continue;
        addAdj(location, q);
    }
}
*/

inline int dis(pair<int, int>start, pair<int, int>end)
{
    return abs(start.first - end.first) + abs(start.second - end.second);
}

pair<int, int> Figure::genStart(pair<int, int>& end)
{
    pair<int, int> start(myRand(rows), myRand(cols));
    int pathsNum = paths.size();

    // Make sure the start and end points are not too close together
    int sum = dis(start, end);
    while(start == end || sum < rows)
    {
        start.first = myRand(rows);
        start.second = myRand(cols);
        sum = dis(start, end);
    }

    for(int i = 0; i < pathsNum; ++i)
    {
        if (paths[i].empty())
            break;
        while(paths[i][0] == start)
        {
            start.first = myRand(rows);
            start.second = myRand(cols);
        }
    }
    return start;
}

int initDirs(bool* dirs, pair<int, int>& start, pair<int, int>& end)
{
    int count = 0;
    // 'start' is below 'end', can't go down.
    if (start.first > end.first)
    {
        dirs[1] = 1;
        ++count;
    }
    // 'start' is above 'end', can't go up.
    if (start.first < end.first)
    {
        dirs[0] = 1;
        ++count;
    }
    // 'start' is on the right of 'end', can't go right.
    if (start.second > end.second)
    {
        dirs[3] = 1;
        ++count;
    }
    // 'start' is on the left of 'end', can't go left.
    if (start.second < end.second)
    {
        dirs[2] = 1;
        ++count;
    }

    return count;
}

bool Figure::dfs(pair<int, int>& start, pair<int, int>& end, vector<pair<int, int>>& path)
{    
    arr[start.first][start.second] = 1;
    path.push_back(start);
    if(start == end)
        return true;
    int direction, countDirs;
    bool ret;
    bool dirs[4] = { 0,0,0,0 };
    countDirs = initDirs(dirs, start, end);
    pair<int, int>cur = start;
    while(countDirs < 4)
    {
        direction = rand0_3();
        while(dirs[direction])
            direction = rand0_3();
        dirs[direction] = 1;
        ++countDirs;
        ret = adj(direction, start);
        if(ret)
            if(dfs(start, end, path))
                return true;
        // restore the value of 'start'
        start = cur;
    }
    path.pop_back();
    return false;
}

void Figure::initArr()
{
    RESET_ARRAY;
    int pathsNum = paths.size();
    for(int i = 0; i < pathsNum; ++i)
    {
        int pointsNum = paths[i].size();
        for(int j = 0; j < pointsNum; ++j)
        {
            int row = paths[i][j].first, col = paths[i][j].second;
            arr[row][col] = 1;
        }
    }
}


void Figure::print()
{
    for(int i = 0; i < paths.size(); ++i)
    {
        cout << "(" << paths[i][0].first << "," << paths[i][0].second << endl;
        show(i);
        cout << endl;
    }
}

void Figure::show(int i)
{
    char arr[8][12];
    memset(arr, '-', sizeof(char) * 8 * 12);
    arr[paths[i][0].first][paths[i][0].second] = 's';
    for (int j = 1; j < paths[i].size(); ++j)
    {
        int row = paths[i][j].first;
        int col = paths[i][j].second;
        arr[row][col] = '*';
    }
    arr[paths[i][paths[i].size() - 1].first][paths[i][paths[i].size() - 1].second] = 'e';
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            cout << (char)arr[i][j] << ' ';
        }
        cout << endl;
    }

}

void Figure::reset()
{
    for(int i = 0; i < paths.size(); ++i)
        paths[i].clear();
    RESET_ARRAY;

}

void Figure::genPaths()
{
    pathsNum = 1;
    rows = 8;
    cols = 12;
    paths.resize(pathsNum);
    pair<int, int>end(myRand(rows), myRand(cols));
    for(int i = 0; i < pathsNum; ++i)
    {
        pair<int, int>start = genStart(end);
        dfs(start, end, paths[i]);
        RESET_ARRAY;
    }
    //print();
}

bool filterComments(ifstream& fin)
{
    string str;
    getline(fin, str);
    if(!fin.fail() && str.substr(0, 2) == "/*")
    {
        while(!fin.fail())
        {
            if(str.length() >= 2 && str.substr(str.length()-2, str.length()) == "*/")
                return true;
            getline(fin, str);
        }
        return false;
    }
    // No comments
    fin.seekg(0, ios::beg);
    return true;
}

bool Figure::genUserDefinedPath(QString &figurePath)
{
    // When a figure instance is generated, size of 'paths' was initialized.
    paths.clear();

    ifstream fin(figurePath.toStdString());
    if(fin.is_open())
    {
        if(!filterComments(fin))
            return false;
        string buffer;
        int lineNO = 1;
        int pathsNum;
        fin >> rows >> cols >> pathsNum;
        if(!fin.fail())
        {

            paths.resize(pathsNum);
            for(int i = 0; i < pathsNum; ++i)
            {
                int len;
                fin >> len;
                if(fin.fail())
                    return false;
                for(int j = 0; j < len; ++j)
                {
                    int row, col;
                    fin >> row >> col;
                    if(fin.fail())
                        return false;
                    paths[i].push_back(make_pair(row, col));
                }
            }
            return true;
        }
    }
    return false;
}

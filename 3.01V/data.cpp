
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

const double yStart = -5.0;
const double yEnd = 5.0;
const double xStart = -8.0;
const double xEnd = 8.0;
const double step = 0.1;

struct fileHeader {
    std::string model, version, date, description, lengthUnit;
    int dimension;
    int nodes;
    int expressions;

    void read(std::istream &fin) {
        std::string tag, axis;
        char percent;
        fin >> percent >> tag >> std::ws;
        std::getline(fin, model);
        fin >> percent >> tag >> std::ws;
        std::getline(fin, version);
        fin >> percent >> tag >> std::ws;
        std::getline(fin, date);
        fin >> percent >> tag >> dimension;
        fin >> percent >> tag >> nodes;
        fin >> percent >> tag >> expressions;
        fin >> percent >> tag >> std::ws;
        std::getline(fin, description);
        fin >> percent >> tag >> tag >> lengthUnit;
        fin >> percent;
        getline(fin, axis);
    }
};

struct Point {
    double x;
    double y;
    double V;
    double E;

    Point() {
        x = 0;
        y = 0;
        V = 0;
        E = 0;
    }

    friend std::istream &operator>>(std::istream &fin, Point &point) {
        fin >> point.x >> point.y >> point.V;
        return fin;
    }

    friend std::ostream &operator<<(std::ostream &fout, const Point &point) {
        fout << point.x << " " << point.y << " " << point.E;
        return fout;
    }
};

double distance(double a_x, double a_y, double b_x, double b_y) {
    return sqrt(pow(a_x - b_x, 2) + pow(a_y - b_y, 2));
}

std::pair<int, int> getNearestX(const std::vector<Point> &points, double x, double y) {
    int l, u;
    l = u = 0;
    double lb, ub;
    lb = ub = 1000;
    for (int i = 0; i < points.size(); i++) {
        double dist = distance(points[i].x, points[i].y, x, y);
        if ((dist < lb) && (points[i].x < x)) {
            lb = dist;
            l = i;
        } else if ((dist < ub) && (points[i].x > x)) {
            ub = dist;
            u = i;
        }
    }
    return {l, u};
}

std::pair<int, int> getNearestY(const std::vector<Point> &points, double x, double y) {
    int l, u;
    l = u = 0;
    double lb, ub;
    lb = ub = 1000;
    for (int i = 0; i < points.size(); i++) {
        double dist = distance(points[i].x, points[i].y, x, y);
        if ((dist < lb) && (points[i].y < y)) {
            lb = dist;
            l = i;
        } else if ((dist < ub) && (points[i].y > y)) {
            ub = dist;
            u = i;
        }
    }
    return {l, u};
}

void calculateE(std::istream &fin, std::ostream &fout) {
    fileHeader header;
    header.read(fin);
    std::vector<Point> points;
    std::string imaginary;
    for (int i = 0; i < header.nodes; i++) {
        Point point;
        fin >> point;
        points.push_back(point);
        if (fin.peek() == '+' || fin.peek() == '-') {
            std::getline(fin, imaginary);
        }
    }
    for (double y = yStart; y <= yEnd; y += step) {
        for (double x = xStart; x <= xEnd; x += step) {
            std::pair<int, int> xPair = getNearestX(points, x, y);
            std::pair<int, int> yPair = getNearestY(points, x, y);

            double Ex = (points[xPair.second].V - points[xPair.first].V) /
                        (points[xPair.second].x - points[xPair.first].x);

            double Ey = (points[yPair.second].V - points[yPair.first].V) /
                        (points[yPair.second].y - points[yPair.first].y);

            Point point;
            point.x = x;
            point.y = y;
            point.E = sqrt(Ex * Ex + Ey * Ey);
            fout << point << "\n";
        }
    }
}

std::string getFileName(int i, int j) {
    std::string name;
    name += (char) ('0' + i);
    name += '_';
    name += (char) ('0' + j);
    return name;
}

int main() {
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 6; j++) {
            std::string fileIn = getFileName(i, j);
            std::string fileOut = "out_";
            fileIn += ".txt";
            fileOut += fileIn;
            std::ifstream fin(fileIn);
            std::ofstream fout(fileOut);
            calculateE(fin, fout);
        }
    }
    return 0;
}

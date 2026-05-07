#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
    double x;
    double y;

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

double cross(const Point& o, const Point& a, const Point& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

double distance(const Point& a, const Point& b) {
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<Point> convexHull(std::vector<Point> points) {
    std::sort(points.begin(), points.end());
    points.erase(std::unique(points.begin(), points.end()), points.end());

    const int n = static_cast<int>(points.size());
    if (n <= 1) return points;

    std::vector<Point> lower;
    for (const auto& p : points) {
        while (lower.size() >= 2 &&
               cross(lower[lower.size() - 2], lower[lower.size() - 1], p) <= 0.0) {
            lower.pop_back();
        }
        lower.push_back(p);
    }

    std::vector<Point> upper;
    for (int i = n - 1; i >= 0; --i) {
        const Point& p = points[i];
        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper[upper.size() - 1], p) <= 0.0) {
            upper.pop_back();
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

double perimeter(const std::vector<Point>& hull) {
    const int m = static_cast<int>(hull.size());
    if (m == 0) return 0.0;
    if (m == 1) return 0.0;
    if (m == 2) return 2.0 * distance(hull[0], hull[1]);

    double perim = 0.0;
    for (int i = 0; i < m; ++i) {
        perim += distance(hull[i], hull[(i + 1) % m]);
    }
    return perim;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<Point> points;
    points.reserve(static_cast<size_t>(n) * 4);

    for (int i = 0; i < n; ++i) {
        double x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        const double minX = std::min(x1, x2);
        const double maxX = std::max(x1, x2);
        const double minY = std::min(y1, y2);
        const double maxY = std::max(y1, y2);

        points.push_back({minX, minY});
        points.push_back({minX, maxY});
        points.push_back({maxX, minY});
        points.push_back({maxX, maxY});
    }

    const std::vector<Point> hull = convexHull(points);
    const double ans = perimeter(hull);

    std::cout << std::fixed << std::setprecision(10) << ans << '\n';
    return 0;
}

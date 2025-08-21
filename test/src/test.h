#pragma once

#include <algorithm> // для std::max
#include <cmath>     // для std::abs
#include <set>

#include "delaunator.hpp"
#include "FastNoiseLite.h"

std::mt19937 rndg(static_cast<unsigned int>(std::time(nullptr)));
std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

constexpr int GRIDSIZE = 50;
constexpr float JITTER = 1000.0f;
constexpr float WAVELENGTH = 0.5;

std::vector<double> coords;

struct DelaunayMap {
    std::vector<glm::vec2> points;
    std::vector<std::size_t> triangles;
    std::vector<std::size_t> halfedges;
    int numRegions;
    int numTriangles;
    int numEdges;
    std::vector<glm::vec2> centers;
    std::vector<float> elevation;
};

struct Line {
    glm::vec2 a;
    glm::vec2 b;
};

struct ColorLine {
    glm::vec2 a;
    glm::vec2 b;
    glm::vec3 color;
};

struct LinePolygon {
    std::vector<ColorLine> lines;
    glm::vec3 color; // RGB [0..1]
};

struct ColoredPolygon {
    std::vector<glm::ivec2> vertices;
    glm::vec3 color; // RGB [0..1]
};

const int SCALE = 2; // множитель масштаба

glm::ivec2 scalePoint(const glm::vec2& p) {
    return glm::ivec2(
        static_cast<int>(std::floor(p.x * SCALE)),
        static_cast<int>(std::floor(p.y * SCALE))
    );
}

class MapGenerator {
public:
    MapGenerator() {
        std::mt19937 rndg(std::random_device{}());
        std::uniform_real_distribution<double> dist(-JITTER, JITTER);

        // создаем координаты с джиттером
        for (int x = 0; x <= GRIDSIZE; ++x) {
            for (int y = 0; y <= GRIDSIZE; ++y) {
                double px = x + dist(rndg);
                double py = y + dist(rndg);
                coords.push_back(px);
                coords.push_back(py);
                points.emplace_back(px, py);
            }
        }

        // используем unique_ptr чтобы избежать проблем с копированием
        delaunator = std::make_unique<delaunator::Delaunator>(coords);

        // создаем карту
        map.points = points;
        map.triangles = delaunator->triangles;
        map.halfedges = delaunator->halfedges;
        map.numRegions = points.size();
        map.numTriangles = delaunator->triangles.size() / 3;
        map.numEdges = delaunator->halfedges.size();
        map.centers = calculateCentroids(map.points, map);
        map.elevation = assignElevation();
    }

    std::vector<Line> drawCellBoundaries() {
        std::vector<Line> lines;

        const auto& points = map.points;
        const auto& centers = map.centers;
        const auto& halfedges = map.halfedges;
        const auto& triangles = map.triangles;
        int numEdges = map.numEdges;

        for (int e = 0; e < numEdges; ++e) {
            if (e < static_cast<int>(halfedges[e])) { // чтобы не дублировать линию
                const glm::vec2& p = centers[e / 3];            // triangleOfEdge(e)
                const glm::vec2& q = centers[halfedges[e] / 3]; // triangleOfEdge(halfedges[e])
                lines.push_back({p, q});
            }
        }
        return lines;
    }
    
    std::vector<float> assignElevation() {
        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

        const auto& points = map.points;
        size_t numRegions = map.numRegions;

        std::vector<float> elevation(numRegions);

        for (size_t r = 0; r < numRegions; ++r) {
            float nx = points[r].x / GRIDSIZE;
            float ny = points[r].y / GRIDSIZE;

            // start with noise, normalize [-1,1] -> [0,1]
            float n = noise.GetNoise(nx / WAVELENGTH, ny / WAVELENGTH) * 10;
            elevation[r] = (1.0f + n) / 2.0f;

            // modify noise to make islands
            float d = 2.0f * std::max(std::abs(nx), std::abs(ny)); // 0..1
            elevation[r] = (1.0f + elevation[r] - d) / 2.0f;
        }

        return elevation;
    }

    glm::vec3 hsl2rgb(float h, float s, float l) {
        float c = (1.0f - std::fabs(2.0f*l - 1.0f)) * s;
        float hp = h * 6.0f; // h в [0..1], умножаем на 6 для сегмента
        float x = c * (1.0f - std::fabs(fmod(hp, 2.0f) - 1.0f));

        float r=0, g=0, b=0;
        if      (0 <= hp && hp < 1) { r=c; g=x; b=0; }
        else if (1 <= hp && hp < 2) { r=x; g=c; b=0; }
        else if (2 <= hp && hp < 3) { r=0; g=c; b=x; }
        else if (3 <= hp && hp < 4) { r=0; g=x; b=c; }
        else if (4 <= hp && hp < 5) { r=x; g=0; b=c; }
        else if (5 <= hp && hp < 6) { r=c; g=0; b=x; }

        float m = l - 0.5f * c;
        return glm::vec3(r+m, g+m, b+m);
    }

    glm::vec3 getColorFromElevation(float e) {
        float h, s = 0.5f, l = 0.5f;
        if (e < 1.0f) {
            // равнина: зелёный → жёлтый
            // float t = e / 0.5f;
            float t = (e - 0.3f) / 0.2f;
            h = glm::mix(240.0f/360.0f, 200.0f/360.0f, t);
            l = glm::mix(0.5f, 0.6f, t);
        }
        else {
            // горы: жёлтый → белый
            float t = (e - 0.5f) / 0.5f;
            h = glm::mix(60.0f/360.0f, 0.0f/360.0f, t);
            l = glm::mix(0.6f, 0.9f, t);
            s = glm::mix(0.5f, 0.0f, t);
        }

        // HSL → RGB
        float c = (1.0f - std::fabs(2.0f*l - 1.0f)) * s;
        float hp = h * 6.0f;
        float x = c * (1.0f - std::fabs(fmod(hp,2.0f) - 1.0f));

        float r=0, g=0, b=0;
        if      (0 <= hp && hp < 1) { r=c; g=x; b=0; }
        else if (1 <= hp && hp < 2) { r=x; g=c; b=0; }
        else if (2 <= hp && hp < 3) { r=0; g=c; b=x; }
        else if (3 <= hp && hp < 4) { r=0; g=x; b=c; }
        else if (4 <= hp && hp < 5) { r=x; g=0; b=c; }
        else if (5 <= hp && hp < 6) { r=c; g=0; b=x; }

        float m = l - 0.5f*c;
        return glm::vec3(r+m, g+m, b+m);
    }

    std::vector<LinePolygon> makeCellColors() {
        std::set<int> seen;
        auto& triangles = map.triangles;
        int numEdges = map.numEdges;
        auto& centers = map.centers;

        std::vector<LinePolygon> result;

        for (int e = 0; e < numEdges; e++) {
            size_t r = triangles[map.halfedges[(e % 3 == 2) ? e - 2 : e + 1]];
            if (!seen.count(r)) {
                seen.insert(r);
                LinePolygon poly;

                float mt = map.elevation[r];
                poly.color = getColorFromElevation(mt);

                std::vector<glm::ivec2> vertices = polygonVertices(e);

                for (size_t i = 1; i < vertices.size(); i++) {
                    ColorLine l;
                    l.a = glm::vec2(vertices[0].x, vertices[0].y);
                    l.b = glm::vec2(vertices[i].x, vertices[i].y);
                    
                    poly.lines.push_back(l);
                }
                result.push_back(poly);
            }
        }

        return result;
    }

        // std::vector<ColoredPolygon> result;
        // std::set<int> seen;

        // for (size_t e = 0; e < map.halfedges.size(); e++) {
        //     int r = map.triangles[(e % 3 == 2) ? e - 2 : e + 1]; // аналог nextHalfedge
        //     if (!seen.count(r)) {
        //         seen.insert(r);
        //         ColoredPolygon poly;
        //         poly.vertices = polygonVertices(e);

        //         float mt = map.elevation[r]; // [0..1]
        //         float h; // цвет (hue)
        //         float s = 0.5f; // насыщенность
        //         float l = 0.5f; // светлота

        //         poly.color = getColorFromElevation(mt); //hsl2rgb(h, s, l);
        //         result.push_back(poly);

                // if (e < 0.3f) {
                //     // глубокое море: от синего (240°) к голубому (200°)
                //     float t = e / 0.3f;
                //     h = glm::mix(240.0f/360.0f, 200.0f/360.0f, t);
                //     l = glm::mix(0.3f, 0.4f, t);
                // }
                // else if (e < 0.5f) {
                //     // берег: от голубого (200°) к зелёному (120°)
                //     float t = (e - 0.3f) / 0.2f;
                //     h = glm::mix(200.0f/360.0f, 120.0f/360.0f, t);
                //     l = glm::mix(0.4f, 0.5f, t);
                // }
                // else if (e < 0.7f) {
                //     // равнина: зелёный → жёлтый
                //     float t = (e - 0.5f) / 0.2f;
                //     h = glm::mix(120.0f/360.0f, 60.0f/360.0f, t);
                //     l = glm::mix(0.5f, 0.6f, t);
                // }
                // else {
                //     // горы: жёлтый → белый (можно через серый)
                //     float t = (e - 0.7f) / 0.3f;
                //     h = glm::mix(60.0f/360.0f, 0.0f/360.0f, t);
                //     l = glm::mix(0.6f, 0.9f, t);
                //     s = glm::mix(0.5f, 0.0f, t); // убираем насыщенность, чтобы ушло в серый/белый
                // }
                
            // }
        // }

        // return result;
    // }

    DelaunayMap getMap() const { return map; }

private:
    std::vector<double> coords;
    std::vector<glm::vec2> points;
    std::unique_ptr<delaunator::Delaunator> delaunator;
    DelaunayMap map;

    std::vector<glm::vec2> calculateCentroids(const std::vector<glm::vec2>& points, const DelaunayMap& delaunay) {
        std::vector<glm::vec2> centroids(delaunay.numTriangles);
        for (int t = 0; t < delaunay.numTriangles; t++) {
            glm::vec2 sum(0.0f);
            for (int i = 0; i < 3; i++) {
                int idx = delaunay.triangles[3 * t + i];
                sum += points[idx];
            }
            centroids[t] = sum / 3.0f;
        }
        return centroids;
    }

    std::vector<int> edgesAroundPoint(int start) {
        std::vector<int> result;
        int incoming = start;
        do {
            result.push_back(incoming);
            int outgoing = (incoming % 3 == 2) ? incoming - 2 : incoming + 1; // nextHalfedge
            incoming = map.halfedges[outgoing];
        } while (incoming != -1 && incoming != start);
        return result;
    }

    std::vector<glm::ivec2> polygonVertices(int edge) {
        auto edges = edgesAroundPoint(edge);
        std::vector<glm::ivec2> vertices;
        for (int e : edges) {
            int tri = e / 3; // triangleOfEdge
            vertices.push_back(scalePoint(map.centers[tri])); // scalePoint переводит в целочисленные координаты
        }
        return vertices;
    }
};
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

struct Vec3 {
    float x, y, z;
};

float norme(const Vec3& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float produitScalaire(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 normaliser(const Vec3& v) {
    float n = norme(v);
    return {v.x / n, v.y / n, v.z / n};
}

class Lumiere {
public:
    virtual float intensite(const Vec3& position) const = 0;
    virtual ~Lumiere() = default;
};

class LumiereDirectionnelle : public Lumiere {
    Vec3 direction;
    float couleur;
public:
    LumiereDirectionnelle(const Vec3& dir, float coul) : direction(normaliser(dir)), couleur(coul) {}
    float intensite(const Vec3&) const override {
        return couleur;
    }
};

class LumierePonctuelle : public Lumiere {
    Vec3 position;
    float attenuation;
    float intensiteBase;
public:
    LumierePonctuelle(const Vec3& pos, float att, float base)
        : position(pos), attenuation(att), intensiteBase(base) {}
    float intensite(const Vec3& point) const override {
        float d = norme({point.x - position.x, point.y - position.y, point.z - position.z});
        return intensiteBase / (1 + attenuation * d * d);
    }
};

class LumiereSpot : public Lumiere {
    Vec3 position;
    Vec3 direction;
    float angle;
    float intensiteBase;
public:
    LumiereSpot(const Vec3& pos, const Vec3& dir, float ang, float base)
        : position(pos), direction(normaliser(dir)), angle(ang), intensiteBase(base) {}
    float intensite(const Vec3& point) const override {
        Vec3 dirPoint = {point.x - position.x, point.y - position.y, point.z - position.z};
        dirPoint = normaliser(dirPoint);
        float cosTheta = produitScalaire(direction, dirPoint);
        if (cosTheta > cos(angle)) return intensiteBase * cosTheta;
        return 0.0f;
    }
};

float calculerEclairage(const Vec3& point, const vector<unique_ptr<Lumiere>>& lumieres) {
    float total = 0;
    for (const auto& l : lumieres) total += l->intensite(point);
    return total;
}

int main() {
    vector<unique_ptr<Lumiere>> lumieres;

    lumieres.push_back(make_unique<LumiereDirectionnelle>(Vec3{1, -1, 0}, 0.8f));
    lumieres.push_back(make_unique<LumierePonctuelle>(Vec3{0, 0, 0}, 0.1f, 2.0f));
    lumieres.push_back(make_unique<LumiereSpot>(Vec3{0, 5, 0}, Vec3{0, -1, 0}, 0.5f, 3.0f));

    vector<Vec3> points = { {0, 0, 0}, {1, 2, 0}, {3, 3, 1} };

    for (const auto& p : points) {
        cout << "Eclairage au point (" << p.x << ", " << p.y << ", " << p.z << ") = "
             << calculerEclairage(p, lumieres) << endl;
    }

    return 0;
}

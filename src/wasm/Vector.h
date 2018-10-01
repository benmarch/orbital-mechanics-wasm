#ifndef ORBITAL_MECHANICS_VECTOR_H
#define ORBITAL_MECHANICS_VECTOR_H

typedef struct {
    double a;
    double b;
    double c;
} Vector;

Vector copy(Vector &vec);

double magnitude(Vector &vec);

Vector scale(Vector &vec, double scale);

Vector add(Vector &vec1, Vector &vec2);

Vector subtract(Vector &vec1, Vector &vec2);

double dot(Vector &vec1, Vector &vec2);

Vector cross(Vector &vec1, Vector &vec2);

#endif //ORBITAL_MECHANICS_VECTOR_H

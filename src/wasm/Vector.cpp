#include <math.h>
#include <emscripten/bind.h>

#include "Vector.h"

Vector copy(Vector &vec)
{
    Vector copy;

    copy.a = vec.a;
    copy.b = vec.b;
    copy.c = vec.c;

    return copy;
}

double magnitude(Vector &vec)
{
    return sqrt(pow(vec.a, 2) + pow(vec.b, 2) + pow(vec.c, 2));
}

Vector scale(Vector &vec, double scale)
{
    Vector newVector;

    newVector.a = vec.a * scale;
    newVector.b = vec.b * scale;
    newVector.c = vec.c * scale;

    return newVector;
}

Vector add(Vector &vec1, Vector &vec2)
{
    Vector newVector;

    newVector.a = vec1.a + vec2.a;
    newVector.b = vec1.b + vec2.b;
    newVector.c = vec1.c + vec2.c;

    return newVector;
}

Vector subtract(Vector &vec1, Vector &vec2)
{
    Vector inverseVector2 = scale(vec2, -1);

    return add(vec1, inverseVector2);
}

double dot(Vector &vec1, Vector &vec2)
{
    return vec1.a * vec2.a + vec1.b * vec2.b + vec1.c * vec2.c;
}

Vector cross(Vector &vec1, Vector &vec2)
{
    Vector product;

    product.a = (vec1.b * vec2.c) - (vec1.c * vec2.b);
    product.b = (vec1.c * vec2.a) - (vec1.a * vec2.c);
    product.c = (vec1.a * vec2.b) - (vec1.b * vec2.a);

    return product;
}

EMSCRIPTEN_BINDINGS(vector_bindings) {
        emscripten::value_array<Vector>("Vector")
                .element(&Vector::a)
                .element(&Vector::b)
                .element(&Vector::c);
};

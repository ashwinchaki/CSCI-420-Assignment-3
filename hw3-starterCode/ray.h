#include <glm/glm.hpp>

struct Vertex
{
    double position[3];
    double color_diffuse[3];
    double color_specular[3];
    double normal[3];
    double shininess;
};

struct Triangle
{
    Vertex v[3];
    glm::vec3 normal; // normal of the triangle (NOT normal of vertices)
};

struct Sphere
{
    double position[3];
    double color_diffuse[3];
    double color_specular[3];
    double shininess;
    double radius;
};

struct Light
{
    double position[3];
    double color[3];
};

class Ray
{
  private:
    glm::vec3 origin;
    glm::vec3 direction;

  public:
    Ray(glm::vec3 o, glm::vec3 d)
    {
        origin = o;
        direction = d;
    }

    bool checkIntersectSpheres(Sphere &sphere, glm::vec3 intersect){
        glm::vec3 dist = origin - glm::vec3(sphere.position[0], sphere.position[1], sphere.position[2]);

        double t0 = -10000000;
        double t1 = -10000000;

        double a = glm::dot(direction, direction);
        double b = 2 * glm::dot(direction, dist);
        double c = glm::dot(dist, dist) - pow(sphere.radius, 2);
        double determinant = std::pow(b, 2) - (4 * a * c);

        // base case
        if (determinant < 0){
            return false;
        }

        else if (determinant < 1e-16){
            t0 = -0.5f * b / a;
            t1 = -0.5f * b / a;
        }

        else{
            double q;
            if (b > 0){
                q = -0.5f * (b + sqrt(determinant));
            }
            else{
                q = -0.5f * (b - sqrt(determinant));
            }

            t0 = q / a;
            t1 =  c / q;
        }

        if (t0 < 0 && t1 < 0){
            return false;
        }

        if (t0 > t1 && t1 > 0){
            t0 = t1;
        }

        intersect = origin + (direction * (float)t0);
        return true;

    }
    bool checkIntersectTriangles(Triangle &triangle, glm::vec3 intersect){
        glm::vec3 point_a(triangle.v[0].position[0], triangle.v[0].position[1], triangle.v[0].position[2]);
        glm::vec3 point_b(triangle.v[1].position[0], triangle.v[1].position[1], triangle.v[1].position[2]);
        glm::vec3 point_c(triangle.v[2].position[0], triangle.v[2].position[1], triangle.v[2].position[2]);

        glm::vec3 normal = glm::normalize(glm::cross(point_b - point_a, point_c - point_a));

        double angle = glm::dot(normal, direction);
        
        if (angle < 1e-16 || angle > -1e-16){
            return false;
        }

        glm::vec3 distance = point_a - origin;
        double dist = glm::dot(distance, normal);

        double t = dist / angle;
        if (t < 0){
            return false;
        }

        intersect = origin + (direction * (float)t);

        if (glm::dot(normal, glm::cross(point_b - point_a, intersect - point_a)) < 0
            || glm::dot(normal, glm::cross(point_c - point_b, intersect - point_b)) < 0
            || glm::dot(normal, glm::cross(point_a - point_c, intersect - point_c)) < 0){
                return false;
        }

        return true;

    }
};
#include <cstdlib>
#include "p6/p6.h"
#ifndef SIMPLE_P6_SETUP_BOIDS_H
#define SIMPLE_P6_SETUP_BOIDS_H


class boid{

private :

    float                   m_speedFactor;
    float                   m_baseWidth;
    float                   m_height;
    float                   m_secureArea = m_baseWidth * m_height * 2;
    float                   m_xSpeed;
    float                   m_ySpeed;
    float                   m_xAcceleration{};
    float                   m_yAcceleration{};
    float                   m_xToTarget{};
    float                   m_yToTarget{};
    float                   m_inertiaFactor = 50;
    float                   m_deviateValue = 0.005;
    glm::vec2               m_center     = {0, 0};
    glm::vec2               m_leftPoint{};
    glm::vec2               m_rightPoint{};
    glm::vec2               m_topPoint{};
    glm::vec2               m_targetDirection{};
    float                   m_targetXSpeed = m_xSpeed;
    float                   m_targetYSpeed = m_ySpeed;
    float                   m_detectionRadius;
    std::vector<boid>       m_neighbors;
    p6::Color               m_color;
    unsigned int            m_id;

public:

    // ---------- CONSTRUCTORS

    // Random position boid constructor
    boid(float speed, float deviateValue, float base, float height, float detectionRadius, unsigned int boidId, p6::Context& context): m_speedFactor(speed), m_baseWidth(base), m_height(height), m_xSpeed(p6::random::number(-1,1)), m_ySpeed(p6::random::number(-1, 1)), m_deviateValue(deviateValue), m_detectionRadius(detectionRadius), m_id(boidId){
        m_center    = glm::vec2(p6::random::number(-context.aspect_ratio() + m_secureArea, context.aspect_ratio() - m_secureArea), p6::random::number(-1 + m_secureArea, 1 - m_secureArea));
        setTriangleVertices();
    }

    // Defined position boid constructor
    boid(glm::vec2 center, float speed, float deviateValue, float base, float height, float detectionRadius, unsigned int boidId): m_speedFactor(speed), m_baseWidth(base), m_height(height), m_xSpeed(p6::random::number(-1,1)), m_ySpeed(p6::random::number(-1, 1)), m_deviateValue(deviateValue), m_center(center), m_detectionRadius(detectionRadius), m_id(boidId){
        setTriangleVertices();
    }

    // ---------- METHODS

    // Check if there are neighbor boids around and store them in an array
    void checkNeighbors(std::vector<boid> boids);

    // Get the general direction of the group of neighbors
    void getGroupDirection();

    // Create vertices of the triangle
    void setTriangleVertices();

    // Update the parameters of a boid
    void updateParameters(float speedFactor, float deviateValue, float base, float height, float detectionRadius);

    // Draw the ID of the boid next to it
    void drawID(p6::Context& ctx);

    // Draw the detection area of the boid
    void drawDetectionCircle(p6::Context& ctx);

    // Move the boid according to its parameters
    void boidMovement();

    // Draw a boid
    void draw(p6::Context& ctx, bool displayCircles, bool displayID);
};

std::u16string to_u16string(unsigned int const &value);

void addBoid(std::vector<boid>& boids, glm::vec2 startPos, float speedFactor, float deviateValue, float baseWidth, float height, float detectionRadius);

void displayBoidsNumber(std::vector<boid>& boids, p6::Context& context);


#endif // SIMPLE_P6_SETUP_BOIDS_H

#pragma once
#include <cstdlib>
#include "p6/p6.h"
#ifndef SIMPLE_P6_SETUP_BOIDS_H
#define SIMPLE_P6_SETUP_BOIDS_H


class boid{

private :

    unsigned int            m_id;

    glm::vec2               m_position          = {0, 0};
    glm::vec2               m_speed{};
    float                   m_maxSpeed          = 1.0;
    float                   m_minSpeed          = 0.4;

    float                   m_speedFactor       = 0.0005;

    // Triangle shape
    glm::vec2               m_triangleLeftPoint{};
    glm::vec2               m_triangleRightPoint{};
    glm::vec2               m_triangleTopPoint{};
    float                   m_baseWidth         = 0.025;
    float                   m_height            = 0.035;

    float                   m_secureArea        = m_baseWidth * m_height * 2;

    p6::Color               m_color             = {1.0f, 0.15f, 0.3f};

    float                   m_detectionRadius   = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
    float                   m_avoidanceRadius   = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
    float                   m_detectionFactor   = 2.0;
    float                   m_avoidanceFactor   = 1.5;
    float                   m_separationFactor  = 0.3;
    float                   m_alignmentFactor   = 0.015;
    float                   m_cohesionFactor    = 0.002;
    float                   m_turnSpeedDefault  = 0.15;
    glm::vec2               m_turnSpeed         = {m_turnSpeedDefault, m_turnSpeedDefault};
    float                   m_turnFactor        = 0.15;
    std::vector<boid>       m_neighbors;
    std::vector<boid>       m_closeNeighbors;

public:

    // ---------- CONSTRUCTORS

    // Random position boid constructor
    boid(float speedFactor, float base, float height, float detectionFactor, float avoidanceFactor, unsigned int boidId, p6::Context& context, float windowMargin): m_id(boidId), m_speed(p6::random::number(-1,1), p6::random::number(-1, 1)), m_speedFactor(speedFactor), m_baseWidth(base), m_height(height), m_detectionFactor(detectionFactor), m_avoidanceFactor(avoidanceFactor){
        m_detectionRadius   = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
        m_avoidanceRadius   = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
        m_position = glm::vec2(p6::random::number(-context.aspect_ratio() + windowMargin, context.aspect_ratio() - windowMargin), p6::random::number(-1 + windowMargin, 1 - windowMargin));
        setTriangleVertices();
    }

    // Defined position boid constructor
    boid(glm::vec2 position, float speedFactor, float base, float height, float detectionFactor, float avoidanceFactor, unsigned int boidId): m_id(boidId), m_position(position), m_speed(p6::random::number(-1,1), p6::random::number(-1, 1)), m_speedFactor(speedFactor), m_baseWidth(base), m_height(height), m_detectionFactor(detectionFactor), m_avoidanceFactor(avoidanceFactor){
        m_detectionRadius   = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
        m_avoidanceRadius   = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
        setTriangleVertices();
    }

    // ---------- METHODS

    // Check if there are neighbor boids around and store them in an array
    void checkNeighbors(const std::vector<boid>& boids);

    void separation();

    // Get the general direction of the group of neighbors
    void alignement();

    void cohesion();

    void avoidBoundaries(p6::Context& ctx, float margin);

    void speedLimits();

    // Create vertices of the triangle
    void setTriangleVertices();

    // Update the parameters of a boid
    void updateBoidParameters(float speedFactor, float base, float height, float separationFactor, float alignmentFactor, float cohesionFactor, float detectionFactor, float avoidanceFactor);

    // Draw the ID of the boid next to it
    void drawID(p6::Context& ctx);

    // Draw the detection area of the boid
    void drawDetectionCircle(p6::Context& ctx);

    void drawAvoidanceCircle(p6::Context& ctx);

    void drawNeighborDistance(p6::Context& ctx);

    // Move the boid according to its parameters
    void boidMovement(p6::Context& ctx, float margin);

    // Draw a boid
    void draw(p6::Context& ctx, float margin, bool isDetectionDisplayed, bool isAvoidanceRadiusDisplayed, bool isIdDisplayed, bool isDistanceToNeighborDisplayed);
};

std::u16string to_u16string(unsigned int const &value);

void addBoid(std::vector<boid>& boids, glm::vec2 startPos, float speedFactor, float baseWidth, float height, float detectionFactor, float avoidanceFactor);

void displayBoidsNumber(std::vector<boid>& boids, p6::Context& context);


#endif // SIMPLE_P6_SETUP_BOIDS_H

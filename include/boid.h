#ifndef SIMPLE_P6_SETUP_BOID_H
#define SIMPLE_P6_SETUP_BOID_H

#pragma once
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <random>
#include <string>
#include <vector>
#include "p6/p6.h"
#include "scene.h"

class Scene;

class Boid {
private:
    friend class Scene;

    unsigned int m_id;
    std::string  m_name;

    Scene& m_scene;

    glm::vec2 m_position = {0, 0};
    glm::vec2 m_speed{};
    float     m_maxSpeed = 1.4;
    float     m_minSpeed = 0.4;

    float m_speedFactor = 0.0005;

    float     m_baseWidth = 0.025;
    float     m_height    = 0.035;
    glm::vec2 m_triangleLeftPoint{};
    glm::vec2 m_triangleRightPoint{};
    glm::vec2 m_triangleTopPoint{};

    p6::Color m_color = {1.0f, 0.0f, 0.4f};

    float     m_detectionRadius  = std::fmax(m_height, m_baseWidth);
    float     m_avoidanceRadius  = std::fmax(m_height, m_baseWidth);
    float     m_detectionFactor  = 2.0;
    float     m_avoidanceFactor  = 1.5;
    float     m_separationFactor = 0.3;
    float     m_alignmentFactor  = 0.015;
    float     m_cohesionFactor   = 0.002;
    float     m_turnSpeedDefault = 0.15;
    glm::vec2 m_turnSpeed        = {m_turnSpeedDefault, m_turnSpeedDefault};
    float     m_turnFactor       = 0.15;

    std::vector<Boid> m_neighbors;
    std::vector<Boid> m_closeNeighbors;

    unsigned int m_pinged = 0;
    glm::vec2    m_pingedPosition{};

public:
    // Random position Boid constructor
    Boid(Scene& world, unsigned int boidId)
        : m_id(boidId), m_name(world.m_namesList[m_id]), m_scene(world), m_speed(p6::random::number(-1, 1), p6::random::number(-1, 1)), m_speedFactor(m_scene.m_globalBoidSpeedFactor), m_baseWidth(m_scene.m_globalBoidWidth), m_height(m_scene.m_globalBoidHeight), m_detectionFactor(m_scene.m_globalBoidDetectionRadiusFactor), m_avoidanceFactor(m_scene.m_globalBoidAvoidanceRadiusFactor), m_pingedPosition(m_position) {
        m_detectionRadius = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
        m_avoidanceRadius = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
        m_position        = glm::vec2(p6::random::number(-m_scene.m_context.aspect_ratio(), m_scene.m_context.aspect_ratio()), p6::random::number(-1, 1));
        setTriangleVertices();
    }

    // Defined position Boid constructor
    Boid(Scene& world, glm::vec2 position, unsigned int boidId)
        : m_id(boidId), m_name(world.m_namesList[m_id]), m_scene(world), m_position(position), m_speed(p6::random::number(-1, 1), p6::random::number(-1, 1)), m_speedFactor(m_scene.m_globalBoidSpeedFactor), m_baseWidth(m_scene.m_globalBoidWidth), m_height(m_scene.m_globalBoidHeight), m_detectionFactor(m_scene.m_globalBoidDetectionRadiusFactor), m_avoidanceFactor(m_scene.m_globalBoidAvoidanceRadiusFactor), m_pingedPosition(m_position) {
        m_detectionRadius = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
        m_avoidanceRadius = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
        setTriangleVertices();
    }

private:
    // Check if there are neighbor boids around and store them in an array
    void updateNeighbors();

    void separation();

    // Get the general direction of the group of neighbors
    void alignement();

    void cohesion();

    void avoidBoundaries();

    void avoidPoint(glm::vec2 position, float avoidanceRadius, float avoidanceFactor);

    void followPoint(glm::vec2 position, float followRadius, float followFactor);

    void drawEdgeProjection();

    void speedLimits();

    // Create vertices of the triangle
    void setTriangleVertices();

    // Update the parameters of a Boid
    void updateParameters();

    // Draw the ID of the Boid next to it
    void drawID();

    // Draw the name of the Boid next to it
    void drawName();

    void drawProximityAlert();

    // Draw the detection area of the Boid
    void drawDetectionCircle();

    void drawAvoidanceCircle();

    void drawNeighborRelations();

    void savePosition();

    void updatePosition();

    void drawShape();

    void draw();

    void pingBoid();

    void clearNeighborArrays();
};

std::u16string uint_to_u16string(unsigned int const& value);
std::u16string utf8_to_utf16(std::string const& utf8);

void addBoid(Scene& world, std::vector<Boid>& boids, glm::vec2 startPos);

#endif // SIMPLE_P6_SETUP_BOID_H

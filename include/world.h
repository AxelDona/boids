#ifndef SIMPLE_P6_SETUP_WORLD_H
#define SIMPLE_P6_SETUP_WORLD_H

#pragma once
#include "p6/p6.h"
#include <vector>
#include <cmath>
#include <fstream>

class Boid;

class World {

private:

    p6::Context&                m_context;
    std::vector<Boid>&          m_boids;
    std::vector<std::string>    m_namesList;
    int                m_boidsToSpawn = 50;

    int                    m_theme                          = 0;

    float                  m_worldSpeedFactor               = 0.006;
    float                  m_worldBase                      = 0.025;
    float                  m_worldHeight                    = 0.035;
    float                  m_worldDetectionFactor           = 2.0f;
    float                  m_worldAvoidanceFactor           = 1.5f;
    float                  m_worldSeparationFactor          = 0.3;
    float                  m_worldAlignmentFactor           = 0.015;
    float                  m_worldCohesionFactor            = 0.002;

    bool                   m_isIdDisplayed                  = false;
    bool                   m_isNameDisplayed                = false;
    bool                   m_isDetectionDisplayed           = false;
    bool                   m_isAvoidanceRadiusDisplayed     = false;
    bool                   m_isDistanceToNeighborDisplayed  = false;
    bool                   m_isEdgeProjectionDisplayed      = true;
    bool                   m_pointerInteraction             = false;
    int                    m_pointerInteractionMode         = 0;

    float                  m_windowMargin                   = 0.4;
    float                  m_pointerAvoidanceRadius         = 0.2;

    p6::Color               m_loneColor                     = {0.1f, 0.7f, 0.8f};
    p6::Color               m_followColor                   = {0.1f, 0.7f, 0.1f};
    p6::Color               m_tooCloseColor                 = {1.0f, 0.0f, 0.5f};



public:

    friend class Boid;

    World(p6::Context& ctx, std::vector<Boid>& boids)
        : m_context(ctx), m_boids(boids)
    {
        std::ifstream file("names.txt");
        std::vector<std::string> lines;

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
        } else {
            std::cerr << "Failed to open file\n" << std::endl;
        }

        // Randomize the names order
        auto random = std::random_device {};
        auto rng = std::default_random_engine { random() };
        std::shuffle(std::begin(lines), std::end(lines), rng);

        m_namesList = lines;
    }

    void displayControls();

};


#endif // SIMPLE_P6_SETUP_WORLD_H

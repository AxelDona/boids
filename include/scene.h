#ifndef SIMPLE_P6_SETUP_SCENE_H
#define SIMPLE_P6_SETUP_SCENE_H

#pragma once
#include <cmath>
#include <fstream>
#include <vector>
#include "p6/p6.h"
#include "skin.h"

class Boid;
class Skin;

struct sceneSkinParameters {
    bool m_isIdDisplayed                 = false;
    bool m_isNameDisplayed               = false;
    bool m_isProximityAlertDisplayed     = false;
    bool m_isDetectionDisplayed          = false;
    bool m_isAvoidanceRadiusDisplayed    = false;
    bool m_isDistanceToNeighborDisplayed = false;
    bool m_isEdgeReflectionDisplayed     = true;
    bool m_isBoidsNumberDisplayed        = true;
    bool m_pointerInteraction            = false;
    int  m_pointerInteractionMode        = 0;
};

class Scene {
    friend class Boid;

private:
    p6::Context&             m_context;
    std::vector<Boid>&       m_boids;
    std::vector<Boid>&       m_boidsPreviousFrame;
    std::vector<std::string> m_namesList;
    unsigned int             m_boidsToSpawn = 50;
    int                      m_counter      = 0.0;

    std::vector<Skin>& m_skins;
    unsigned int       m_currentSkinID = 0;
    Skin               m_currentSkin   = m_skins[m_currentSkinID];
    int                m_theme         = 0;
    glm::vec2          m_sonarLineEnd{};
    unsigned int       m_pingDuration = 160;
    unsigned int       m_pingTimer    = 30;

    float m_globalBoidSpeedFactor           = 0.006;
    float m_globalBoidWidth                 = 0.025;
    float m_globalBoidHeight                = 0.035;
    float m_globalBoidDetectionRadiusFactor = 2.0f;
    float m_globalBoidAvoidanceRadiusFactor = 1.5f;
    float m_globalSeparationFactor          = 0.3;
    float m_globalAlignmentFactor           = 0.015;
    float m_globalCohesionFactor            = 0.002;

    sceneSkinParameters m_skinParameters;

    float m_windowMargin           = 0.4;
    float m_pointerAvoidanceRadius = 0.2;

    p6::FullScreen m_screen = {};

public:
    Scene(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Skin>& skins)
        : m_context(ctx), m_boids(boids), m_boidsPreviousFrame(boids), m_skins(skins) {
        std::ifstream            file("names.txt");
        std::vector<std::string> lines;

        if (m_skins.empty()) {
            Skin defaultSkin;
            m_skins.push_back(defaultSkin);
            m_currentSkin = defaultSkin;
        } else {
            m_currentSkin = m_skins[0];
        }

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
        } else {
            std::cerr << "Failed to open file\n"
                      << std::endl;
        }

        // Randomize the names order
        auto random = std::random_device{};
        auto rng    = std::default_random_engine{random()};
        std::shuffle(std::begin(lines), std::end(lines), rng);

        m_namesList = lines;
    }

    size_t getBoidsNumber();

    void displayControls();

    void draw();

private:
    void displayBoidsNumber();

    void updateCurrentSkin();
};

#endif // SIMPLE_P6_SETUP_SCENE_H

#pragma once
#include "p6/p6.h"
#include <cmath>

#ifndef SIMPLE_P6_SETUP_WORLD_H
#define SIMPLE_P6_SETUP_WORLD_H

class World {

private:

    p6::Context&           m_context;

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
    bool                   m_isPointerRepellingBoids        = false;
    bool                   m_isPointerAttractingBoids       = false;

    float                  m_windowMargin                   = 0.4;
    float                  m_pointerAvoidanceRadius         = 0.2;

public:

    friend class Boid;

    explicit World(p6::Context& ctx)
        : m_context(ctx)
    {
    }

    void displayControls(){
        ImGui::Begin("Boids control panel");
        ImGui::Text("Click anywhere to add more boids");
        ImGui::Separator();
        ImGui::Text("Boid shape");
        ImGui::SliderFloat("Speed", &m_worldSpeedFactor, 0.001f, 0.1f);
        ImGui::SliderFloat("Width", &m_worldBase, 0.001f, 0.1f);
        ImGui::SliderFloat("Height", &m_worldHeight, 0.001f, 0.1f);
        ImGui::Separator();
        ImGui::Text("Boid behaviour");
        ImGui::SliderFloat("Separation", &m_worldSeparationFactor, 0.0, 1.0);
        ImGui::SliderFloat("Alignment", &m_worldAlignmentFactor, 0.0, 0.1);
        ImGui::SliderFloat("Cohesion", &m_worldCohesionFactor, 0.0, 0.01);
        ImGui::Checkbox("Boids avoid pointer", &m_isPointerRepellingBoids);
        ImGui::Checkbox("Boids follow pointer (not yet implemented", &m_isPointerAttractingBoids);
        ImGui::Separator();
        ImGui::Text("Boid sight");
        ImGui::SliderFloat("Detection radius", &m_worldDetectionFactor, std::fmax(1.0f, m_worldAvoidanceFactor), 5.0f);
        ImGui::SliderFloat("Avoidance radius", &m_worldAvoidanceFactor, 1.0f, 5.0f);
        ImGui::Separator();
        ImGui::Text("Display options");
        ImGui::Checkbox("Display detection circle", &m_isDetectionDisplayed);
        ImGui::Checkbox("Display collision circle", &m_isAvoidanceRadiusDisplayed);
        ImGui::Checkbox("Display neighbor tension", &m_isDistanceToNeighborDisplayed);
        ImGui::Checkbox("Display ID", &m_isIdDisplayed);
        ImGui::Checkbox("Display name", &m_isNameDisplayed);
        ImGui::Checkbox("Display edge projection", &m_isEdgeProjectionDisplayed);
        ImGui::End();
    }

};


#endif // SIMPLE_P6_SETUP_WORLD_H

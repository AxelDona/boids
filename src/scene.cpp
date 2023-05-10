#include "scene.h"
#include <cmath>
#include <iostream>
#include "boid.h"
#include "skin.h"

size_t Scene::getBoidsNumber() {
    return m_boids.size();
}

void Scene::displayControls() {
    ImGuiStyle& style      = ImGui::GetStyle();
    style.GrabRounding     = 5;
    style.WindowRounding   = 5;
    style.FrameRounding    = 3;
    style.WindowPadding    = ImVec2(10, 10);
    style.ItemSpacing.y    = 6;
    style.WindowTitleAlign = ImVec2(0.50, 0.50);
    // ImGui::ShowDemoWindow();
    ImGui::Begin("BOIDWALKER | Boids control panel");
    ImGui::PushItemWidth(100);
    ImGui::SliderInt("", reinterpret_cast<int*>(&m_boidsToSpawn), 1, 400);
    ImGui::SameLine(120);
    ImGui::PushItemWidth(180);
    if (m_boids.size() + m_boidsToSpawn <= 4000) {
        std::string spawnLabelString = "Spawn " + std::to_string(m_boidsToSpawn) + " boids";
        const char* spawnLabel       = spawnLabelString.c_str();
        if (ImGui::Button(spawnLabel)) {
            for (unsigned int i = 0; i < m_boidsToSpawn; i++) {
                Boid singleBoid(*this, m_boids.size());
                m_boids.push_back(singleBoid);
            }
        };
    } else {
        std::string spawnLabelString = "Cannot display more than 4000 boids";
        const char* spawnLabel       = spawnLabelString.c_str();
        if (ImGui::Button(spawnLabel)) {
        };
    }
    if (ImGui::Button("Clear all boids")) {
        m_boids.clear();
    };
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
    if (m_boids.size() < 4000) {
        ImGui::Text("Click anywhere to add more boids");
    } else {
        ImGui::Text("Maximum amount of boids reached");
    }
    ImGui::Separator();
    ImGui::Text("Skin");
    int skinRadioButtonOffset = 0;
    for (unsigned int i = 0; i < m_skins.size(); i++) {
        if (i % 3 != 0 && i != 0) {
            ImGui::SameLine((static_cast<float>(skinRadioButtonOffset) + 1.0f) * 100);
            skinRadioButtonOffset++;
        } else {
            skinRadioButtonOffset = 0;
        }
        ImGui::RadioButton(m_skins[i].m_name, reinterpret_cast<int*>(&m_currentSkinID), static_cast<int>(i));
    }
    ImGui::Separator();
    ImGui::Text("Boid shape");
    ImGui::PushItemWidth(140);
    ImGui::SliderFloat("Speed", &m_globalBoidSpeedFactor, 0.001f, 0.1f);
    if (m_currentSkin.m_boidShape == Skin::Triangle) {
        ImGui::SliderFloat("Width", &m_globalBoidWidth, 0.001f, 0.1f);
        ImGui::SliderFloat("Height", &m_globalBoidHeight, 0.001f, 0.1f);
    } else {
        ImGui::SliderFloat("Size", &m_globalBoidWidth, 0.001f, 0.1f);
    }
    ImGui::Separator();
    ImGui::Text("Boid behaviour");
    ImGui::SliderFloat("Separation", &m_globalSeparationFactor, 0.0, 1.0);
    ImGui::SliderFloat("Alignment", &m_globalAlignmentFactor, 0.0, 0.1);
    ImGui::SliderFloat("Cohesion", &m_globalCohesionFactor, 0.0, 0.01);
    ImGui::Separator();
    ImGui::Checkbox("Pointer interaction", &m_pointerInteraction);
    if (m_pointerInteraction) {
        ImGui::RadioButton("Pointer repels boids", &m_pointerInteractionMode, 0);
        ImGui::RadioButton("Pointer attracts boids (not yet implemented)", &m_pointerInteractionMode, 1);
    }
    ImGui::Separator();
    ImGui::Text("Boid sight");
    ImGui::SliderFloat("Detection radius", &m_globalBoidDetectionRadiusFactor, std::fmax(1.0f, m_globalBoidAvoidanceRadiusFactor), 8.0f);
    ImGui::SliderFloat("Avoidance radius", &m_globalBoidAvoidanceRadiusFactor, 1.0f, 5.0f);
    ImGui::Separator();
    ImGui::Text("Display options");
    ImGui::Checkbox("Display boids count", &m_isBoidsNumberDisplayed);
    if (m_currentSkin.m_areDistanceDisplayOptionsEnabled) {
        ImGui::Checkbox("Display detection circle", &m_isDetectionDisplayed);
        ImGui::Checkbox("Display collision circle", &m_isAvoidanceRadiusDisplayed);
        ImGui::Checkbox("Display relations", &m_isDistanceToNeighborDisplayed);
    }
    ImGui::Checkbox("Display ID", &m_isIdDisplayed);
    if (m_currentSkin.m_areBoidNameOptionsEnabled) {
        ImGui::Checkbox("Display name", &m_isNameDisplayed);
    }
    if (m_currentSkin.m_isEdgeReflectionOptionEnabled) {
        ImGui::Checkbox("Display edge reflection", &m_isEdgeReflectionDisplayed);
    }
    if (m_currentSkin.m_isProximityAlertOptionEnabled) {
        ImGui::Checkbox("Draw proximity alert", &m_isProximityAlertDisplayed);
    }
    ImGui::End();
}

void Scene::displayBoidsNumber() {
    float              margin  = 0.05;
    p6::TopRightCorner textPos = {m_context.aspect_ratio() - margin, 1 - margin};
    std::u16string     text    = uint_to_u16string(m_boids.size());
    m_context.text_inflating   = 0.02f;
    m_context.text_size        = 0.03f;
    if (m_currentSkin.m_isMonochrome) {
        m_context.fill = m_currentSkin.m_sceneSecondaryColor;
    } else {
        m_context.fill = {1.0f, 1.0f, 1.0f, 0.7f};
    }
    m_context.text(text, textPos);
}

void Scene::updateCurrentSkin() {
    if (m_currentSkin.m_id != m_currentSkinID) {
        m_currentSkin                     = m_skins[m_currentSkinID];
        m_globalBoidSpeedFactor           = m_currentSkin.m_defaultBoidSpeedFactor;
        m_globalBoidWidth                 = m_currentSkin.m_defaultBoidWidth;
        m_globalBoidDetectionRadiusFactor = m_currentSkin.m_defaultDetectionRadiusFactor;
        m_globalBoidAvoidanceRadiusFactor = 1.5f;
        m_globalSeparationFactor          = 0.3;
        m_globalAlignmentFactor           = 0.015;
        m_globalCohesionFactor            = 0.002;
        m_isIdDisplayed                   = m_currentSkin.m_isIdDisplayedByDefault;
        m_isNameDisplayed                 = m_currentSkin.m_isNameDisplayedByDefault;
        m_isProximityAlertDisplayed       = m_currentSkin.m_isProximityAlertDisplayedByDefault;
        m_isDetectionDisplayed            = m_currentSkin.m_isDetectionDisplayedByDefault;
        m_isAvoidanceRadiusDisplayed      = m_currentSkin.m_isAvoidanceRadiusDisplayedByDefault;
        m_isDistanceToNeighborDisplayed   = m_currentSkin.m_isDistanceToNeighborDisplayedByDefault;
        m_isEdgeReflectionDisplayed       = m_currentSkin.m_isEdgeReflectionDisplayedByDefault;
        m_isBoidsNumberDisplayed          = m_currentSkin.m_isBoidsNumberDisplayedByDefault;
        m_pointerInteraction              = m_currentSkin.m_isPointerInteractionEnabledByDefault;
        m_pointerInteractionMode          = 0;

        for (auto& m_boid : m_boids) {
            m_boid.m_pinged         = 0;
            m_boid.m_pingedPosition = m_boid.m_position;
        }
    }
}

void Scene::draw() {
    updateCurrentSkin();
    if (m_currentSkin.m_isBackgroundFading) {
        m_context.use_stroke = false;
        m_context.use_fill   = true;
        m_context.fill       = m_currentSkin.m_sceneBackgroundColor;
        m_context.rectangle(m_screen);
    } else {
        m_context.background(m_currentSkin.m_sceneBackgroundColor);
    }
    if (m_currentSkin.m_usesRadarLayout) {
        m_context.use_stroke    = true;
        m_context.use_fill      = false;
        m_context.stroke_weight = 0.002f;
        m_context.stroke        = m_currentSkin.m_sceneSecondaryColor;
        m_context.circle(glm::vec2(0, 0), 0.25);
        m_context.circle(glm::vec2(0, 0), 0.75);
        m_context.circle(glm::vec2(0, 0), 1.25);
        m_context.circle(glm::vec2(0, 0), 1.75);
        m_context.line(glm::vec2(0, -1), glm::vec2(0, 1));
        m_context.line(glm::vec2(m_context.aspect_ratio(), 0), glm::vec2(-m_context.aspect_ratio(), 0));
    }
    if (m_currentSkin.m_boidPositionRefresh == Skin::OnSonarPing) {
        m_sonarLineEnd          = glm::vec2(m_context.aspect_ratio() * 2, 0);
        m_sonarLineEnd.x        = m_context.aspect_ratio() * 2.0f * cos(-m_counter * 0.01);
        m_sonarLineEnd.y        = m_context.aspect_ratio() * 2.0f * sin(-m_counter * 0.01);
        m_context.stroke_weight = 0.004f;
        m_context.line(glm::vec2(0, 0), m_sonarLineEnd);
    }

    for (auto& m_boid : m_boids) {
        m_boid.updateParameters();
        m_boid.updateNeighbors();
        m_boid.updatePosition();
        if (m_currentSkin.m_boidPositionRefresh == Skin::RealTime) {
            m_boid.draw();
        } else {
            m_boid.pingBoid();
            if (m_boid.m_pinged > 0) {
                m_boid.draw();
            }
        }
        m_boid.clearNeighborArrays();
    }

    if (m_isBoidsNumberDisplayed) {
        displayBoidsNumber();
    }

    m_counter += 1;
}
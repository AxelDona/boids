#include "world.h"
#include "boids.h"

void World::displayControls(){
    ImGui::Begin("Boids control panel");
    ImGui::PushItemWidth(100);
    ImGui::SliderInt("", &m_boidsToSpawn, 1, 400);
    ImGui::SameLine(120);
    ImGui::PushItemWidth(180);
    std::string spawnLabelString = "Spawn " + std::to_string(m_boidsToSpawn) + " boids";
    const char* spawnLabel = spawnLabelString.c_str();
    if (ImGui::Button(spawnLabel)){
        for (unsigned int i = 0; i < m_boidsToSpawn; i++){
            Boid singleBoid(*this, m_boids.size());
            m_boids.push_back(singleBoid);
        }
    };
    if (ImGui::Button("Clear all boids")){
        m_boids.clear();
    };
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
    ImGui::Text("Click anywhere to add more boids");
    ImGui::Separator();
    ImGui::Text("Theme");
    ImGui::RadioButton("Classic", &m_theme, 0); ImGui::SameLine(100); ImGui::RadioButton("Radar", &m_theme, 1);
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
    ImGui::Separator();
    ImGui::Checkbox("Pointer interaction", &m_pointerInteraction);
    if (m_pointerInteraction){
        ImGui::RadioButton("Pointer repels boids", &m_pointerInteractionMode, 0);
        ImGui::RadioButton("Pointer attracts boids", &m_pointerInteractionMode, 1);
    }
    ImGui::Separator();
    ImGui::Text("Boid sight");
    ImGui::SliderFloat("Detection radius", &m_worldDetectionFactor, std::fmax(1.0f, m_worldAvoidanceFactor), 5.0f);
    ImGui::SliderFloat("Avoidance radius", &m_worldAvoidanceFactor, 1.0f, 5.0f);
    ImGui::Separator();
    ImGui::Text("Display options");
    ImGui::Checkbox("Display detection circle", &m_isDetectionDisplayed);
    ImGui::Checkbox("Display collision circle", &m_isAvoidanceRadiusDisplayed);
    ImGui::Checkbox("Display relations", &m_isDistanceToNeighborDisplayed);
    ImGui::Checkbox("Display ID", &m_isIdDisplayed);
    ImGui::Checkbox("Display name", &m_isNameDisplayed);
    ImGui::Checkbox("Display edge reflection", &m_isEdgeProjectionDisplayed);
    ImGui::End();
}
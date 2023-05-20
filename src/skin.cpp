#include "skin.h"

std::vector<Skin> initSkins() {
    skinDefaultDisplayOptions nodesDefaultDisplayParameters         = {false, false, false, false, false, true, true, true, false, 0};
    skinDefaultDisplayOptions shootingStarsDefaultDisplayParameters = {false, false, false, false, false, false, true, true, false, 0};
    skinDefaultDisplayOptions radarDefaultDisplayParameters         = {true, false, true, false, false, false, false, true, false, 0};
    skinDefaultDisplayOptions sonarDefaultDisplayParameters         = {false, false, false, false, false, false, false, true, false, 0};

    Skin              defaultSkin;
    Skin              nodes("Nodes", 0.015, 0.007, 5.0, nodesDefaultDisplayParameters, true, false, Skin::BoidShape::Dot, Skin::BoidPositionRefresh::RealTime, true, false, true, false, false, p6::Color{0.85f, 0.05f, 0.40f}, p6::Color{0.70f, 0.10f, 0.25f}, p6::Color{0.06f, 0.01f, 0.02f, 0.05f});
    Skin              shootingStars("Shooting Stars", 0.009, 0.005, 3.0, shootingStarsDefaultDisplayParameters, true, false, Skin::BoidShape::Dot, Skin::BoidPositionRefresh::RealTime, false, false, true, false, true, p6::Color{0.80f, 0.95f, 1.0f}, p6::Color{0.50f, 0.60f, 0.60f}, p6::Color{0.07f, 0.08f, 0.08f, 0.025f});
    Skin              radar("Radar", 0.001, 0.025, 2.0, radarDefaultDisplayParameters, true, true, Skin::BoidShape::Cross, Skin::BoidPositionRefresh::OnTimer, true, false, false, true, false, p6::Color{0.7f, 0.45f, 0.25f}, p6::Color{0.5f, 0.35f, 0.15f}, p6::Color{0.05f, 0.02f, 0.0f, 1.0f});
    Skin              sonar("Sonar", 0.006, 0.025, 2.0, sonarDefaultDisplayParameters, true, true, Skin::BoidShape::Dot, Skin::BoidPositionRefresh::OnSonarPing, false, false, false, false, true, p6::Color{0.5f, 0.75f, 0.2f}, p6::Color{0.3f, 0.6f, 0.1f}, p6::Color{0.05f, 0.1f, 0.05f, 0.05f});
    std::vector<Skin> skinsList{defaultSkin, nodes, shootingStars, radar, sonar};
    for (size_t i = 0; i < skinsList.size(); i++) {
        skinsList[i].setSkinID(i);
    }
    return skinsList;
}
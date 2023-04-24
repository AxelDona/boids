#include "skin.h"

std::vector<Skin> initSkins() {
    Skin              defaultSkin;
    Skin              radar("Radar", 0.001, false, false, true, false, false, false, false, true, false, 0, true, true, Skin::BoidShape::Cross, Skin::BoidPositionRefresh::OnTimer, true, false, false, true, false, p6::Color{0.7f, 0.45f, 0.25f}, p6::Color{0.5f, 0.35f, 0.15f}, p6::Color{0.05f, 0.02f, 0.0f, 1.0f});
    Skin              sonar("Sonar", 0.06, false, false, false, false, false, false, false, true, false, 0, true, true, Skin::BoidShape::Dot, Skin::BoidPositionRefresh::OnSonarPing, false, false, false, false, true, p6::Color{0.5f, 0.75f, 0.2f}, p6::Color{0.3f, 0.6f, 0.1f}, p6::Color{0.05f, 0.1f, 0.05f, 0.05f});
    std::vector<Skin> skinsList{defaultSkin, radar, sonar};
    for (size_t i = 0; i < skinsList.size(); i++) {
        skinsList[i].setSkinID(i);
    }
    return skinsList;
}
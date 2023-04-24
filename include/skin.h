#ifndef SIMPLE_P6_SETUP_SKIN_H
#define SIMPLE_P6_SETUP_SKIN_H

#pragma once
#include <utility>
#include "p6/p6.h"

class Scene;
class Boid;

class Skin {
public:
    enum BoidShape { Triangle,
                     Cross,
                     Dot };

    enum BoidPositionRefresh {
        RealTime,
        OnSonarPing,
        OnTimer
    };

private:
    friend class Scene;
    friend class Boid;

    const char*  m_name = "Default";
    unsigned int m_id   = 0;

    float m_defaultBoidSpeedFactor = 0.006;

    bool m_isIdDisplayedByDefault                 = false;
    bool m_isNameDisplayedByDefault               = false;
    bool m_isProximityAlertDisplayedByDefault     = false;
    bool m_isDetectionDisplayedByDefault          = false;
    bool m_isAvoidanceRadiusDisplayedByDefault    = false;
    bool m_isDistanceToNeighborDisplayedByDefault = false;
    bool m_isEdgeReflectionDisplayedByDefault     = true;
    bool m_isBoidsNumberDisplayedByDefault        = true;
    bool m_isPointerInteractionEnabledByDefault   = false;
    int  m_pointerInteractionModeByDefault        = 0;

    bool                m_isMonochrome                     = false;
    bool                m_usesRadarLayout                  = false;
    BoidShape           m_boidShape                        = Triangle;
    BoidPositionRefresh m_boidPositionRefresh              = RealTime;
    bool                m_areDistanceDisplayOptionsEnabled = true;
    bool                m_areBoidNameOptionsEnabled        = true;
    bool                m_isEdgeReflectionOptionEnabled    = true;
    bool                m_isProximityAlertOptionEnabled    = false;
    bool                m_isBackgroundFading               = false;

    p6::Color m_loneBoidColor        = {0.1f, 0.7f, 0.8f};
    p6::Color m_followingBoidColor   = {0.1f, 0.7f, 0.1f};
    p6::Color m_tooCloseBoidColor    = {1.0f, 0.0f, 0.5f};
    p6::Color m_sceneMainColor       = {0.75f, 0.5f, 0.2f};
    p6::Color m_sceneSecondaryColor  = {0.3f, 0.6f, 0.1f};
    p6::Color m_sceneBackgroundColor = {0.15f, 0.15f, 0.2f};

public:
    Skin() = default;

    Skin(const char* name, float defaultBoidSpeedFactor, bool isIdDisplayedByDefault, bool isNameDisplayedByDefault, bool isProximityAlertDisplayedByDefault, bool isDetectionDisplayedByDefault, bool isAvoidanceRadiusDisplayedByDefault, bool isDistanceToNeighborDisplayedByDefault, bool isEdgeReflectionDisplayedByDefault, bool isBoidsNumberDisplayedByDefault, bool isPointerInteractionEnabledByDefault, int pointerInteractionModeByDefault, bool isMonochrome, bool usesRadarLayout, BoidShape boidShape, BoidPositionRefresh boidPositionRefresh, bool areDistanceDisplayOptionsEnabled, bool areBoidNameOptionsEnabled, bool isEdgeReflectionOptionEnabled, bool isProximityAlertOptionEnabled, bool isBackgroundFading, p6::Color sceneMainColor, p6::Color sceneSecondaryColor, p6::Color sceneBackgroundColor)
        : m_name(name), m_defaultBoidSpeedFactor(defaultBoidSpeedFactor), m_isIdDisplayedByDefault(isIdDisplayedByDefault), m_isNameDisplayedByDefault(isNameDisplayedByDefault), m_isProximityAlertDisplayedByDefault(isProximityAlertDisplayedByDefault), m_isDetectionDisplayedByDefault(isDetectionDisplayedByDefault), m_isAvoidanceRadiusDisplayedByDefault(isAvoidanceRadiusDisplayedByDefault), m_isDistanceToNeighborDisplayedByDefault(isDistanceToNeighborDisplayedByDefault), m_isEdgeReflectionDisplayedByDefault(isEdgeReflectionDisplayedByDefault), m_isBoidsNumberDisplayedByDefault(isBoidsNumberDisplayedByDefault), m_isPointerInteractionEnabledByDefault(isPointerInteractionEnabledByDefault), m_pointerInteractionModeByDefault(pointerInteractionModeByDefault), m_isMonochrome(isMonochrome), m_usesRadarLayout(usesRadarLayout), m_boidShape(boidShape), m_boidPositionRefresh(boidPositionRefresh), m_areDistanceDisplayOptionsEnabled(areDistanceDisplayOptionsEnabled), m_areBoidNameOptionsEnabled(areBoidNameOptionsEnabled), m_isEdgeReflectionOptionEnabled(isEdgeReflectionOptionEnabled), m_isProximityAlertOptionEnabled(isProximityAlertOptionEnabled), m_isBackgroundFading(isBackgroundFading), m_sceneMainColor(sceneMainColor), m_sceneSecondaryColor(sceneSecondaryColor), m_sceneBackgroundColor(sceneBackgroundColor) {
        if (m_isMonochrome) {
            m_loneBoidColor = m_followingBoidColor = m_tooCloseBoidColor = m_sceneMainColor;
        }
    }

    Skin(const char* name, float defaultBoidSpeedFactor, bool isIdDisplayedByDefault, bool isNameDisplayedByDefault, bool isProximityAlertDisplayedByDefault, bool isDetectionDisplayedByDefault, bool isAvoidanceRadiusDisplayedByDefault, bool isDistanceToNeighborDisplayedByDefault, bool isEdgeReflectionDisplayedByDefault, bool isBoidsNumberDisplayedByDefault, bool isPointerInteractionEnabledByDefault, int pointerInteractionModeByDefault, bool isMonochrome, bool usesRadarLayout, BoidShape boidShape, BoidPositionRefresh boidPositionRefresh, bool areDistanceDisplayOptionsDisabled, bool areBoidNameOptionsEnabled, bool isEdgeReflectionOptionEnabled, bool isProximityAlertOptionEnabled, bool isBackgroundFading, p6::Color loneBoidColor, p6::Color followingBoidColor, p6::Color tooCloseBoidColor, p6::Color sceneMainColor, p6::Color sceneSecondaryColor, p6::Color sceneBackgroundColor)
        : m_name(name), m_defaultBoidSpeedFactor(defaultBoidSpeedFactor), m_isIdDisplayedByDefault(isIdDisplayedByDefault), m_isNameDisplayedByDefault(isNameDisplayedByDefault), m_isProximityAlertDisplayedByDefault(isProximityAlertDisplayedByDefault), m_isDetectionDisplayedByDefault(isDetectionDisplayedByDefault), m_isAvoidanceRadiusDisplayedByDefault(isAvoidanceRadiusDisplayedByDefault), m_isDistanceToNeighborDisplayedByDefault(isDistanceToNeighborDisplayedByDefault), m_isEdgeReflectionDisplayedByDefault(isEdgeReflectionDisplayedByDefault), m_isBoidsNumberDisplayedByDefault(isBoidsNumberDisplayedByDefault), m_isPointerInteractionEnabledByDefault(isPointerInteractionEnabledByDefault), m_pointerInteractionModeByDefault(pointerInteractionModeByDefault), m_isMonochrome(isMonochrome), m_usesRadarLayout(usesRadarLayout), m_boidShape(boidShape), m_boidPositionRefresh(boidPositionRefresh), m_areDistanceDisplayOptionsEnabled(areDistanceDisplayOptionsDisabled), m_areBoidNameOptionsEnabled(areBoidNameOptionsEnabled), m_isEdgeReflectionOptionEnabled(isEdgeReflectionOptionEnabled), m_isProximityAlertOptionEnabled(isProximityAlertOptionEnabled), m_isBackgroundFading(isBackgroundFading), m_loneBoidColor(loneBoidColor), m_followingBoidColor(followingBoidColor), m_tooCloseBoidColor(tooCloseBoidColor), m_sceneMainColor(sceneMainColor), m_sceneSecondaryColor(sceneSecondaryColor), m_sceneBackgroundColor(sceneBackgroundColor) {
        if (m_isMonochrome) {
            m_loneBoidColor = m_followingBoidColor = m_tooCloseBoidColor = m_sceneMainColor;
        }
    }

    void setSkinID(unsigned int skinID) { m_id = skinID; }
};

std::vector<Skin> initSkins();

#endif // SIMPLE_P6_SETUP_SKIN_H

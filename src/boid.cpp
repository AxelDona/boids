#include "boid.h"
#include <cmath>

// ---------- METHODS

// Check if there are neighbor boids around and store them in an array
void Boid::updateNeighbors() {
    for (const auto& boid : m_scene.m_boids) {
        if (glm::distance(m_position, boid.m_position) <= m_detectionRadius * 2 && m_id != boid.m_id) {
            m_neighbors.push_back(boid);
        }
    }
}

void Boid::separation() {
    for (auto& m_neighbor : m_neighbors) {
        if (glm::distance(m_position, m_neighbor.m_position) < m_avoidanceRadius) {
            m_closeNeighbors.push_back(m_neighbor);
            m_speed += (m_position - m_neighbor.m_position) * m_separationFactor;
        }
    }
}

// Get the general direction of the group of neighbors
void Boid::alignement() {
    glm::vec2 averageGroupSpeed{};
    for (auto& m_neighbor : m_neighbors) {
        averageGroupSpeed += m_neighbor.m_speed;
    }
    averageGroupSpeed.x = averageGroupSpeed.x / static_cast<float>(m_neighbors.size());
    averageGroupSpeed.y = averageGroupSpeed.y / static_cast<float>(m_neighbors.size());
    m_speed += (averageGroupSpeed - m_speed) * m_alignmentFactor;
}

void Boid::cohesion() {
    glm::vec2 averagePosition{};
    for (auto& m_neighbor : m_neighbors) {
        averagePosition += m_neighbor.m_position;
    }
    averagePosition.x = averagePosition.x / static_cast<float>(m_neighbors.size());
    averagePosition.y = averagePosition.y / static_cast<float>(m_neighbors.size());
    m_speed += (averagePosition - m_speed) * m_cohesionFactor;
}

void Boid::avoidBoundaries() {
    if (m_position.x < -m_scene.m_context.aspect_ratio() + m_scene.m_windowMargin) {
        m_turnSpeed.x = (-(m_position.x - (-m_scene.m_context.aspect_ratio() + m_scene.m_windowMargin))) * m_turnFactor;
        m_speed.x     = m_speed.x + m_turnSpeed.x;
    } else if (m_position.x > m_scene.m_context.aspect_ratio() - m_scene.m_windowMargin) {
        m_turnSpeed.x = (m_position.x - (m_scene.m_context.aspect_ratio() - m_scene.m_windowMargin)) * m_turnFactor;
        m_speed.x     = m_speed.x - m_turnSpeed.x;
    } else {
        m_turnSpeed.x = m_turnSpeedDefault;
    }
    if (m_position.y > 1 - m_scene.m_windowMargin) {
        m_turnSpeed.y = (m_position.y - (1 - m_scene.m_windowMargin)) * m_turnFactor;
        m_speed.y     = m_speed.y - m_turnSpeed.y;
    } else if (m_position.y < -1 + m_scene.m_windowMargin) {
        m_turnSpeed.y = -((m_position.y - (-1 + m_scene.m_windowMargin))) * m_turnFactor;
        m_speed.y     = m_speed.y + m_turnSpeed.y;
    } else {
        m_turnSpeed.y = m_turnSpeedDefault;
    }
}

void Boid::avoidPoint(glm::vec2 position, float avoidanceRadius, float avoidanceFactor) {
    if (glm::distance(m_position, position) < avoidanceRadius) {
        if (m_position.x < position.x + avoidanceRadius && m_position.x > position.x) {
            m_turnSpeed.x = (-(m_position.x - (position.x + avoidanceRadius))) * m_turnFactor * avoidanceFactor;
            m_speed.x     = m_speed.x + m_turnSpeed.x;
        } else if (m_position.x > position.x - avoidanceRadius && m_position.x < position.x) {
            m_turnSpeed.x = (m_position.x - (position.x - avoidanceRadius)) * m_turnFactor * avoidanceFactor;
            m_speed.x     = m_speed.x - m_turnSpeed.x;
        } else {
            m_turnSpeed.x = m_turnSpeedDefault;
        }

        if (m_position.y < position.y + avoidanceRadius && m_position.y > position.y) {
            m_turnSpeed.y = (-(m_position.y - (position.y + avoidanceRadius))) * m_turnFactor * avoidanceFactor;
            m_speed.y     = m_speed.y + m_turnSpeed.y;
        } else if (m_position.y > position.y - avoidanceRadius && m_position.y < position.y) {
            m_turnSpeed.y = (m_position.y - (position.y - avoidanceRadius)) * m_turnFactor * avoidanceFactor;
            m_speed.y     = m_speed.y - m_turnSpeed.y;
        } else {
            m_turnSpeed.y = m_turnSpeedDefault;
        }
    }
}

void Boid::followPoint(glm::vec2 position, float followRadius, float followFactor) {
    if (glm::distance(m_position, position) < followRadius) {
    }
}

void Boid::drawEdgeProjection() {
    float circleRadius              = 0.5;
    float boundaryAlertCircleOffset = circleRadius - 0.02f;
    m_scene.m_context.use_stroke    = false;
    m_scene.m_context.use_fill      = true;
    m_scene.m_context.fill          = m_color;

    if (m_position.x < -m_scene.m_context.aspect_ratio() + m_scene.m_windowMargin) {
        m_scene.m_context.fill.a() = (m_position.x - (-m_scene.m_context.aspect_ratio() + m_scene.m_windowMargin)) / -m_scene.m_context.aspect_ratio();
        m_scene.m_context.circle(glm::vec2(-m_scene.m_context.aspect_ratio() - boundaryAlertCircleOffset, m_position.y), circleRadius);
    } else if (m_position.x > m_scene.m_context.aspect_ratio() - m_scene.m_windowMargin) {
        m_scene.m_context.fill.a() = (m_position.x - (m_scene.m_context.aspect_ratio() - m_scene.m_windowMargin)) / m_scene.m_context.aspect_ratio();
        m_scene.m_context.circle(glm::vec2(m_scene.m_context.aspect_ratio() + boundaryAlertCircleOffset, m_position.y), circleRadius);
    }

    if (m_position.y > 1 - m_scene.m_windowMargin) {
        m_scene.m_context.fill.a() = (m_position.y - (1 - m_scene.m_windowMargin)) / 1;
        m_scene.m_context.circle(glm::vec2(m_position.x, 1 + boundaryAlertCircleOffset), circleRadius);
    } else if (m_position.y < -1 + m_scene.m_windowMargin) {
        m_scene.m_context.fill.a() = (m_position.y - (-1 + m_scene.m_windowMargin)) / -1;
        m_scene.m_context.circle(glm::vec2(m_position.x, -1 - boundaryAlertCircleOffset), circleRadius);
    }
}

void Boid::speedLimits() {
    float totalSpeed = std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y);
    if (totalSpeed > m_maxSpeed) {
        m_speed.x = (m_speed.x / totalSpeed) * m_maxSpeed;
        m_speed.y = (m_speed.y / totalSpeed) * m_maxSpeed;
    }
    if (totalSpeed < m_minSpeed) {
        m_speed.x = (m_speed.x / totalSpeed) * m_minSpeed;
        m_speed.y = (m_speed.y / totalSpeed) * m_minSpeed;
    }
}

// Create vertices of the triangle
void Boid::setTriangleVertices() {
    m_triangleLeftPoint  = {-m_height / 2, m_baseWidth / 2};
    m_triangleRightPoint = {-m_height / 2, -m_baseWidth / 2};
    m_triangleTopPoint   = {m_height / 2, 0};
}

// Update the parameters of a Boid
void Boid::updateParameters() {
    m_speedFactor      = m_scene.m_globalBoidSpeedFactor;
    m_baseWidth        = m_scene.m_globalBoidWidth;
    m_height           = m_scene.m_globalBoidHeight;
    m_separationFactor = m_scene.m_globalSeparationFactor;
    m_alignmentFactor  = m_scene.m_globalAlignmentFactor;
    m_cohesionFactor   = m_scene.m_globalCohesionFactor;
    m_avoidanceFactor  = m_scene.m_globalBoidAvoidanceRadiusFactor;
    m_detectionFactor  = m_scene.m_globalBoidDetectionRadiusFactor;
    m_avoidanceRadius  = std::fmax(m_height, m_baseWidth) * m_avoidanceFactor;
    m_detectionRadius  = std::fmax(m_height, m_baseWidth) * m_detectionFactor;
    setTriangleVertices();
    if (m_detectionRadius <= m_avoidanceRadius) {
        m_detectionRadius = m_avoidanceRadius;
    }
}

// Draw the ID of the Boid next to it
void Boid::drawID() {
    std::u16string text = uint_to_u16string(m_id);
    if (m_scene.m_currentSkin.m_isMonochrome) {
        m_scene.m_context.fill = m_scene.m_currentSkin.m_sceneMainColor;
    } else {
        m_scene.m_context.fill = {0.5f, 0.5f, 0.5f};
    }
    m_scene.m_context.text_inflating = 0.006f;
    m_scene.m_context.text_size      = 0.013f;
    if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::RealTime) {
        p6::BottomLeftCorner textPos = m_position + glm::vec2{m_height / 2, m_height / 2};
        m_scene.m_context.text(text, textPos);
    } else {
        p6::BottomLeftCorner textPos = m_pingedPosition + glm::vec2{m_height / 2, m_height / 2};
        m_scene.m_context.text(text, textPos);
    }
}

// Draw the name of the Boid next to it
void Boid::drawName() {
    p6::TopLeftCorner textPos        = m_position + glm::vec2{m_height, -m_height / 2};
    std::u16string    text           = utf8_to_utf16(m_name);
    m_scene.m_context.fill           = {1.0f, 1.0f, 1.0f, 0.5f};
    m_scene.m_context.text_inflating = 0.008f;
    m_scene.m_context.text_size      = 0.015f;
    m_scene.m_context.text(text, textPos);
}

void Boid::drawProximityAlert() {
    m_scene.m_context.fill           = {0.8f, 0.2f, 0.2f};
    m_scene.m_context.text_inflating = 0.006f;
    m_scene.m_context.text_size      = 0.013f;
    glm::vec2 alertPosition{};
    if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::RealTime) {
        alertPosition = m_position;
    } else {
        alertPosition = m_pingedPosition;
    }
    p6::BottomLeftCorner textPos = alertPosition + glm::vec2{(m_height / 2), 0 - (m_scene.m_context.text_size / 2)};
    std::u16string       text    = u"WARNING";
    m_scene.m_context.text(text, textPos);
}

// Draw the detection area of the Boid
void Boid::drawDetectionCircle() {
    m_scene.m_context.use_stroke    = true;
    m_scene.m_context.use_fill      = false;
    m_scene.m_context.stroke_weight = 0.0027f;
    m_scene.m_context.stroke        = {0.1f, 0.5f, 0.1f, 0.7f};
    m_scene.m_context.circle(
        m_position,
        m_detectionRadius
    );
}

void Boid::drawAvoidanceCircle() {
    m_scene.m_context.use_stroke    = true;
    m_scene.m_context.use_fill      = false;
    m_scene.m_context.stroke_weight = 0.0027f;
    m_scene.m_context.stroke        = {0.5f, 0.1f, 0.1f, 0.7f};
    m_scene.m_context.circle(
        m_position,
        m_avoidanceRadius
    );
}

void Boid::drawNeighborRelations() {
    if (m_neighbors.empty()) {
        return;
    }
    for (auto& m_neighbor : m_neighbors) {
        m_scene.m_context.stroke_weight = 0.0005f;
        if (m_scene.m_currentSkin.m_isMonochrome) {
            m_scene.m_context.stroke = m_scene.m_currentSkin.m_sceneSecondaryColor;
        } else {
            m_scene.m_context.stroke = {1.0f, 1.0f, 1.0f, 0.5f};
        }
        m_scene.m_context.line(
            m_position,
            m_neighbor.m_position
        );
    }
}

// Move the Boid according to its parameters
void Boid::updatePosition() {
    if (!m_neighbors.empty()) {
        separation();
        alignement();
        cohesion();
    }
    if (!ImGui::GetIO().WantCaptureMouse) {
        if (m_scene.m_pointerInteraction) {
            if (m_scene.m_pointerInteractionMode == 0) {
                avoidPoint(m_scene.m_context.mouse(), m_scene.m_pointerAvoidanceRadius, 4.0);
            } else if (m_scene.m_pointerInteractionMode == 1) {
                followPoint(m_scene.m_context.mouse(), m_scene.m_pointerAvoidanceRadius, 2.0);
            }
        }
    }
    avoidBoundaries();
    speedLimits();
    m_position = m_position + (m_speed * m_speedFactor);
}

void Boid::savePosition() {
    m_pingedPosition = m_position;
}

void Boid::clearNeighborArrays() {
    m_neighbors.clear();
    m_closeNeighbors.clear();
}

void Boid::drawShape() {
    glm::vec2 positionToDraw;
    if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::RealTime) {
        positionToDraw = m_position;
    } else {
        positionToDraw = m_pingedPosition;
    }
    if (m_scene.m_currentSkin.m_boidShape == Skin::Triangle) {
        m_scene.m_context.use_stroke    = true;
        m_scene.m_context.use_fill      = false;
        m_scene.m_context.stroke_weight = 0.004f;
        m_scene.m_context.stroke        = m_color;
        m_scene.m_context.triangle(
            m_triangleLeftPoint,
            m_triangleRightPoint,
            m_triangleTopPoint,
            positionToDraw,
            p6::Angle(glm::normalize(m_speed))
        );
    } else if (m_scene.m_currentSkin.m_boidShape == Skin::Cross) {
        m_scene.m_context.use_stroke    = true;
        m_scene.m_context.use_fill      = false;
        m_scene.m_context.stroke_weight = 0.002f;
        m_scene.m_context.stroke        = m_color;
        m_scene.m_context.line(glm::vec2(positionToDraw.x - m_baseWidth / 1.5, positionToDraw.y), glm::vec2(positionToDraw.x + m_baseWidth / 1.5, positionToDraw.y));
        m_scene.m_context.line(glm::vec2(positionToDraw.x, positionToDraw.y - m_baseWidth / 1.5), glm::vec2(positionToDraw.x, positionToDraw.y + m_baseWidth / 1.5));
    } else if (m_scene.m_currentSkin.m_boidShape == Skin::Dot) {
        float dotSize = NAN;
        if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::OnSonarPing) {
            dotSize = static_cast<float>(m_pinged) * 0.0001f;
        } else {
            dotSize = m_baseWidth;
        }
        m_scene.m_context.use_stroke = false;
        m_scene.m_context.use_fill   = true;
        m_scene.m_context.fill       = m_color;
        m_scene.m_context.circle(
            positionToDraw,
            dotSize
        );
    }
}

void Boid::pingBoid() {
    if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::OnTimer) {
        m_pinged = 1;
        if (m_scene.m_counter % (m_scene.m_pingTimer + static_cast<int>(std::floor((m_position.x + m_scene.m_context.aspect_ratio()) * 20 / m_scene.m_context.aspect_ratio()))) == 0) {
            m_pingedPosition = m_position;
        }
    } else if (m_scene.m_currentSkin.m_boidPositionRefresh == Skin::OnSonarPing) {
        glm::vec2 linePos            = m_scene.m_sonarLineEnd;
        float     relevantLineLength = NAN;
        relevantLineLength           = glm::distance(glm::vec2(0, 0), m_position);
        linePos                      = (relevantLineLength / glm::length(linePos)) * linePos;
        if (glm::distance(m_position, linePos) < m_baseWidth / 2) {
            if (m_pinged == 0) {
                savePosition();
                m_pinged = m_scene.m_pingDuration;
            }
        } else {
            if (m_pinged > 0 && glm::distance(m_position, linePos) > m_baseWidth * 4) {
                m_pinged--;
            }
        }
    }
}

// Draw a Boid
void Boid::draw() {
    if (!m_closeNeighbors.empty()) {
        m_color = m_scene.m_currentSkin.m_tooCloseBoidColor;
    } else if (!m_neighbors.empty()) {
        m_color = m_scene.m_currentSkin.m_followingBoidColor;
    } else {
        m_color = m_scene.m_currentSkin.m_loneBoidColor;
    }

    drawShape();

    // Draw detection circle
    if (m_scene.m_isDetectionDisplayed) {
        drawDetectionCircle();
    }

    if (m_scene.m_isAvoidanceRadiusDisplayed) {
        drawAvoidanceCircle();
    }

    if (m_scene.m_isDistanceToNeighborDisplayed) {
        drawNeighborRelations();
    }

    // Write ID
    if (m_scene.m_isIdDisplayed) {
        drawID();
    }

    // Write name
    if (m_scene.m_isNameDisplayed) {
        drawName();
    }

    if (!m_closeNeighbors.empty() && m_scene.m_isProximityAlertDisplayed) {
        drawProximityAlert();
    }

    if (m_scene.m_isEdgeReflectionDisplayed) {
        drawEdgeProjection();
    }
}

std::u16string uint_to_u16string(unsigned int const& value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(std::to_string(value));
}

std::u16string utf8_to_utf16(std::string const& utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::codecvt_mode::little_endian>, char16_t> cnv;
    std::u16string                                                                                                output = cnv.from_bytes(utf8);
    if (cnv.converted() < utf8.size()) {
        throw std::runtime_error("incomplete conversion");
    }
    return output;
}

void addBoid(Scene& world, std::vector<Boid>& boids, glm::vec2 startPos) {
    Boid singularBoid(world, startPos, boids.size());
    boids.push_back(singularBoid);
}

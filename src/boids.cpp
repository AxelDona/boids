#include "boids.h"

#include <cmath>

// ---------- METHODS

// Check if there are neighbor boids around and store them in an array
void boid::checkNeighbors(const std::vector<boid>& boids){
    for (const auto & boid : boids){
        if (glm::distance(m_position, boid.m_position) <= m_detectionRadius*2 && m_id != boid.m_id){
            m_neighbors.push_back(boid);
        }
    }
}

void boid::separation(){
    for (auto & m_neighbor : m_neighbors){
        if (glm::distance(m_position, m_neighbor.m_position) < m_avoidanceRadius){
            m_closeNeighbors.push_back(m_neighbor);
            m_speed += (m_position - m_neighbor.m_position) * m_separationFactor;
        }
    }
}

// Get the general direction of the group of neighbors
void boid::alignement(){
    glm::vec2 averageGroupSpeed{};
    for (auto & m_neighbor : m_neighbors){
        averageGroupSpeed += m_neighbor.m_speed;
    }
    averageGroupSpeed.x = averageGroupSpeed.x / static_cast<float>(m_neighbors.size());
    averageGroupSpeed.y = averageGroupSpeed.y / static_cast<float>(m_neighbors.size());
    m_speed += (averageGroupSpeed - m_speed) * m_alignmentFactor;
}

void boid::cohesion(){
    glm::vec2 averagePosition{};
    for (auto & m_neighbor : m_neighbors){
        averagePosition += m_neighbor.m_position;
    }
    averagePosition.x = averagePosition.x / static_cast<float>(m_neighbors.size());
    averagePosition.y = averagePosition.y / static_cast<float>(m_neighbors.size());
    m_speed += (averagePosition - m_speed) * m_cohesionFactor;
}

void boid::avoidBoundaries(p6::Context& ctx, float margin){
    if (m_position.x < -ctx.aspect_ratio() + margin){
        m_turnSpeed.x = (- (m_position.x - (-ctx.aspect_ratio() + margin))) * m_turnFactor;
        m_speed.x = m_speed.x + m_turnSpeed.x;
    } else if (m_position.x > ctx.aspect_ratio() - margin){
        m_turnSpeed.x = (m_position.x - (ctx.aspect_ratio() - margin)) * m_turnFactor;
        m_speed.x = m_speed.x - m_turnSpeed.x;
    } else {
        m_turnSpeed.x = m_turnSpeedDefault;
    }
    if (m_position.y > 1 - margin){
        m_turnSpeed.y = (m_position.y - (1 - margin)) * m_turnFactor;
        m_speed.y = m_speed.y - m_turnSpeed.y;
    } else if (m_position.y < - 1 + margin){
        m_turnSpeed.y = - ((m_position.y - (- 1 + margin))) * m_turnFactor;
        m_speed.y = m_speed.y + m_turnSpeed.y;
    } else {
        m_turnSpeed.y = m_turnSpeedDefault;
    }
}

void boid::speedLimits(){
    float totalSpeed = std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y);
    if (totalSpeed > m_maxSpeed){
        m_speed.x = (m_speed.x / totalSpeed) * m_maxSpeed;
        m_speed.y = (m_speed.y / totalSpeed) * m_maxSpeed;
    }
    if (totalSpeed < m_minSpeed){
        m_speed.x = (m_speed.x / totalSpeed) * m_minSpeed;
        m_speed.y = (m_speed.y / totalSpeed) * m_minSpeed;
    }
}

// Create vertices of the triangle
void boid::setTriangleVertices(){
    m_triangleLeftPoint = {-m_height/2, m_baseWidth/2};
    m_triangleRightPoint = {-m_height/2, -m_baseWidth/2};
    m_triangleTopPoint   = {m_height/2, 0};
}

// Update the parameters of a boid
void boid::updateBoidParameters(float speedFactor, float base, float height, float separationFactor, float alignmentFactor, float cohesionFactor, float detectionFactor, float avoidanceFactor){
    m_speedFactor = speedFactor;
    m_baseWidth = base;
    m_height = height;
    m_separationFactor = separationFactor;
    m_alignmentFactor  = alignmentFactor;
    m_cohesionFactor = cohesionFactor;
    m_avoidanceFactor = avoidanceFactor;
    m_detectionFactor = detectionFactor;
    m_avoidanceRadius = std::fmax(m_height, m_baseWidth)*m_avoidanceFactor;
    m_detectionRadius = std::fmax(m_height, m_baseWidth)*m_detectionFactor;
    setTriangleVertices();
    if (m_detectionRadius <= m_avoidanceRadius){
        m_detectionRadius = m_avoidanceRadius;
    }
}

// Draw the ID of the boid next to it
void boid::drawID(p6::Context& ctx){
    p6::BottomLeftCorner textPos = m_position + glm::vec2{m_height/2, m_height/2};
    std::u16string text = to_u16string(m_id);
    ctx.text_inflating = 0.008f;
    ctx.text_size = 0.015f;
    ctx.text(text, textPos);
}

// Draw the detection area of the boid
void boid::drawDetectionCircle(p6::Context& ctx){
    ctx.stroke_weight = 0.0027f;
    ctx.stroke = {0.1f, 0.5f, 0.1f, 0.7f};
    ctx.circle(
        m_position,
        m_detectionRadius
    );
}

void boid::drawAvoidanceCircle(p6::Context& ctx){
    ctx.stroke_weight = 0.0027f;
    ctx.stroke = {0.5f, 0.1f, 0.1f, 0.7f};
    ctx.circle(
        m_position,
        m_avoidanceRadius
    );
}

void boid::drawNeighborDistance(p6::Context& ctx){
    if (m_neighbors.empty()){
        return;
    }
    for (auto & m_neighbor : m_neighbors){
        ctx.stroke_weight = 0.0005f;
        ctx.stroke = {1.0f, 1.0f, 1.0f, 0.5f};
        ctx.line(
            m_position,
            m_neighbor.m_position
        );
    }
}

// Move the boid according to its parameters
void boid::boidMovement(p6::Context& ctx, float margin){
    if(!m_neighbors.empty()){
        separation();
        alignement();
        cohesion();
    }
    avoidBoundaries(ctx, margin);
    speedLimits();
    m_position = m_position + (m_speed * m_speedFactor);
}

// Draw a boid
void boid::draw(p6::Context& ctx, float margin, bool isDetectionDisplayed, bool isAvoidanceRadiusDisplayed, bool isIdDisplayed, bool isDistanceToNeighborDisplayed){

    // Draw the triangle
    ctx.use_stroke = true;
    ctx.use_fill = false;
    ctx.stroke_weight = 0.004f;
    ctx.stroke = m_color;
    ctx.triangle(
        m_triangleLeftPoint,
        m_triangleRightPoint,
        m_triangleTopPoint,
        m_position,
        p6::Angle(glm::normalize(m_speed))
    );

    // Draw detection circle
    if (isDetectionDisplayed){
        drawDetectionCircle(ctx);
    }

    if (isAvoidanceRadiusDisplayed){
        drawAvoidanceCircle(ctx);
    }

    // Draw ID
    if (isIdDisplayed){
        drawID(ctx);
    }

    if (isDistanceToNeighborDisplayed){
        drawNeighborDistance(ctx);
    }

    // Move triangle
    boidMovement(ctx, margin);

    // Get color for next frame
    if (!m_closeNeighbors.empty()){
        m_color = {0.7f, 0.2f, 0.7f};
    } else if (!m_neighbors.empty()){
        m_color = {0.2f, 0.8f, 0.3f};
    } else  {
        m_color = {1.0f, 0.15f, 0.3f};
    }

    // Reset array of neighbors
    m_neighbors.clear();
    m_closeNeighbors.clear();
}

std::u16string to_u16string(unsigned int const &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(std::to_string(value));
}

void addBoid(std::vector<boid>& boids, glm::vec2 startPos, float speedFactor, float baseWidth, float height, float detectionFactor, float avoidanceFactor){
    boid singularBoid(startPos, speedFactor, baseWidth, height, detectionFactor, avoidanceFactor, boids.size());
    boids.push_back(singularBoid);
}

void displayBoidsNumber(std::vector<boid>& boids, p6::Context& context){
    float margin = 0.1;
    p6::TopRightCorner textPos = {context.aspect_ratio() - margin, 1 - margin};
    std::u16string text = to_u16string(boids.size());
    context.text_inflating = 0.02f;
    context.text_size = 0.03f;
    context.text(text, textPos);
}


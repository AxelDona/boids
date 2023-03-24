#include "boids.h"

// ---------- METHODS

// Check if there are neighbor boids around and store them in an array
void boid::checkNeighbors(std::vector<boid> boids){
    for (size_t i = 0; i < boids.size(); i++){
        if (glm::distance(m_center, boids[i].m_center) <= m_detectionRadius*2 && m_id != boids[i].m_id){
            m_neighbors.push_back(boids[i]);
        }
    }
    if (!m_neighbors.empty()){
        m_color = {0.2f, 0.8f, 0.3f};
    } else {
        m_color = {1.0f, 0.15f, 0.3f};
    }
}

// Get the general direction of the group of neighbors
void boid::getGroupDirection(){
    m_targetXSpeed = m_xSpeed;
    m_targetYSpeed = m_ySpeed;
    if (m_neighbors.empty()){
        return;
    }
    for (size_t i = 0; i < m_neighbors.size(); i++){
        m_targetXSpeed += m_neighbors[i].m_xSpeed;
        m_targetYSpeed += m_neighbors[i].m_ySpeed;
    }
    m_targetXSpeed = m_targetXSpeed / static_cast<float>(m_neighbors.size() + 1);
    m_targetYSpeed = m_targetYSpeed / static_cast<float>(m_neighbors.size() + 1);
}

// Create vertices of the triangle
void boid::setTriangleVertices(){
    m_leftPoint = {-m_height/2, m_baseWidth/2};
    m_rightPoint = {-m_height/2, -m_baseWidth/2};
    m_topPoint = {m_height/2, 0};
}

// Update the parameters of a boid
void boid::updateParameters(float speedFactor, float deviateValue, float base, float height, float detectionRadius){
    m_speedFactor = speedFactor;
    m_deviateValue = deviateValue;
    m_baseWidth = base;
    m_height = height;
    setTriangleVertices();
    if (detectionRadius >= m_height){
        m_detectionRadius = detectionRadius;
    } else {
        m_detectionRadius = m_height;
    }
}

// Draw the ID of the boid next to it
void boid::drawID(p6::Context& ctx){
    p6::BottomLeftCorner textPos = m_center + glm::vec2{m_height/2, m_height/2};
    std::u16string text = to_u16string(m_id);
    ctx.text_inflating = 0.008f;
    ctx.text_size = 0.015f;
    ctx.text(text, textPos);
}

// Draw the detection area of the boid
void boid::drawDetectionCircle(p6::Context& ctx){
    ctx.stroke_weight = 0.0027f;
    ctx.stroke = {0.1f, 0.1f, 0.1f, 0.6f};
    ctx.circle(
        m_center,
        m_detectionRadius
    );
}

// Move the boid according to its parameters
void boid::boidMovement(){
    m_xToTarget = m_targetXSpeed - m_xSpeed;
    m_yToTarget = m_targetYSpeed - m_ySpeed;
    m_xSpeed += (m_xToTarget / m_inertiaFactor) + p6::random::number(-m_deviateValue, m_deviateValue);
    m_ySpeed += (m_yToTarget / m_inertiaFactor) + p6::random::number(-m_deviateValue, m_deviateValue);
    m_center += glm::normalize(glm::vec2(m_xSpeed, m_ySpeed)) * m_speedFactor;
}

// Draw a boid
void boid::draw(p6::Context& ctx, bool displayCircles, bool displayID){

    // Draw boid
    ctx.use_stroke = true;
    ctx.use_fill = false;
    ctx.stroke_weight = 0.004f;
    ctx.stroke = m_color;
    ctx.triangle(
        m_leftPoint,
        m_rightPoint,
        m_topPoint,
        m_center,
        p6::Angle(glm::normalize(glm::vec2(m_xSpeed, m_ySpeed)))
    );

    // Draw detection circle
    if (displayCircles){
        drawDetectionCircle(ctx);
    }

    // Draw ID
    if (displayID){
        drawID(ctx);
    }

    // Find target direction from neighbors
    getGroupDirection();

    // Move triangle
    boidMovement();

    // Check if within window limits and teleport
    if (m_center[0] < -ctx.aspect_ratio()){
        m_center[0] = ctx.aspect_ratio();
    }
    if (m_center[0] > ctx.aspect_ratio()){
        m_center[0] = - ctx.aspect_ratio();
    }
    if (m_center[1] > 1){
        m_center[1] = - 1;
    }
    if (m_center[1] < - 1){
        m_center[1] = 1;
    }
    m_neighbors.clear();
}

std::u16string to_u16string(unsigned int const &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(std::to_string(value));
}

void addBoid(std::vector<boid>& boids, glm::vec2 startPos, float speedFactor, float deviateValue, float baseWidth, float height, float detectionRadius){
    boid oneboid(startPos, speedFactor, deviateValue, baseWidth, height, detectionRadius, boids.size());
    boids.push_back(oneboid);
}

void displayBoidsNumber(std::vector<boid>& boids, p6::Context& context){
    float margin = 0.1;
    p6::TopRightCorner textPos = {context.aspect_ratio() - margin, 1 - margin};
    std::u16string text = to_u16string(boids.size());
    context.text_inflating = 0.02f;
    context.text_size = 0.03f;
    context.text(text, textPos);
}


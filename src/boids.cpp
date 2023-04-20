#include "boids.h"

// ---------- METHODS

// Check if there are neighbor boids around and store them in an array
void Boid::checkNeighbors(const std::vector<Boid>& boids){
    for (const auto & boid : boids){
        if (glm::distance(m_position, boid.m_position) <= m_detectionRadius*2 && m_id != boid.m_id){
            m_neighbors.push_back(boid);
        }
    }
}

void Boid::separation(){
    for (auto & m_neighbor : m_neighbors){
        if (glm::distance(m_position, m_neighbor.m_position) < m_avoidanceRadius){
            m_closeNeighbors.push_back(m_neighbor);
            m_speed += (m_position - m_neighbor.m_position) * m_separationFactor;
        }
    }
}

// Get the general direction of the group of neighbors
void Boid::alignement(){
    glm::vec2 averageGroupSpeed{};
    for (auto & m_neighbor : m_neighbors){
        averageGroupSpeed += m_neighbor.m_speed;
    }
    averageGroupSpeed.x = averageGroupSpeed.x / static_cast<float>(m_neighbors.size());
    averageGroupSpeed.y = averageGroupSpeed.y / static_cast<float>(m_neighbors.size());
    m_speed += (averageGroupSpeed - m_speed) * m_alignmentFactor;
}

void Boid::cohesion(){
    glm::vec2 averagePosition{};
    for (auto & m_neighbor : m_neighbors){
        averagePosition += m_neighbor.m_position;
    }
    averagePosition.x = averagePosition.x / static_cast<float>(m_neighbors.size());
    averagePosition.y = averagePosition.y / static_cast<float>(m_neighbors.size());
    m_speed += (averagePosition - m_speed) * m_cohesionFactor;
}

void Boid::avoidBoundaries(){
    if (m_position.x < -m_world.m_context.aspect_ratio() + m_world.m_windowMargin){
        m_turnSpeed.x = (- (m_position.x - (-m_world.m_context.aspect_ratio() + m_world.m_windowMargin))) * m_turnFactor;
        m_speed.x = m_speed.x + m_turnSpeed.x;
    } else if (m_position.x > m_world.m_context.aspect_ratio() - m_world.m_windowMargin){
        m_turnSpeed.x = (m_position.x - (m_world.m_context.aspect_ratio() - m_world.m_windowMargin)) * m_turnFactor;
        m_speed.x = m_speed.x - m_turnSpeed.x;
    } else {
        m_turnSpeed.x = m_turnSpeedDefault;
    }
    if (m_position.y > 1 - m_world.m_windowMargin){
        m_turnSpeed.y = (m_position.y - (1 - m_world.m_windowMargin)) * m_turnFactor;
        m_speed.y = m_speed.y - m_turnSpeed.y;
    } else if (m_position.y < - 1 + m_world.m_windowMargin){
        m_turnSpeed.y = - ((m_position.y - (- 1 + m_world.m_windowMargin))) * m_turnFactor;
        m_speed.y = m_speed.y + m_turnSpeed.y;
    } else {
        m_turnSpeed.y = m_turnSpeedDefault;
    }
}

void Boid::avoidPoint(glm::vec2 position, float avoidanceRadius, float avoidanceFactor){
    if (glm::distance(m_position, position) < avoidanceRadius){

        if (m_position.x < position.x + avoidanceRadius && m_position.x > position.x){
            m_turnSpeed.x = (- (m_position.x - (position.x + avoidanceRadius))) * m_turnFactor * avoidanceFactor;
            m_speed.x = m_speed.x + m_turnSpeed.x;
        } else if (m_position.x > position.x - avoidanceRadius  && m_position.x < position.x){
            m_turnSpeed.x = (m_position.x - (position.x - avoidanceRadius)) * m_turnFactor * avoidanceFactor;
            m_speed.x = m_speed.x - m_turnSpeed.x;
        } else {
            m_turnSpeed.x = m_turnSpeedDefault;
        }

        if (m_position.y < position.y + avoidanceRadius && m_position.y > position.y){
            m_turnSpeed.y = (- (m_position.y - (position.y + avoidanceRadius))) * m_turnFactor * avoidanceFactor;
            m_speed.y = m_speed.y + m_turnSpeed.y;
        } else if (m_position.y > position.y - avoidanceRadius && m_position.y < position.y){
            m_turnSpeed.y = (m_position.y - (position.y - avoidanceRadius)) * m_turnFactor * avoidanceFactor;
            m_speed.y = m_speed.y - m_turnSpeed.y;
        } else {
            m_turnSpeed.y = m_turnSpeedDefault;
        }
    }
}

void Boid::followPoint(glm::vec2 position, float followRadius, float followFactor){
    if (glm::distance(m_position, position) < followRadius){

    }
}

void Boid::drawEdgeProjection(){
    float circleRadius = 0.5;
    float boundaryAlertCircleOffset = circleRadius - 0.02f;
    m_world.m_context.use_stroke = false;
    m_world.m_context.use_fill = true;
    m_world.m_context.fill = m_color;

    if (m_position.x < -m_world.m_context.aspect_ratio() + m_world.m_windowMargin){
        m_world.m_context.fill.a() = (m_position.x - (- m_world.m_context.aspect_ratio() + m_world.m_windowMargin)) / - m_world.m_context.aspect_ratio();
        m_world.m_context.circle(glm::vec2(-m_world.m_context.aspect_ratio() - boundaryAlertCircleOffset, m_position.y), circleRadius);
    } else if (m_position.x > m_world.m_context.aspect_ratio() - m_world.m_windowMargin){
        m_world.m_context.fill.a() = (m_position.x - (m_world.m_context.aspect_ratio() - m_world.m_windowMargin)) / m_world.m_context.aspect_ratio();
        m_world.m_context.circle(glm::vec2(m_world.m_context.aspect_ratio() + boundaryAlertCircleOffset, m_position.y), circleRadius);
    }

    if (m_position.y > 1 - m_world.m_windowMargin){
        m_world.m_context.fill.a() = (m_position.y - (1 - m_world.m_windowMargin)) / 1;
        m_world.m_context.circle(glm::vec2(m_position.x, 1 + boundaryAlertCircleOffset), circleRadius);
    } else if (m_position.y < - 1 + m_world.m_windowMargin){
        m_world.m_context.fill.a() = (m_position.y - (-1 + m_world.m_windowMargin)) / -1;
        m_world.m_context.circle(glm::vec2( m_position.x, -1 - boundaryAlertCircleOffset), circleRadius);
    }
}

void Boid::speedLimits(){
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
void Boid::setTriangleVertices(){
    m_triangleLeftPoint = {-m_height/2, m_baseWidth/2};
    m_triangleRightPoint = {-m_height/2, -m_baseWidth/2};
    m_triangleTopPoint   = {m_height/2, 0};
}

// Update the parameters of a Boid
void Boid::updateBoidParameters(){
    m_speedFactor = m_world.m_worldSpeedFactor;
    m_baseWidth = m_world.m_worldBase;
    m_height = m_world.m_worldHeight;
    m_separationFactor = m_world.m_worldSeparationFactor;
    m_alignmentFactor  = m_world.m_worldAlignmentFactor;
    m_cohesionFactor = m_world.m_worldCohesionFactor;
    m_avoidanceFactor = m_world.m_worldAvoidanceFactor;
    m_detectionFactor = m_world.m_worldDetectionFactor;
    m_avoidanceRadius = std::fmax(m_height, m_baseWidth)*m_avoidanceFactor;
    m_detectionRadius = std::fmax(m_height, m_baseWidth)*m_detectionFactor;
    setTriangleVertices();
    if (m_detectionRadius <= m_avoidanceRadius){
        m_detectionRadius = m_avoidanceRadius;
    }
}

// Draw the ID of the Boid next to it
void Boid::drawID(){
    p6::BottomLeftCorner textPos = m_position + glm::vec2{m_height/2, m_height/2};
    std::u16string text = uint_to_u16string(m_id);
    m_world.m_context.fill = {1.0f, 1.0f, 1.0f, 0.5f};
    m_world.m_context.text_inflating = 0.008f;
    m_world.m_context.text_size = 0.015f;
    m_world.m_context.text(text, textPos);
}

// Draw the name of the Boid next to it
void Boid::drawName(){
    p6::TopLeftCorner textPos = m_position + glm::vec2{m_height, - m_height/2 };
    std::u16string text = utf8_to_utf16(m_name);
    m_world.m_context.fill = {1.0f, 1.0f, 1.0f, 0.5f};
    m_world.m_context.text_inflating = 0.008f;
    m_world.m_context.text_size = 0.015f;
    m_world.m_context.text(text, textPos);
}

// Draw the detection area of the Boid
void Boid::drawDetectionCircle(){
    m_world.m_context.stroke_weight = 0.0027f;
    m_world.m_context.stroke = {0.1f, 0.5f, 0.1f, 0.7f};
    m_world.m_context.circle(
        m_position,
        m_detectionRadius
    );
}

void Boid::drawAvoidanceCircle(){
    m_world.m_context.stroke_weight = 0.0027f;
    m_world.m_context.stroke = {0.5f, 0.1f, 0.1f, 0.7f};
    m_world.m_context.circle(
        m_position,
        m_avoidanceRadius
    );
}

void Boid::drawNeighborDistance(){
    if (m_neighbors.empty()){
        return;
    }
    for (auto & m_neighbor : m_neighbors){
        m_world.m_context.stroke_weight = 0.0005f;
        m_world.m_context.stroke = {1.0f, 1.0f, 1.0f, 0.5f};
        m_world.m_context.line(
            m_position,
            m_neighbor.m_position
        );
    }
}

// Move the Boid according to its parameters
void Boid::boidMovement(){
    if(!m_neighbors.empty()){
        separation();
        alignement();
        cohesion();
    }
    if (m_world.m_pointerInteraction){
        if (m_world.m_pointerInteractionMode == 0){
            avoidPoint(m_world.m_context.mouse(), m_world.m_pointerAvoidanceRadius, 4.0);
        } else if (m_world.m_pointerInteractionMode == 1){
            followPoint(m_world.m_context.mouse(), m_world.m_pointerAvoidanceRadius, 2.0);
            }
    }
    avoidBoundaries();
    speedLimits();
    m_position = m_position + (m_speed * m_speedFactor);
}

// Draw a Boid
void Boid::draw(){

    // Draw the triangle
    m_world.m_context.use_stroke = true;
    m_world.m_context.use_fill = false;
    m_world.m_context.stroke_weight = 0.004f;
    m_world.m_context.stroke = m_color;
    m_world.m_context.triangle(
        m_triangleLeftPoint,
        m_triangleRightPoint,
        m_triangleTopPoint,
        m_position,
        p6::Angle(glm::normalize(m_speed))
    );

    // Draw detection circle
    if (m_world.m_isDetectionDisplayed){
        drawDetectionCircle();
    }

    if (m_world.m_isAvoidanceRadiusDisplayed){
        drawAvoidanceCircle();
    }

    // Write ID
    if (m_world.m_isIdDisplayed){
        drawID();
    }

    // Write name
    if (m_world.m_isNameDisplayed){
        drawName();
    }

    if (m_world.m_isDistanceToNeighborDisplayed){
        drawNeighborDistance();
    }

    if (m_world.m_isEdgeProjectionDisplayed){
        drawEdgeProjection();
    }

    // Move triangle
    boidMovement();

    // Get color for next frame
    if (!m_closeNeighbors.empty()){
        m_color = m_world.m_tooCloseColor;
    } else if (!m_neighbors.empty()){
        m_color = m_world.m_followColor;
    } else  {
        m_color = m_world.m_loneColor;
    }

    // Reset array of neighbors
    m_neighbors.clear();
    m_closeNeighbors.clear();
}

std::u16string uint_to_u16string(unsigned int const &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(std::to_string(value));
}

std::u16string utf8_to_utf16(std::string const& utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
                                                 std::codecvt_mode::little_endian>, char16_t> cnv;
    std::u16string output = cnv.from_bytes(utf8);
    if (cnv.converted() < utf8.size()) {
        throw std::runtime_error("incomplete conversion");
    }
    return output;
}

void addBoid(World& world, std::vector<Boid>& boids, glm::vec2 startPos){
    Boid singularBoid(world, startPos, boids.size());
    boids.push_back(singularBoid);
}

void displayBoidsNumber(std::vector<Boid>& boids, p6::Context& ctx){
    float margin = 0.05;
    p6::TopRightCorner textPos = {ctx.aspect_ratio() - margin, 1 - margin};
    std::u16string text = uint_to_u16string(boids.size());
    ctx.fill = {1.0f, 1.0f, 1.0f, 0.7f};
    ctx.text_inflating = 0.02f;
    ctx.text_size = 0.03f;
    ctx.text(text, textPos);
}


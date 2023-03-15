#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <string>

std::u16string to_u16string(unsigned int const &value) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
    return conv.from_bytes(std::to_string(value));
}

class boid{

private :

    unsigned int            m_id;
    float                   m_speedFactor;
    float                   m_inertiaFactor = 50;
    float                   m_deviateValue = 0.005;
    float                   m_baseWidth;
    float                   m_height;
    float                   m_secureArea = m_baseWidth * m_height * 2;
    float                   m_xSpeed;
    float                   m_ySpeed;
    float                   m_xAcceleration;
    float                   m_yAcceleration;
    float                   m_xToTarget;
    float                   m_yToTarget;
    glm::vec2               m_center     = {0, 0};
    glm::vec2               m_leftPoint;
    glm::vec2               m_rightPoint;
    glm::vec2               m_topPoint;
    glm::vec2               m_targetDirection;
    float                   m_targetXSpeed = m_xSpeed;
    float                   m_targetYSpeed = m_ySpeed;
    float                   m_detectionRadius;
    std::vector<boid>       m_neighbors;
    p6::Color               m_color;

public:

    // Random position boid constructor
    boid(float speed, float deviateValue, float base, float height, float detectionRadius, unsigned int boidId, p6::Context& context): m_speedFactor(speed), m_deviateValue(deviateValue), m_baseWidth(base), m_height(height), m_xSpeed(p6::random::number(-1,1)), m_ySpeed(p6::random::number(-1, 1)), m_detectionRadius(detectionRadius), m_id(boidId){
        m_center    = glm::vec2(p6::random::number(-context.aspect_ratio() + m_secureArea, context.aspect_ratio() - m_secureArea), p6::random::number(-1 + m_secureArea, 1 - m_secureArea));
        setTriangleVertexes();
    }

    // Defined position boid constructor
    boid(glm::vec2 center, float speed, float deviateValue, float base, float height, float detectionRadius, unsigned int boidId, p6::Context& context): m_center(center), m_speedFactor(speed), m_deviateValue(deviateValue), m_baseWidth(base), m_height(height), m_xSpeed(p6::random::number(-1,1)), m_ySpeed(p6::random::number(-1, 1)), m_detectionRadius(detectionRadius), m_id(boidId){
        setTriangleVertexes();
    }

    void checkNeighbors(std::vector<boid> boids){
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

    void getGroupDirection(){
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

    void setTriangleVertexes(){
        m_leftPoint = {-m_height/2, m_baseWidth/2};
        m_rightPoint = {-m_height/2, -m_baseWidth/2};
        m_topPoint = {m_height/2, 0};
    }

    void updateParameters(float speedFactor, float deviateValue, float base, float height, float detectionRadius){
        m_speedFactor = speedFactor;
        m_deviateValue = deviateValue;
        m_baseWidth = base;
        m_height = height;
        setTriangleVertexes();
        if (detectionRadius >= m_height){
            m_detectionRadius = detectionRadius;
        } else {
            m_detectionRadius = m_height;
        }
    }

    void drawID(p6::Context& ctx){
        p6::BottomLeftCorner textPos = m_center + glm::vec2{m_height/2, m_height/2};
        std::u16string text = to_u16string(m_id);
        ctx.text_inflating = 0.008f;
        ctx.text_size = 0.015f;
        ctx.text(text, textPos);
    }

    void drawDetectionCircle(p6::Context& ctx){
        ctx.stroke_weight = 0.0027f;
        ctx.stroke = {0.1f, 0.1f, 0.1f, 0.6f};
        ctx.circle(
            m_center,
            m_detectionRadius
        );
    }

    void boidMovement(){
        m_xToTarget = m_targetXSpeed - m_xSpeed;
        m_yToTarget = m_targetYSpeed - m_ySpeed;
        m_xSpeed += (m_xToTarget / m_inertiaFactor) + p6::random::number(-m_deviateValue, m_deviateValue);
        m_ySpeed += (m_yToTarget / m_inertiaFactor) + p6::random::number(-m_deviateValue, m_deviateValue);
        m_center += glm::normalize(glm::vec2(m_xSpeed, m_ySpeed)) * m_speedFactor;
    }

    void draw(p6::Context& ctx, bool displayCircles, bool displayID){

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
};

void addBoid(std::vector<boid>& boids, glm::vec2 startPos, float speedFactor, float deviateValue, float baseWidth, float height, float detectionRadius, p6::Context& context){
    boid oneboid(startPos, speedFactor, deviateValue, baseWidth, height, detectionRadius, boids.size(), context);
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

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    std::vector<boid> boids;
    float                  globalSpeedFactor  = 0.006;
    float                  globalDeviateValue = 0.0;
    int                    globalDirectionChangeChance = 1;
    float                  globalBase   = 0.025;
    float                  globalHeight = 0.035;
    float                  globalDetectionRadius = 0.05;
    bool                   displayID = false;
    bool                   displayDetectionCircle = false;

    size_t boidNumbers = 50;

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};

    for (unsigned int i = 0; i < boidNumbers; i++){
    boid oneboid(globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius, i, ctx);
    boids.push_back(oneboid);
    }


    std::string text          = "Hello";

    // Declare your infinite update loop.
    ctx.update = [&]() {
        //ctx.use_stroke = false;
        //ctx.fill = {0.15f, 0.15f, 0.2f, 0.3f};
        //ctx.rectangle(p6::FullScreen{});
        ctx.background({0.15f, 0.15f, 0.2f, 0.3f});

        for(size_t i = 0; i < boids.size(); i++){
            boids[i].updateParameters(globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius);
            boids[i].checkNeighbors(boids);
            boids[i].draw(ctx, displayDetectionCircle, displayID);
        }


        // Show a simple window
        ImGui::Begin("Boids control panel");
        ImGui::Text("Click anywhere to add more boids");
        ImGui::SliderFloat("Speed factor", &globalSpeedFactor, 0.001f, 0.1f);
        ImGui::SliderFloat("Base width", &globalBase, 0.001f, 0.1f);
        ImGui::SliderFloat("Height", &globalHeight, 0.001f, 0.1f);
        ImGui::SliderFloat("Detection radius", &globalDetectionRadius, std::fmax(globalHeight, globalBase), 0.2f);
        ImGui::Checkbox("Display detection circle", &displayDetectionCircle);
        ImGui::Checkbox("Display ID", &displayID);
        ImGui::End();

        //ImGui::ShowDemoWindow();

        if (ctx.mouse_button_is_pressed(static_cast<p6::Button>(GLFW_MOUSE_BUTTON_LEFT)) && !ImGui::GetIO().WantCaptureMouse){
            addBoid(boids, ctx.mouse(), globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius, ctx);
        }

        displayBoidsNumber(boids, ctx);

    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}

// A faire
// Mettre un shift maximum pour que les boids ne tournent pas trop vite
// Faire en sort que les boids ne puissent pas empiéter sur les espaces les uns des autres
// S'inspirer des vidéos pour les comportements des boids
// Rassembler les constructeurs en un avec la position aléatoire par défaut
// Repousser plus ou moins fort selon la distance
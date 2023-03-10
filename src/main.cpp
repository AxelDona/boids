#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

class boid{

private :

    unsigned int            m_id;
    float                   m_speedFactor;
    float                   m_deviateValue;
    int                     m_directionChangeChance = 1;
    float                   m_baseWidth;
    float                   m_height;
    float                   m_secureArea = m_baseWidth * m_height * 2;
    glm::vec2               m_center     = {0, 0};
    glm::vec2               m_leftPoint = {-m_height/2, m_baseWidth/2};
    glm::vec2               m_rightPoint = {-m_height/2, -m_baseWidth/2};
    glm::vec2               m_topPoint = {m_height/2, 0};
    glm::vec2               m_direction{};
    float                   m_detectionRadius;
    std::vector<boid>       m_neighbors;
    p6::Color               m_color;

public:

    boid(float speed, float deviateValue, float base, float height, float detectionRadius, unsigned int boidId, p6::Context& context): m_speedFactor(speed), m_deviateValue(deviateValue), m_baseWidth(base), m_height(height), m_detectionRadius(detectionRadius), m_id(boidId){
        m_center    = glm::vec2(p6::random::number(-context.aspect_ratio() + m_secureArea, context.aspect_ratio() - m_secureArea), p6::random::number(-1 + m_secureArea, 1 - m_secureArea));
        m_direction = p6::random::direction();
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

    void draw(p6::Context& ctx){
        // Draw boid
        ctx.use_stroke = true;
        ctx.stroke_weight = 0.005f;
        ctx.stroke = m_color;
        ctx.triangle(
            m_leftPoint,
            m_rightPoint,
            m_topPoint,
            m_center,
            p6::Angle(m_direction)
            );
        // Move triangle
        m_center += m_direction * m_speedFactor;
        // Check if direction should be randomly changed (probability is 1/directionChangeChance)
        if (p6::random::integer(m_directionChangeChance) == 0){
            // Slightly modify direction
            m_direction = glm::normalize(m_direction += glm::vec2(p6::random::number(-m_deviateValue, m_deviateValue), p6::random::number(-m_deviateValue, m_deviateValue)));
        }
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

    void drawDetectionCircle(p6::Context& ctx){
        // Draw detection circle
        ctx.use_stroke = false;
        ctx.fill = {0.1f, 0.1f, 0.1f, 0.07f};
        ctx.circle(
            m_center,
            m_detectionRadius
        );
    }
};

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
    float                  globalSpeedFactor  = 0.002;
    float                  globalDeviateValue = 0.04;
    int                    globalDirectionChangeChance = 1;
    float                  globalBase   = 0.05;
    float                  globalHeight = 0.06;
    float                   globalDetectionRadius = 0.1;

    size_t voidNumbers = 100;


    // Set triangle positions
    std::vector<glm::vec2> baseCenter(100);

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};


    for (unsigned int i = 0; i < voidNumbers; i++){
        boid oneboid(globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius, i, ctx);
        boids.push_back(oneboid);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.use_stroke = false;
        ctx.fill = {0.15f, 0.15f, 0.2f, 0.15f};
        ctx.rectangle(p6::FullScreen{});

        for(size_t i = 0; i < boids.size(); i++){
            boids[i].checkNeighbors(boids);
            boids[i].draw(ctx);
        }
        for(size_t j = 0; j < boids.size(); j++){
            boids[j].checkNeighbors(boids);
            boids[j].drawDetectionCircle(ctx);
        }

    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}

// A faire
#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

class boid{

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

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    float                   speedFactor  = 0.002;
    float                   deviateValue = 0.04;
    int                     directionChangeChance = 1;

    // Set triangle positions
    std::vector<glm::vec2> baseCenter(100);
    float                  base   = 0.05;
    float                  height = 0.06;
    float                  secureArea = base * height * 2;
    glm::vec2              leftPoint(0, base/2);
    glm::vec2              rightPoint(-leftPoint);
    glm::vec2              topPoint(height, 0);
    for (unsigned int i = 0; i <= 100; i++) {
        baseCenter[i] = glm::vec2(p6::random::number(-ctx.aspect_ratio() + secureArea, ctx.aspect_ratio() - secureArea), p6::random::number(-1 + secureArea, 1 - secureArea));
    }

    // Set triangle centers
    std::vector<glm::vec2> centerValue(100);
    for (unsigned int i = 0; i <= 100; i++) {
        centerValue[i] = baseCenter[i];
    }

    // Set triangle directions
    std::vector<glm::vec2> directionValue(100);
    for (unsigned int i = 0; i <= 100; i++) {
        directionValue[i] = p6::random::direction();
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.use_stroke = false;
        ctx.fill = {0.15f, 0.15f, 0.2f, 0.15f};
        ctx.rectangle(p6::FullScreen{});

        ctx.use_stroke = true;
        ctx.stroke_weight = 0.005f;
        ctx.stroke = {1.0f, 0.15f, 0.3f};

        for (unsigned int i = 0; i <= 100; i++) {
            ctx.triangle(
                leftPoint,
                rightPoint,
                topPoint,
                centerValue[i],
                p6::Angle(directionValue[i])
            );
            // Move triangle
            centerValue[i] += directionValue[i] * speedFactor;
            // Check if direction should be randomly changed (probability is 1/directionChangeChance)
            if (p6::random::integer(directionChangeChance) == 0){
                // Slightly modify direction
                directionValue[i] = glm::normalize(directionValue[i] += glm::vec2(p6::random::number(-deviateValue, deviateValue), p6::random::number(-deviateValue, deviateValue)));
            }
            // Check if within window limits and teleport
            if (centerValue[i][0] < -ctx.aspect_ratio()){
                centerValue[i][0] = ctx.aspect_ratio();
            }
            if (centerValue[i][0] > ctx.aspect_ratio()){
                centerValue[i][0] = - ctx.aspect_ratio();
            }
            if (centerValue[i][1] > 1){
                centerValue[i][1] = - 1;
            }
            if (centerValue[i][1] < - 1){
                centerValue[i][1] = 1;
            }
        }
    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}
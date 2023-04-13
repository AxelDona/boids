#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "boids.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0) {
            return EXIT_FAILURE;
        }
        // The CI does not have a GPU, so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available) {
            return EXIT_SUCCESS;
        }
    }

    std::vector<boid> boids;
    float                  globalSpeedFactor                = 0.006;
    float                  globalBase                       = 0.025;
    float                  globalHeight                     = 0.035;
    float                  globalDetectionFactor            = 2.0f;
    float                  globalAvoidanceFactor            = 1.5f;
    float                  globalSeparationFactor           = 0.3;
    float                  globalAlignmentFactor            = 0.015;
    float                  globalCohesionFactor             = 0.002;
    bool                   isIdDisplayed                    = false;
    bool                   isDetectionDisplayed             = false;
    bool                   isAvoidanceRadiusDisplayed       = false;
    bool                   isDistanceToNeighborDisplayed    = false;
    float                  windowMargin                     = 0.4;

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};

    // Initialize a number of boids
    size_t boidNumbers = 50;

    for (unsigned int i = 0; i < boidNumbers; i++){
        boid singleBoid(globalSpeedFactor, globalBase, globalHeight, globalDetectionFactor, globalAvoidanceFactor, i, ctx, windowMargin);
        boids.push_back(singleBoid);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background({0.15f, 0.15f, 0.2f, 0.3f});

        for(size_t i = 0; i < boids.size(); i++){
            boids[i].updateBoidParameters(globalSpeedFactor, globalBase, globalHeight, globalSeparationFactor, globalAlignmentFactor, globalCohesionFactor, globalDetectionFactor, globalAvoidanceFactor);
            boids[i].checkNeighbors(boids);
            boids[i].draw(ctx, windowMargin, isDetectionDisplayed, isAvoidanceRadiusDisplayed, isIdDisplayed, isDistanceToNeighborDisplayed);
        }

        // Show a simple window
        ImGui::Begin("Boids control panel");
        ImGui::Text("Click anywhere to add more boids");
        ImGui::Separator();
        ImGui::Text("Boid shape");
        ImGui::SliderFloat("Speed", &globalSpeedFactor, 0.001f, 0.1f);
        ImGui::SliderFloat("Width", &globalBase, 0.001f, 0.1f);
        ImGui::SliderFloat("Height", &globalHeight, 0.001f, 0.1f);
        ImGui::Separator();
        ImGui::Text("Boid behaviour");
        ImGui::SliderFloat("Separation", &globalSeparationFactor, 0.0, 1.0);
        ImGui::SliderFloat("Alignment", &globalAlignmentFactor, 0.0, 0.1);
        ImGui::SliderFloat("Cohesion", &globalCohesionFactor, 0.0, 0.01);
        ImGui::Separator();
        ImGui::Text("Boid sight");
        ImGui::SliderFloat("Detection radius", &globalDetectionFactor, std::fmax(1.0f, globalAvoidanceFactor), 5.0f);
        ImGui::SliderFloat("Avoidance radius", &globalAvoidanceFactor, 1.0f, 5.0f);
        ImGui::Separator();
        ImGui::Text("Display options");
        ImGui::Checkbox("Display detection circle", &isDetectionDisplayed);
        ImGui::Checkbox("Display collision circle", &isAvoidanceRadiusDisplayed);
        ImGui::Checkbox("Display neighbor tension", &isDistanceToNeighborDisplayed);
        ImGui::Checkbox("Display ID", &isIdDisplayed);
        ImGui::End();

        if (ctx.mouse_button_is_pressed(static_cast<p6::Button>(GLFW_MOUSE_BUTTON_LEFT)) && !ImGui::GetIO().WantCaptureMouse){
            addBoid(boids, ctx.mouse(), globalSpeedFactor, globalBase, globalHeight, globalDetectionFactor, globalAvoidanceFactor);
        }

        displayBoidsNumber(boids, ctx);

    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}

// À faire
// Rassembler les constructeurs en un avec la position aléatoire par défaut
// Voir les sign detecton
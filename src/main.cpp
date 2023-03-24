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
    float                  globalSpeedFactor  = 0.006;
    float                  globalDeviateValue = 0.0;
    float                  globalBase   = 0.025;
    float                  globalHeight = 0.035;
    float                  globalDetectionRadius = 0.05;
    bool                   displayID = false;
    bool                   displayDetectionCircle = false;

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};

    // Initialize a number of boids
    size_t boidNumbers = 50;
    for (unsigned int i = 0; i < boidNumbers; i++){
        boid singleBoid(globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius, i, ctx);
        boids.push_back(singleBoid);
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background({0.15f, 0.15f, 0.2f, 0.3f});

        for(size_t i = 0; i < boids.size(); i++){
            boids[i].updateParameters(globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius);
            boids[i].checkNeighbors(boids);
            boids[i].draw(ctx, displayDetectionCircle, displayID);
        }

        // Show a simple window
        ImGui::Begin("Boids control panel");
        ImGui::Text("Click anywhere to add more boids");
        ImGui::Separator();
        ImGui::SliderFloat("Speed factor", &globalSpeedFactor, 0.001f, 0.1f);
        ImGui::SliderFloat("Base width", &globalBase, 0.001f, 0.1f);
        ImGui::SliderFloat("Height", &globalHeight, 0.001f, 0.1f);
        ImGui::SliderFloat("Detection radius", &globalDetectionRadius, std::fmax(globalHeight, globalBase), 0.2f);
        ImGui::Separator();
        ImGui::Checkbox("Display detection circle", &displayDetectionCircle);
        ImGui::Checkbox("Display ID", &displayID);
        ImGui::End();

        if (ctx.mouse_button_is_pressed(static_cast<p6::Button>(GLFW_MOUSE_BUTTON_LEFT)) && !ImGui::GetIO().WantCaptureMouse){
            addBoid(boids, ctx.mouse(), globalSpeedFactor, globalDeviateValue, globalBase, globalHeight, globalDetectionRadius);
        }

        displayBoidsNumber(boids, ctx);

    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}

// À faire
// Mettre un shift maximum pour que les boids ne tournent pas trop vite
// Faire en sort que les boids ne puissent pas empiéter sur les espaces les uns des autres
// S'inspirer des vidéos pour les comportements des boids
// Rassembler les constructeurs en un avec la position aléatoire par défaut
// Repousser plus ou moins fort selon la distance
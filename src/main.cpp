#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "boids.h"
#include <iostream>

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

    std::vector<Boid> boids;

    // Generate environment
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    World world(ctx);

    // Initialize a number of boids
    size_t boidNumbers = 50;

    // Get list of names from file
    std::vector<std::string> namesList = getNamesList();

    // Create as many boids as intended
    for (unsigned int i = 0; i < boidNumbers; i++){
        Boid singleBoid(world, i, namesList);
        boids.push_back(singleBoid);
    }

    // Start the update loop
    ctx.update = [&]() {
        ctx.background({0.15f, 0.15f, 0.2f, 0.3f});

        for(size_t i = 0; i < boids.size(); i++){
            boids[i].updateBoidParameters();
            boids[i].checkNeighbors(boids);
            boids[i].draw();
        }

        // Show a simple window
        world.displayControls();

        if (ctx.mouse_button_is_pressed(static_cast<p6::Button>(GLFW_MOUSE_BUTTON_LEFT)) && !ImGui::GetIO().WantCaptureMouse){
            addBoid(world, boids, ctx.mouse(), namesList);
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
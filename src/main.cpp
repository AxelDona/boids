#include <cstdlib>
#include "boids.h"
#include "p6/p6.h"
#include "skin.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char* argv[]) {
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

    // Generate environment and boids array
    auto              ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    std::vector<Boid> boids;
    std::vector<Skin> skins = initSkins();
    Scene             scene(ctx, boids, skins);

    // Get list of names from file
    // std::vector<std::string> namesList = getNamesList();

    // Create as many boids as intended
    /*for (unsigned int i = 0; i < boidNumbers; i++){
        Boid singleBoid(scene, i, namesList);
        boids.push_back(singleBoid);
    }*/

    // Start the update loop
    ctx.update = [&]() {
        // Display ImGui controls
        scene.displayControls();

        scene.draw();

        if (ctx.mouse_button_is_pressed(static_cast<p6::Button>(GLFW_MOUSE_BUTTON_LEFT)) && !ImGui::GetIO().WantCaptureMouse && scene.getBoidsNumber() <= 4000) {
            addBoid(scene, boids, ctx.mouse());
        }
    };

    ctx.maximize_window();
    ctx.start();
}

// À faire
// Rassembler les constructeurs en un avec la position aléatoire par défaut
// Voir les sign detection
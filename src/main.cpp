#include <cstdlib>
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

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

    // Set square values
    std::vector<glm::vec2> baseCenter(100);
    float                  base   = 0.1;
    float                  height = 0.2;
    glm::vec2              rightPoint(base / 2, 0.0);
    glm::vec2              leftPoint(-rightPoint);
    glm::vec2              topPoint(0, height);
    std::vector<p6::Angle> rotationValue(100);
    for (unsigned int i = 0; i <= 100; i++) {
        baseCenter[i] = glm::vec2(p6::random::number(-ctx.aspect_ratio() + base / 2, ctx.aspect_ratio() - base / 2), p6::random::number(-1, 1));
    }

    for (unsigned int i = 0; i <= 100; i++) {
        rotationValue[i] = p6::random::angle();
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (unsigned int i = 0; i <= 100; i++) {
            ctx.triangle(
                baseCenter[i] + leftPoint,
                baseCenter[i] + rightPoint,
                baseCenter[i] + topPoint
            );
        }
    };

    ctx.maximize_window();
    // Should be done last. It starts the infinite loop.
    ctx.start();
}
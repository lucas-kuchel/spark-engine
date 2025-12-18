#include <spark/spark.hpp>

struct FixedStep {
};

class Application {
public:
    Application() {
        dispatcher_.sink<FixedStep>().connect<Application::fixstep>();
    }

    ~Application() {
        dispatcher_.sink<FixedStep>().disconnect<Application::fixstep>();
    }

    void update(double deltaTime) {
        accum_ += deltaTime;

        while (accum_ >= fixedStep_) {
            dispatcher_.enqueue<FixedStep>();
            accum_ -= fixedStep_;
        }

        dispatcher_.update();
    }

    [[nodiscard]] bool running() {
        return true;
    }

    static void fixstep(const FixedStep& event) {
        spark::println("timestep");
    }

private:
    double accum_ = 0.0;
    double fixedStep_ = 1.0 / 1.0;

    spark::dispatcher dispatcher_;
    spark::registry registry_;
};

int main() {
    spark::program<Application> program;

    program.run();

    return 0;
}
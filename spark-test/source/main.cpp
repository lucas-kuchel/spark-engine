#include <spark/spark.hpp>

struct heal_event {
    float value;
};

struct damage_event {
    float value;
};

void heal_react(const heal_event& event) {
    spark::println("heal: {}", event.value);
}

void damage_react(const damage_event& event) {
    spark::println("damage: {}", event.value);
}

int main() {
    spark::dispatcher disp;

    disp.sink<heal_event>().connect<heal_react>();
    disp.sink<damage_event>().connect<damage_react>();

    disp.enqueue<heal_event>(0.3);
    disp.enqueue<damage_event>(0.9);
    disp.enqueue<heal_event>(0.7);

    disp.sink<damage_event>().disconnect<damage_react>();

    disp.update();
}

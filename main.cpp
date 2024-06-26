#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <chrono>


std::mutex mtx;

class Goldfish {
public:
    Goldfish(double v, double startX, double endX) : speed(v), x(startX), startX(startX), endX(endX), direction(1) {}

    void update(double timeStep) {
        x += speed * direction * timeStep;
        if (x >= endX || x <= startX) {
            direction *= -1; // Change direction when reaching the end
        }
    }

    void printPosition() const {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << "Goldfish Position: (" << x << ", 0)\n";
    }

private:
    double x;
    double speed;
    double startX, endX;
    int direction;
};

class Guppy {
public:
    Guppy(double v, double startY, double endY) : speed(v), y(startY), startY(startY), endY(endY), direction(1) {}

    void update(double timeStep) {
        y += speed * direction * timeStep;
        if (y >= endY || y <= startY) {
            direction *= -1; // Change direction when reaching the end
        }
    }

    void printPosition() const {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << "Guppy Position: (0, " << y << ")\n";
    }

private:
    double y;
    double speed;
    double startY, endY;
    int direction;
};

void goldfishThread(Goldfish& fish, double timeStep, double totalTime) {
    for (double time = 0; time <= totalTime; time += timeStep) {
        fish.update(timeStep);
        fish.printPosition();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeStep * 1000)));
    }
}

void guppyThread(Guppy& fish, double timeStep, double totalTime) {
    for (double time = 0; time <= totalTime; time += timeStep) {
        fish.update(timeStep);
        fish.printPosition();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeStep * 1000)));
    }
}

int main() {
    double speed = 1.0;
    double startX = 0.0;
    double endX = 10.0;
    double startY = 0.0;
    double endY = 10.0;
    double timeStep = 1.0;
    double totalTime = 20.0;

    Goldfish goldfish(speed, startX, endX);
    Guppy guppy(speed, startY, endY);

    std::thread goldfishT(goldfishThread, std::ref(goldfish), timeStep, totalTime);
    std::thread guppyT(guppyThread, std::ref(guppy), timeStep, totalTime);

    goldfishT.join();
    guppyT.join();

    return 0;
}

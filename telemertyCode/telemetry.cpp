//
// Simple example to demonstrate how to imitate a smart battery.
//
// Author: Julian Oes <julian@oes.ch>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <future>
#include <memory>
// For coloring output
#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace std::this_thread;
using namespace mavsdk;

void print_health(Telemetry::Health health)
{
    std::cout << "Got health: " << '\n';

    std::cout << "Gyro calibration:  " << (health.is_gyrometer_calibration_ok ? "ok" : "not ok")
              << '\n';
    std::cout << "Accel calibration: " << (health.is_accelerometer_calibration_ok ? "ok" : "not ok")
              << '\n';
    std::cout << "Mag calibration:   " << (health.is_magnetometer_calibration_ok ? "ok" : "not ok")
              << '\n';
    std::cout << "Level calibration: " << (health.is_level_calibration_ok ? "ok" : "not ok")
              << '\n';
    std::cout << "Local position:    " << (health.is_local_position_ok ? "ok" : "not ok") << '\n';
    std::cout << "Global position:   " << (health.is_global_position_ok ? "ok" : "not ok") << '\n';
    std::cout << "Home position:     " << (health.is_home_position_ok ? "ok" : "not ok") << '\n';
}

int main(int argc, char** argv)
{
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;

    if (argc == 2) {
        connection_url = argv[1];
        connection_result = mavsdk.add_any_connection(connection_url);
    } else {
        return 1;
    }

    if (connection_result != ConnectionResult::Success) {
        std::cout << "Connection failed: " << connection_result << std::endl;
        return 1;
    }

    std::promise<void> prom;
    std::future<void> fut = prom.get_future();
    std::cout << "Waiting to discover system..." << std::endl;
    mavsdk.subscribe_on_new_system([&mavsdk, &prom]() {
        const auto system = mavsdk.systems().at(0);

        if (system->is_connected()) {
            prom.set_value();
        }
    });

    if (fut.wait_for(std::chrono::seconds(2)) != std::future_status::ready) {
        std::cout << "No device found, exiting." << std::endl;
        return 1;
    }

    auto system = mavsdk.systems().at(0);
    // auto telemetry = Telemetry{system};
    auto telemetry = std::make_shared<Telemetry>(system);
    // Set position update rate to 1 Hz.
    telemetry->subscribe_health(std::bind(&print_health, std::placeholders::_1));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    while (true) {
        std::cout << "Waiting for system to be ready" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "System is ready" << std::endl;
        std::cout << "Position_latitude_deg: " << telemetry->position().latitude_deg << '\n';
        std::cout << "Position_longitude_deg: " << telemetry->position().longitude_deg << '\n';
        std::cout << "Position_absolute_altitude_m: " << telemetry->position().absolute_altitude_m<< '\n';
        std::cout << "Position_relative_altitude_m: " << telemetry->position().relative_altitude_m<< '\n';
    }

    // for (unsigned i = 0; i < 1; ++i) {
    //     std::cout << "Position_latitude_deg: " << telemetry->position().latitude_deg << '\n';
    //     std::cout << "Position_longitude_deg: " << telemetry->position().longitude_deg << '\n';
    //     std::cout << "Position_absolute_altitude_m: " << telemetry->position().absolute_altitude_m<< '\n';
    //     std::cout << "Position_relative_altitude_m: " << telemetry->position().relative_altitude_m<< '\n';
    //     }
    return 0;
}





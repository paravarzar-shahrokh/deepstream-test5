#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour
// #define DEFAULT_SERIAL_BAUDRATE (115200)
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using namespace mavsdk;
void wait_until_discover(Mavsdk& mavsdk)
{
    std::cout << "Waiting to discover system..." << std::endl;
    std::promise<void> discover_promise;
    auto discover_future = discover_promise.get_future();

    mavsdk.subscribe_on_new_system([&mavsdk, &discover_promise]() {
        const auto system = mavsdk.systems().at(0);

        if (system->is_connected()) {
            std::cout << "Discovered system" << std::endl;
            discover_promise.set_value();
        }
    });

    discover_future.wait();
}
void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

int main(int argc, char** argv)
{
    Mavsdk mavsdk;
    std::string connection_url;
    ConnectionResult connection_result;

    if (argc == 2) {
        connection_url = argv[1];
        // connection_result = mavsdk.add_any_connection(connection_url);
        connection_result = mavsdk.add_serial_connection(connection_url);
    } else {
        usage(argv[0]);
        return 1;
    }

    if (connection_result != ConnectionResult::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Connection failed: " << connection_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }

    // Wait for the system to connect via heartbeat
    // wait_until_discover(mavsdk);

    // System got discovered.
    auto system = mavsdk.systems().at(0);
    auto telemetry = Telemetry{system};


    while (!telemetry.health_all_ok()) {
        std::cout << "Waiting for system to be ready" << std::endl;
        sleep_for(seconds(1));
        std::cout << "System is ready" << std::endl;
    }
    

    // We want to listen to the altitude of the drone at 1 Hz.
    const Telemetry::Result set_rate_result = telemetry.set_rate_position(1.0);
    if (set_rate_result != Telemetry::Result::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Setting rate failed:" << set_rate_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;
    }
    
   
}  
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <libserial/SerialStream.h>

int main() {
    try {
        LibSerial::SerialStream serial_port;
        
        // Open and configure serial port
        serial_port.Open("/dev/ttyUSB0");
        serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_230400);
        serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        serial_port.SetParity(LibSerial::Parity::PARITY_NONE);
        serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
        
        char c;
        std::string line;
        int packet_count = 0;
        int total_packets = 0;
        auto start = std::chrono::steady_clock::now();
        auto program_start = start;
        
        std::cout << "Monitoring IMU data packets on /dev/ttyUSB0 at 115200 baud..." << std::endl;
        std::cout << "Expected JSON format: {\"accX\":...}" << std::endl;
        std::cout << "Press Ctrl+C to exit" << std::endl << std::endl;
        
        while (true) {
            while (serial_port.get(c)) {
                if (c == '\n' || c == '\r') {
                    if (!line.empty()) {
                        // Check if this looks like a valid JSON packet
                        if (line.front() == '{' && line.back() == '}') {
                            packet_count++;
                            total_packets++;
                            
                            // Optional: Print first few packets for verification
                            if (total_packets <= 3) {
                                std::cout << "Packet " << total_packets << ": " << line << std::endl;
                            }
                        }
                        
                        auto now = std::chrono::steady_clock::now();
                        std::chrono::duration<double> elapsed = now - start;
                        
                        if (elapsed.count() >= 1.0) {
                            double rate = packet_count / elapsed.count();
                            std::chrono::duration<double> total_elapsed = now - program_start;
                            double avg_rate = total_packets / total_elapsed.count();
                            
                            std::cout << "Current rate: " << std::fixed << std::setprecision(1) 
                                      << rate << " Hz | "
                                      << "Average rate: " << avg_rate << " Hz | "
                                      << "Total packets: " << total_packets << std::endl;
                            
                            packet_count = 0;
                            start = now;
                        }
                        line.clear();
                    }
                } else {
                    line += c;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
#include <iostream>
#include <string>
#include <map>
#include <memory>
using namespace std;

class SmartDevice {
protected:
    string deviceId;
    string name;
    bool isOn;
    float powerConsumption;
public:
    SmartDevice(string id, string n, float power)
        : deviceId(id), name(n), isOn(false), powerConsumption(power) {}

    virtual void performAction() = 0;
    void toggle() {
        isOn = !isOn;
        cout << name << " is now " << (isOn ? "ON" : "OFF") << endl;
    }

    virtual void displayStatus() const {
        cout << name << " (ID: " << deviceId << ") - "
             << (isOn ? "ON" : "OFF") << " | Power: " 
             << (isOn ? to_string(powerConsumption) : "0") << "W" << endl;
    }
    string getId() const { return deviceId; }
    bool getStatus() const { return isOn; }
    virtual ~SmartDevice() = default;
};

class SmartLight : public SmartDevice {
public:
    SmartLight(string id, string n, float power)
        : SmartDevice(id, n, power) {}

    void performAction() override {
        toggle(); 
    }

    void displayStatus() const override {
        SmartDevice::displayStatus();
        cout << "  (Smart Light)" << endl;
    }
};

class SmartThermostat : public SmartDevice {
    float temperature;
public:
    SmartThermostat(string id, string n, float power, float temp)
        : SmartDevice(id, n, power), temperature(temp) {}
    void performAction() override {
        toggle(); 
        if (isOn) {
            cout << "  Setting temperature to " << temperature << "°C" << endl;
        }
    }

    void displayStatus() const override {
        SmartDevice::displayStatus();
        if (isOn) {
            cout << "  Temperature: " << temperature << "°C" << endl;
        }
    }
};

class SmartHomeHub {
    map<string, shared_ptr<SmartDevice>> devices;
public:
    void addDevice(shared_ptr<SmartDevice> device) {
        devices[device->getId()] = device;
    }

    void toggleDevice(string deviceId) {
        if (devices.find(deviceId) != devices.end()) {
            devices[deviceId]->performAction();
        } else {
            cout << "Device with ID " << deviceId << " not found!" << endl;
        }
    }

    void displayAllDevices() const {
        cout << "\nDevice Status:\n";
        for (const auto& devicePair : devices) {
            devicePair.second->displayStatus();
        }
    }

    bool isValidDeviceId(string deviceId) const {
        return devices.find(deviceId) != devices.end();
    }
};

int main() {
    SmartHomeHub myHome;
    myHome.addDevice(make_shared<SmartLight>("light1", "Living Room Light", 50));
    myHome.addDevice(make_shared<SmartThermostat>("thermo1", "Living Room Thermostat", 200, 22.5));

    cout << "Welcome to the Smart Home System!\n";
    cout << "You can control your devices by entering commands.\n";
    
    string command;
    string deviceId;

    while (true) {
        cout << "\nEnter command (type 'exit' to quit): ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        if (command == "toggle") {
            cout << "Enter device ID to toggle: ";
            getline(cin, deviceId);

            if (myHome.isValidDeviceId(deviceId)) {
                myHome.toggleDevice(deviceId);
            } else {
                cout << "Invalid device ID!\n";
            }
        }
        else if (command == "status") {
            myHome.displayAllDevices();
        } else {
            cout << "Unknown command. Please use 'toggle', 'status', or 'exit'.\n";
        }
    }
    cout << "Exiting Smart Home System...\n";
    return 0;
}

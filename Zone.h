#ifndef ZONE_H
#define ZONE_H
#include "Vehicle.h"

#include <string>
#include <ctime>
#include <vector>
#include <cmath>
#include <windows.h>
#include "ParkingArea.h"

struct AreaNode {
    ParkingArea* area;
    AreaNode* next;
    AreaNode(std::string name) { area = new ParkingArea(name); next = nullptr; }
};

class Zone {
private:
    std::string zoneName;
    AreaNode* head;
    int posX, posY;

public:
    Zone(std::string name, int x, int y) : zoneName(name), head(nullptr), posX(x), posY(y) {}

    std::string getName() { return zoneName; }
    AreaNode* getAreaHead() { return head; }

    void addArea(std::string name) {
        AreaNode* newNode = new AreaNode(name);
        if (!head) head = newNode;
        else {
            AreaNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    void addSlotsToArea(std::string areaName, int count) {
        AreaNode* temp = head;
        while (temp) {
            if (temp->area->getAreaName() == areaName) {
                temp->area->addSlots(count);
                return;
            }
            temp = temp->next;
        }
    }

    std::string findVehicleLocation(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) {
                    return "Building: " + zoneName + ", " + tempArea->area->getAreaName() + 
                           ", Slot: S-" + std::to_string(tempSlot->slot->getSlotId());
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return "";
    }

    bool parkInSpecificArea(Vehicle* v, std::string floorName) {
        AreaNode* temp = head;
        while (temp) {
            if (temp->area->getAreaName() == floorName) {
                ParkingSlot* spot = temp->area->findEmptySlot();
                if (spot) { spot->occupy(v); return true; }
                else {
                    MessageBoxA(NULL, (floorName + " is full!").c_str(), "Floor Full", MB_OK | MB_ICONWARNING);
                    return false;
                }
            }
            temp = temp->next;
        }
        return false;
    }

    // UPDATED: Now returns the vehicle type so GUI can calculate price
    std::string releaseAndGetType(std::string plate) {
        AreaNode* tempArea = head;
        while (tempArea) {
            SlotNode* tempSlot = tempArea->area->getHead();
            while (tempSlot) {
                if (tempSlot->slot->getStatus() == 1 && tempSlot->slot->getPlate() == plate) {
                    std::string type = tempSlot->slot->getVehicleType();
                    tempSlot->slot->release();
                    return type; // Return "Car" or "Bike"
                }
                tempSlot = tempSlot->next;
            }
            tempArea = tempArea->next;
        }
        return ""; // Not found
    }
};

class GlobalManager {
private:
    std::vector<Zone*> buildings;
public:
    void addBuilding(Zone* z) { buildings.push_back(z); }
    std::vector<Zone*>& getBuildings() { return buildings; }
    std::string getGlobalVehicleLocation(std::string plate) {
        for (Zone* b : buildings) {
            std::string loc = b->findVehicleLocation(plate);
            if (!loc.empty()) return loc;
        }
        return "";
    }
    void searchGlobal(std::string plate) {
        std::string loc = getGlobalVehicleLocation(plate);
        if (!loc.empty()) {
            std::string msg = "Vehicle Found!\nPlate: " + plate + "\n" + loc;
            MessageBoxA(NULL, msg.c_str(), "Search", MB_OK | MB_ICONINFORMATION);
        } else {
            MessageBoxA(NULL, "Vehicle not found in the Gulberg network.", "Search Result", MB_OK | MB_ICONERROR);
        }
    }
};

#endif
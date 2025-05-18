#ifndef HEADER_H
#define HEADER_H

#include <graphics.h>
#include <iostream>
#include <cstring>
#include <climits>

using namespace std;

// Structure for Medical Item
struct MedicalItem {
    char name[50];
    char category[30];
    int quantity;
    int expiryDate;
    MedicalItem *next;
};

// Structure for Supply Request
struct SupplyRequest {
    int requestID;
    char itemName[50];
    int urgencyLevel;
    int quantity;
};

// Node for Supply Request Linked List
struct SupplyRequestNode {
    SupplyRequest data;
    SupplyRequestNode *next;
};

// Manager Class
struct MedicalSuppliesManager {
    SupplyRequestNode *requestHead;
    MedicalSuppliesManager() : requestHead(nullptr) {}

    void addMedicalItem(MedicalItem **head, const char *name, const char *category, int quantity, int expiryDate);
    void removeMedicalItem(MedicalItem **head, const char *name);
    void displayInventory(MedicalItem *head);
    MedicalItem *searchMedicalItem(MedicalItem *head, const char *name);

    void addSupplyRequest(int requestID, const char *itemName, int urgencyLevel, int quantity);
    void processSupplyRequests(MedicalItem **inventoryHead);

    void alertForExpiry(MedicalItem *head, int currentDate);
    void monitorExpiryDates(MedicalItem *head, int currentDate);

    void drawMenu();
    void displayInventoryGraphically(MedicalItem *head);
};

#endif

#include "m.h"
#include <graphics.h>
#include <cstring>
#include <cstdlib>
#include <climits>

// Display message
void displayMessage(const char *message, int x, int y) {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(x, y, (char *)message);
}

void MedicalSuppliesManager::addMedicalItem(MedicalItem **head, const char *name, const char *category, int quantity, int expiryDate) {
    MedicalItem *newItem = new MedicalItem;
    strcpy(newItem->name, name);
    strcpy(newItem->category, category);
    newItem->quantity = quantity;
    newItem->expiryDate = expiryDate;
    newItem->next = nullptr;

    if (*head == nullptr || (*head)->expiryDate > expiryDate) {
        newItem->next = *head;
        *head = newItem;
    } else {
        MedicalItem *current = *head;
        while (current->next != nullptr && current->next->expiryDate <= expiryDate) {
            current = current->next;
        }
        newItem->next = current->next;
        current->next = newItem;
    }

    char msg[200];
    sprintf(msg, "Medical item added: %s (%s) Quantity: %d Expiry Date: %d", name, category, quantity, expiryDate);
    displayMessage(msg, 50, 50);
}

void MedicalSuppliesManager::removeMedicalItem(MedicalItem **head, const char *name) {
    if (*head == nullptr) {
        displayMessage("Inventory is empty. Cannot remove item.", 50, 50);
        return;
    }

    MedicalItem *current = *head;
    MedicalItem *previous = nullptr;

    while (current != nullptr && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        char msg[200];
        sprintf(msg, "Medical item not found: %s.", name);
        displayMessage(msg, 50, 50);
    } else {
        if (previous == nullptr) {
            *head = current->next;
        } else {
            previous->next = current->next;
        }
        char msg[200];
        sprintf(msg, "Medical item removed: %s.", current->name);
        displayMessage(msg, 50, 50);
        delete current;
    }
}

void MedicalSuppliesManager::displayInventory(MedicalItem *head) {
    if (head == nullptr) {
        displayMessage("Inventory is empty.", 50, 50);
        return;
    }

    int y = 100;
    displayMessage("--- Medical Inventory ---", 50, y);
    y += 20;
    displayMessage("Name    Category    Quantity    Expiry Date", 50, y);
    y += 20;

    MedicalItem *current = head;
    while (current != nullptr) {
        char itemDetails[200];
        sprintf(itemDetails, "%s    %s    %d    %d", current->name, current->category, current->quantity, current->expiryDate);
        displayMessage(itemDetails, 50, y);
        y += 20;
        current = current->next;
    }
}

MedicalItem *MedicalSuppliesManager::searchMedicalItem(MedicalItem *head, const char *name) {
    MedicalItem *current = head;
    while (current != nullptr) {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return nullptr;
}

void MedicalSuppliesManager::addSupplyRequest(int requestID, const char *itemName, int urgencyLevel, int quantity) {
    SupplyRequestNode *newNode = new SupplyRequestNode;
    newNode->data.requestID = requestID;
    strcpy(newNode->data.itemName, itemName);
    newNode->data.urgencyLevel = urgencyLevel;
    newNode->data.quantity = quantity;
    newNode->next = nullptr;

    if (requestHead == nullptr || requestHead->data.urgencyLevel > urgencyLevel) {
        newNode->next = requestHead;
        requestHead = newNode;
    } else {
        SupplyRequestNode *current = requestHead;
        while (current->next != nullptr && current->next->data.urgencyLevel <= urgencyLevel) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    char msg[200];
    sprintf(msg, "Supply request added: ID %d for item %s Urgency: %d Quantity: %d", requestID, itemName, urgencyLevel, quantity);
    displayMessage(msg, 50, 50);
}

void MedicalSuppliesManager::processSupplyRequests(MedicalItem **inventoryHead) {
    if (requestHead == nullptr) {
        displayMessage("No supply requests to process.", 50, 50);
        return;
    }

    SupplyRequestNode *currentRequestNode = requestHead;
    SupplyRequest currentRequest = currentRequestNode->data;
    requestHead = requestHead->next;

    MedicalItem *item = searchMedicalItem(*inventoryHead, currentRequest.itemName);
    if (item != nullptr) {
        if (item->quantity >= currentRequest.quantity) {
            item->quantity -= currentRequest.quantity;
            if (item->quantity == 0)
                removeMedicalItem(inventoryHead, currentRequest.itemName);

            char msg[200];
            sprintf(msg, "Request ID %d processed.", currentRequest.requestID);
            displayMessage(msg, 50, 50);
        } else {
            char msg[200];
            sprintf(msg, "Request ID %d partially processed. Only %d available.", currentRequest.requestID, item->quantity);
            displayMessage(msg, 50, 50);
            removeMedicalItem(inventoryHead, currentRequest.itemName);
        }
    } else {
        char msg[200];
        sprintf(msg, "Request ID %d: Item %s not found in inventory.", currentRequest.requestID, currentRequest.itemName);
        displayMessage(msg, 50, 50);
    }

    delete currentRequestNode;
}

void MedicalSuppliesManager::alertForExpiry(MedicalItem *head, int currentDate) {
    if (head == nullptr) {
        displayMessage("No items in inventory to check for expiry.", 50, 50);
        return;
    }

    bool expiredItemsFound = false;
    MedicalItem *current = head;
    while (current != nullptr) {
        if (current->expiryDate <= currentDate) {
            char msg[200];
            sprintf(msg, "ALERT: Item %s has expired. Expiry Date: %d", current->name, current->expiryDate);
            displayMessage(msg, 50, 50);
            expiredItemsFound = true;
        }
        current = current->next;
    }

    if (!expiredItemsFound)
        displayMessage("No expired items found in inventory.", 50, 50);
}

void MedicalSuppliesManager::monitorExpiryDates(MedicalItem *head, int currentDate) {
    if (head == nullptr) {
        displayMessage("No items in inventory to monitor for expiry.", 50, 50);
        return;
    }

    bool expiringSoonFound = false;
    MedicalItem *current = head;
    while (current != nullptr) {
        int daysToExpiry = current->expiryDate - currentDate;
        if (daysToExpiry > 0 && daysToExpiry <= 30) {
            char msg[200];
            sprintf(msg, "Item %s is expiring soon. Expiry Date: %d", current->name, current->expiryDate);
            displayMessage(msg, 50, 50);
            expiringSoonFound = true;
        }
        current = current->next;
    }

    if (!expiringSoonFound)
        displayMessage("No items expiring soon.", 50, 50);
}

void MedicalSuppliesManager::drawMenu() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char *)"");

    setbkcolor(WHITE);
    cleardevice();
    setcolor(BLUE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(200, 50, (char *)"Medical Supplies Manager Menu");

    int yStart = 100;
    int optionHeight = 50;
    char options[][50] = {
        "1. Add Medical Item",
        "2. Remove Medical Item",
        "3. Display Inventory",
        "4. Process Supply Requests",
        "5. Monitor Expiry Dates",
        "6. Exit"
    };

    for (int i = 0; i < 6; i++) {
        rectangle(150, yStart, 450, yStart + optionHeight);
        outtextxy(160, yStart + 15, options[i]);
        yStart += optionHeight + 10;
    }

    outtextxy(150, yStart + 20, (char *)"Press any key to exit...");
    getch();
}

void MedicalSuppliesManager::displayInventoryGraphically(MedicalItem *head) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    setbkcolor(WHITE);
    cleardevice();
    setcolor(GREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

    outtextxy(200, 20, (char*)"Graphical Inventory Display");

    if (head == nullptr) {
        setcolor(RED);
        outtextxy(100, 100, (char*)"Inventory is empty.");
        getch();
        closegraph();
        return;
    }

    int xStart = 100, yBase = 400, barWidth = 50, spacing = 20;
    MedicalItem *current = head;
    while (current != nullptr) {
        int barHeight = current->quantity * 10;
        setfillstyle(SOLID_FILL, BLUE);
        bar(xStart, yBase - barHeight, xStart + barWidth, yBase);

        setcolor(BLACK);
        outtextxy(xStart, yBase + 10, current->name);

        xStart += barWidth + spacing;
        current = current->next;
    }

    outtextxy(200, yBase + 50, (char*)"Press any key to exit...");
    getch();
    closegraph();
}

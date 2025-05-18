#include <graphics.h>
#include <iostream>
#include <cstring>
#include "m.h"

using namespace std;

int SCREEN_WIDTH = 1500;
int SCREEN_HEIGHT = 1000;

// Function to draw text
void drawText(int x, int y, const char *text, int color) {
    setcolor(color);
    outtextxy(x, y, const_cast<char *>(text));
}

// Function to create a button
void createButton(int x1, int y1, int x2, int y2, const char *label, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x1, y1, x2, y2);
    setbkcolor(color);
    drawText((x1 + x2) / 2 - (strlen(label) * 5), (y1 + y2) / 2 - 5, label, WHITE);
}

// Function to check button click
bool isButtonClicked(int mouseX, int mouseY, int x1, int y1, int x2, int y2) {
    return (mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2);
}

// Function to get text input from user
void getInput(int x, int y, char *buffer, int maxLength) {
    char key;
    int i = 0;
    while (true) {
        key = getch();
        if (key == 13) { // Enter
            buffer[i] = '\0';
            break;
        } else if (key == 8 && i > 0) { // Backspace
            i--;
            buffer[i] = '\0';
            setfillstyle(SOLID_FILL, BLACK);
            bar(x, y, x + maxLength * 10, y + 20);
            drawText(x, y, buffer, WHITE);
        } else if (key >= 32 && key <= 126 && i < maxLength - 1) {
            buffer[i++] = key;
            buffer[i] = '\0';
            drawText(x, y, buffer, WHITE);
        }
    }
}

// Function to display inventory visually
void displayInventory(MedicalItem *head, int x, int &y) {
    MedicalItem *current = head;
    while (current != nullptr) {
        char details[200];
        sprintf(details, "Name: %s, Category: %s, Quantity: %d, Expiry: %d",
                current->name, current->category, current->quantity, current->expiryDate);
        drawText(x, y, details, WHITE);
        y += 20;
        current = current->next;
    }
    if (head == nullptr) {
        drawText(x, y, "Inventory is empty.", RED);
    }
}

// ---------- MAIN FUNCTION -------------
int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    MedicalItem *inventory = nullptr;
    MedicalSuppliesManager manager;
    int choice = 0;

    while (choice != 8) {
        setfillstyle(SOLID_FILL, BLACK);
        bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        drawText(50, 20, "--- Medical Supplies Management System ---", WHITE);

        // Draw buttons
        createButton(50, 80, 200, 120, "1. Add Item", BLUE);
        createButton(50, 140, 200, 180, "2. Remove Item", BLUE);
        createButton(50, 200, 200, 240, "3. Display Inventory", BLUE);
        createButton(50, 260, 200, 300, "4. Search Item", BLUE);
        createButton(50, 320, 200, 360, "5. Add Request", BLUE);
        createButton(50, 380, 200, 420, "6. Process Requests", BLUE);
        createButton(50, 440, 200, 480, "7. Check Expiry", BLUE);
        createButton(50, 500, 200, 540, "8. Exit", RED);

        int mouseX, mouseY;
        while (!ismouseclick(WM_LBUTTONDOWN)) {}
        getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);

        if (isButtonClicked(mouseX, mouseY, 50, 80, 200, 120)) choice = 1;
        else if (isButtonClicked(mouseX, mouseY, 50, 140, 200, 180)) choice = 2;
        else if (isButtonClicked(mouseX, mouseY, 50, 200, 200, 240)) choice = 3;
        else if (isButtonClicked(mouseX, mouseY, 50, 260, 200, 300)) choice = 4;
        else if (isButtonClicked(mouseX, mouseY, 50, 320, 200, 360)) choice = 5;
        else if (isButtonClicked(mouseX, mouseY, 50, 380, 200, 420)) choice = 6;
        else if (isButtonClicked(mouseX, mouseY, 50, 440, 200, 480)) choice = 7;
        else if (isButtonClicked(mouseX, mouseY, 50, 500, 200, 540)) choice = 8;

        char inputBuffer[50];
        switch (choice) {
            case 1: {
                drawText(250, 100, "Enter name: ", WHITE);
                getInput(400, 100, inputBuffer, 50);
                char name[50], category[30];
                strcpy(name, inputBuffer);

                drawText(250, 140, "Enter category: ", WHITE);
                getInput(400, 140, inputBuffer, 30);
                strcpy(category, inputBuffer);

                drawText(250, 180, "Enter quantity: ", WHITE);
                getInput(400, 180, inputBuffer, 10);
                int quantity = atoi(inputBuffer);

                drawText(250, 220, "Enter expiry date (YYYYMMDD): ", WHITE);
                getInput(400, 220, inputBuffer, 10);
                int expiryDate = atoi(inputBuffer);

                manager.addMedicalItem(&inventory, name, category, quantity, expiryDate);
                drawText(250, 260, "Item added successfully!", GREEN);
                delay(2000);
                break;
            }
            case 2: {
                drawText(250, 100, "Enter name of the item to remove: ", WHITE);
                getInput(400, 100, inputBuffer, 50);
                manager.removeMedicalItem(&inventory, inputBuffer);
                drawText(250, 140, "Item removed successfully (if found).", GREEN);
                delay(2000);
                break;
            }
            case 3: {
                cleardevice();
                drawText(50, 50, "Inventory:", WHITE);
                int yOffset = 100;
                displayInventory(inventory, 50, yOffset);
                getch();
                break;
            }
            case 4: {
                drawText(250, 100, "Enter name of the item to search: ", WHITE);
                getInput(400, 100, inputBuffer, 50);
                MedicalItem *result = manager.searchMedicalItem(inventory, inputBuffer);
                if (result) {
                    char details[200];
                    sprintf(details, "Name: %s, Category: %s, Quantity: %d, Expiry: %d",
                            result->name, result->category, result->quantity, result->expiryDate);
                    drawText(250, 140, details, GREEN);
                } else {
                    drawText(250, 140, "Item not found.", RED);
                }
                delay(2000);
                break;
            }
            case 5: {
                drawText(250, 100, "Enter request ID: ", WHITE);
                getInput(400, 100, inputBuffer, 10);
                int requestID = atoi(inputBuffer);

                drawText(250, 140, "Enter item name: ", WHITE);
                getInput(400, 140, inputBuffer, 50);
                char itemName[50];
                strcpy(itemName, inputBuffer);

                drawText(250, 180, "Enter urgency level (1=High, 2=Med, 3=Low): ", WHITE);
                getInput(400, 180, inputBuffer, 2);
                int urgencyLevel = atoi(inputBuffer);

                drawText(250, 220, "Enter quantity: ", WHITE);
                getInput(400, 220, inputBuffer, 10);
                int quantityReq = atoi(inputBuffer);

                manager.addSupplyRequest(requestID, itemName, urgencyLevel, quantityReq);
                drawText(250, 260, "Supply request added successfully!", GREEN);
                delay(2000);
                break;
            }
            case 6: {
                cleardevice();
                drawText(50, 50, "Processing Supply Requests...", WHITE);
                manager.processSupplyRequests(&inventory);
                drawText(50, 100, "Requests processed.", GREEN);
                delay(2000);
                break;
            }
            case 7: {
                drawText(250, 100, "Enter current date (YYYYMMDD): ", WHITE);
                getInput(400, 100, inputBuffer, 10);
                int currentDate = atoi(inputBuffer);

                cleardevice();
                drawText(50, 50, "Checking expiry status...", WHITE);
                manager.alertForExpiry(inventory, currentDate);
                manager.monitorExpiryDates(inventory, currentDate);
                drawText(50, 100, "Check complete.", GREEN);
                delay(2000);
                break;
            }
            case 8: {
                drawText(250, 560, "Exiting program...", RED);
                delay(1000);
                break;
            }
            default:
                drawText(250, 560, "Invalid choice.", RED);
                delay(1000);
        }
    }

    closegraph();
    return 0;
}

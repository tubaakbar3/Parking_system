#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include <windows.h>
#include <string>
#include "Zone.h"

#define ID_BTN_PARK 201
#define ID_BTN_RELEASE 202
#define ID_BTN_SEARCH 207
#define ID_INPUT_PLATE 203
#define ID_COMBO_TYPE 204
#define ID_COMBO_BUILDING 205
#define ID_COMBO_FLOOR 206

class ParkingGUI {
public:
    static Zone* currentZone;
    static GlobalManager* globalNet;
    static HWND hPlateInput, hTypeCombo, hBuildingCombo, hFloorCombo;

    static void UpdateFloorCombo() {
        if (!hFloorCombo || !currentZone) return;
        SendMessage(hFloorCombo, CB_RESETCONTENT, 0, 0);
        AreaNode* temp = currentZone->getAreaHead();
        while (temp) {
            SendMessageA(hFloorCombo, CB_ADDSTRING, 0, (LPARAM)temp->area->getAreaName().c_str());
            temp = temp->next;
        }
        SendMessage(hFloorCombo, CB_SETCURSEL, 0, 0);
    }

    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg) {
        case WM_CREATE: {
            CreateWindowW(L"Static", L"BUILDING:", WS_VISIBLE | WS_CHILD, 150, 620, 80, 20, hwnd, NULL, NULL, NULL);
            hBuildingCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 230, 618, 140, 200, hwnd, (HMENU)ID_COMBO_BUILDING, NULL, NULL);

            CreateWindowW(L"Static", L"FLOOR:", WS_VISIBLE | WS_CHILD, 390, 620, 60, 20, hwnd, NULL, NULL, NULL);
            hFloorCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 450, 618, 120, 200, hwnd, (HMENU)ID_COMBO_FLOOR, NULL, NULL);

            CreateWindowW(L"Static", L"PLATE:", WS_VISIBLE | WS_CHILD, 150, 660, 60, 20, hwnd, NULL, NULL, NULL);
            hPlateInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 210, 658, 120, 25, hwnd, (HMENU)ID_INPUT_PLATE, NULL, NULL);

            hTypeCombo = CreateWindowW(L"Combobox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 340, 658, 80, 200, hwnd, (HMENU)ID_COMBO_TYPE, NULL, NULL);
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Car");
            SendMessageW(hTypeCombo, CB_ADDSTRING, 0, (LPARAM)L"Bike");
            SendMessageW(hTypeCombo, CB_SETCURSEL, 0, 0);

            CreateWindowW(L"Button", L"PARK", WS_VISIBLE | WS_CHILD, 430, 655, 90, 32, hwnd, (HMENU)ID_BTN_PARK, NULL, NULL);
            CreateWindowW(L"Button", L"RELEASE", WS_VISIBLE | WS_CHILD, 530, 655, 90, 32, hwnd, (HMENU)ID_BTN_RELEASE, NULL, NULL);
            CreateWindowW(L"Button", L"SEARCH", WS_VISIBLE | WS_CHILD, 630, 655, 90, 32, hwnd, (HMENU)ID_BTN_SEARCH, NULL, NULL);

            for (Zone* z : globalNet->getBuildings()) {
                SendMessageA(hBuildingCombo, CB_ADDSTRING, 0, (LPARAM)z->getName().c_str());
            }
            SendMessageW(hBuildingCombo, CB_SETCURSEL, 0, 0);
            UpdateFloorCombo();
        } break;

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORBTN: {
            HDC hdcControl = (HDC)wp;
            SetTextColor(hdcControl, RGB(255, 255, 255));
            SetBkColor(hdcControl, RGB(0, 0, 0));
            return (LRESULT)GetStockObject(BLACK_BRUSH);
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect; GetClientRect(hwnd, &rect);
            int winWidth = rect.right;

            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        
            HFONT hTitleFont = CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI");
            HFONT hSubFont = CreateFontW(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI");
            HFONT hSlotFont = CreateFontW(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI");
            HFONT hEmojiFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI Emoji");

            SelectObject(hdc, hTitleFont);
        
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(0, 0, 0));         // FORCE background color to BLACK
            SetBkMode(hdc, OPAQUE);                // Use the color we just set

            RECT rTitle = { 0, 5, winWidth, 45 };
            DrawTextW(hdc, L"CanttParking Management System", -1, &rTitle, DT_CENTER | DT_SINGLELINE);

            SelectObject(hdc, hSubFont);
            SetTextColor(hdc, RGB(200, 200, 200));
            std::string bName = "Building: " + currentZone->getName();
            std::wstring wbName(bName.begin(), bName.end());
            RECT rSub = { 0, 40, winWidth, 65 };
            DrawTextW(hdc, wbName.c_str(), -1, &rSub, DT_CENTER | DT_SINGLELINE);

            AreaNode* areaNode = currentZone->getAreaHead();
            int areaY = 75; 

            while(areaNode) {
                SelectObject(hdc, hSlotFont);
                SetTextColor(hdc, RGB(255, 255, 255));
                std::string fName = areaNode->area->getAreaName();
                std::wstring wfName(fName.begin(), fName.end());
                RECT rFloor = { 0, areaY, winWidth, areaY + 15 };
                DrawTextW(hdc, wfName.c_str(), -1, &rFloor, DT_CENTER | DT_SINGLELINE);

                SlotNode* slotNode = areaNode->area->getHead();
                int col = 0;
                int startX = (winWidth - (8 * 105)) / 2;

                while(slotNode) {
                    int curX = startX + (col * 105);
                    int curY = areaY + 18;
                    RECT rSlot = { curX, curY, curX + 95, curY + 60 };
                    
                    bool occupied = (slotNode->slot->getStatus() == 1);

                    // FIXED: Using a unique name 'hSlotBrush' to avoid redeclaration errors
                    HBRUSH hSlotBrush = occupied ? CreateSolidBrush(RGB(100, 100, 100)) : CreateSolidBrush(RGB(50, 50, 50));
                    SelectObject(hdc, hTitleFont);
                    SetTextColor(hdc, RGB(255, 255, 255)); // White text
                    SetBkMode(hdc, TRANSPARENT);           // Transparent background
                    RECT rTitle = { 0, 5, winWidth, 40 };
                DrawTextW(hdc, L"Cantt Parking Management System", -1, &rTitle, DT_CENTER | DT_SINGLELINE);
                    
                    FillRect(hdc, &rSlot, hSlotBrush);
                    FrameRect(hdc, &rSlot, (HBRUSH)GetStockObject(WHITE_BRUSH));
                    DeleteObject(hSlotBrush); // Cleanup memory

                    SetBkMode(hdc, TRANSPARENT);

                    if(!occupied) {
                        SetTextColor(hdc, RGB(0, 200, 255)); 
                        SelectObject(hdc, hSlotFont);
                        TextOutW(hdc, curX + 30, curY + 22, L"FREE", 4);
                    } else {
                        SelectObject(hdc, hEmojiFont);
                        std::string type = slotNode->slot->getVehicleType();
                        if(type == "Bike") TextOutW(hdc, curX + 32, curY + 4, L"\U0001F3CD", 2); 
                        else TextOutW(hdc, curX + 32, curY + 4, L"\U0001F697", 2); 

                        SelectObject(hdc, hSlotFont);
                        SetTextColor(hdc, RGB(255, 100, 100)); 
                        std::string p = slotNode->slot->getPlate();
                        std::wstring wp(p.begin(), p.end());
                        TextOutW(hdc, curX + 10, curY + 38, wp.c_str(), wp.length());
                    }
                    slotNode = slotNode->next; 
                    col++;
                }
                areaY += 88; 
                areaNode = areaNode->next;
            }
            DeleteObject(hTitleFont); DeleteObject(hSubFont); 
            DeleteObject(hSlotFont); DeleteObject(hEmojiFont);
            EndPaint(hwnd, &ps);
        } break;

        case WM_COMMAND: {
            if (LOWORD(wp) == ID_COMBO_BUILDING && HIWORD(wp) == CBN_SELCHANGE) {
                int index = SendMessage(hBuildingCombo, CB_GETCURSEL, 0, 0);
                currentZone = globalNet->getBuildings()[index];
                UpdateFloorCombo();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            if (LOWORD(wp) == ID_BTN_PARK) {
                char p[20], t[20], f[50];
                GetWindowTextA(hPlateInput, p, 20);
                SendMessageA(hTypeCombo, CB_GETLBTEXT, SendMessage(hTypeCombo, CB_GETCURSEL, 0, 0), (LPARAM)t);
                SendMessageA(hFloorCombo, CB_GETLBTEXT, SendMessage(hFloorCombo, CB_GETCURSEL, 0, 0), (LPARAM)f);
                if (strlen(p) == 0) return 0;
                if (currentZone->parkInSpecificArea(new Vehicle(p, t), f)) {
                    SetWindowTextA(hPlateInput, ""); InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            if (LOWORD(wp) == ID_BTN_RELEASE) {
                char p[20]; GetWindowTextA(hPlateInput, p, 20);
                if (strlen(p) == 0) return 0;
                
                std::string type = currentZone->releaseAndGetType(p);
                if (!type.empty()) {
                    int price = (type == "Bike") ? 50 : 100;
                    std::string msg = "PARKING RECEIPT\n------------------\nPlate: " + std::string(p) + 
                                     "\nVehicle: " + type + "\nAmount: Rs. " + std::to_string(price) + 
                                     "\n\nThank you for using our service!";
                    MessageBoxA(hwnd, msg.c_str(), "Receipt Issued", MB_OK | MB_ICONINFORMATION);
                    
                    SetWindowTextA(hPlateInput, ""); 
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxA(hwnd, "Vehicle not found in this building!", "Error", MB_OK | MB_ICONERROR);
                }
            }
            if (LOWORD(wp) == ID_BTN_SEARCH) {
                char p[20]; GetWindowTextA(hPlateInput, p, 20);
                if (strlen(p) > 0) globalNet->searchGlobal(p);
            }
        } break;
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
        }
        return 0;
    }

    void run(GlobalManager& gm) {
        globalNet = &gm;
        currentZone = globalNet->getBuildings()[0];
        WNDCLASSW wc = {0};
        wc.lpszClassName = L"CanttCompactGUI";
        wc.lpfnWndProc = WindowProcedure;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        RegisterClassW(&wc);
        CreateWindowW(L"CanttCompactGUI", L"Cantt Parking Management System", 
                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, 1100, 740, NULL, NULL, wc.hInstance, NULL);
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    }
};

Zone* ParkingGUI::currentZone = nullptr;
GlobalManager* ParkingGUI::globalNet = nullptr;
HWND ParkingGUI::hPlateInput = nullptr;
HWND ParkingGUI::hTypeCombo = nullptr;
HWND ParkingGUI::hBuildingCombo = nullptr;
HWND ParkingGUI::hFloorCombo = nullptr;

#endif
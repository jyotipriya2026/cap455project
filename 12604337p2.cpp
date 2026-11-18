#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <limits>

using namespace std;

const char* DATA_FILE = "grievances.dat";

// Helper function to safely copy strings into fixed-size char arrays
void safeCopy(char* dest, const char* src, size_t maxLen) {
    strncpy(dest, src, maxLen - 1);
    dest[maxLen - 1] = '\0';
}

class Grievance {
private:
    int grievanceId;
    char citizenName[40];
    char department[30];
    char category[25];
    char description[120];
    char status[20];
    char adminRemarks[80];
    bool isActive;

public:
    // 1. Default Constructor
    Grievance() {
        grievanceId = 0;
        safeCopy(citizenName, "Unknown", sizeof(citizenName));
        safeCopy(department, "General", sizeof(department));
        safeCopy(category, "Unclassified", sizeof(category));
        safeCopy(description, "No description provided.", sizeof(description));
        safeCopy(status, "Pending", sizeof(status));
        safeCopy(adminRemarks, "Awaiting officer review.", sizeof(adminRemarks));
        isActive = false;
    }

    // 2. Parameterized Constructor
    Grievance(int id, const char* name, const char* dept, const char* cat, const char* desc) {
        grievanceId = id;
        safeCopy(citizenName, name, sizeof(citizenName));
        safeCopy(department, dept, sizeof(department));
        safeCopy(category, cat, sizeof(category));
        safeCopy(description, desc, sizeof(description));
        safeCopy(status, "Pending", sizeof(status));
        safeCopy(adminRemarks, "Complaint registered; assigned to desk.", sizeof(adminRemarks));
        isActive = true;
    }

    // 3. Copy Constructor
    Grievance(const Grievance& other) {
        grievanceId = other.grievanceId;
        safeCopy(citizenName, other.citizenName, sizeof(citizenName));
        safeCopy(department, other.department, sizeof(department));
        safeCopy(category, other.category, sizeof(category));
        safeCopy(description, other.description, sizeof(description));
        safeCopy(status, other.status, sizeof(status));
        safeCopy(adminRemarks, other.adminRemarks, sizeof(adminRemarks));
        isActive = other.isActive;
    }

    // 4. Destructor
    ~Grievance() {
        // Automatic cleanup when Grievance object goes out of scope
    }

    // Getters & Setters
    int getId() const { return grievanceId; }
    bool getActive() const { return isActive; }
    const char* getStatus() const { return status; }

    void updateProgress(const char* newStatus, const char* remarks) {
        safeCopy(status, newStatus, sizeof(status));
        safeCopy(adminRemarks, remarks, sizeof(adminRemarks));
    }

    // Display single record in formatted table row
    void displayRow() const {
        cout << left << setw(8) << grievanceId
             << setw(18) << citizenName
             << setw(16) << department
             << setw(14) << status
             << setw(28) << adminRemarks << endl;
    }

    // Display full detailed card
    void displayDetails() const {
        cout << "\n------------------------------------------------------------\n";
        cout << " Grievance ID   : " << grievanceId << "\n";
        cout << " Citizen Name   : " << citizenName << "\n";
        cout << " Department     : " << department << "\n";
        cout << " Category       : " << category << "\n";
        cout << " Description    : " << description << "\n";
        cout << " Current Status : " << status << "\n";
        cout << " Admin Remarks  : " << adminRemarks << "\n";
        cout << "------------------------------------------------------------\n";
    }
};

class GrievanceTracker {
private:
    // Helper: Returns 0-based record index in binary file, or -1 if not found
    int findRecordIndex(int searchId) {
        ifstream inFile(DATA_FILE, ios::binary | ios::in);
        if (!inFile) return -1;

        Grievance temp;
        int index = 0;
        while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Grievance))) {
            if (temp.getActive() && temp.getId() == searchId) {
                inFile.close();
                return index;
            }
            index++;
        }
        inFile.close();
        return -1;
    }

public:
    // Create & Persist new Grievance to binary file
    void lodgeGrievance() {
        int id;
        char name[40], dept[30], cat[25], desc[120];

        cout << "\n--- Lodge New Citizen Grievance ---\n";
        cout << "Enter Unique Grievance ID (e.g., 1001): ";
        while (!(cin >> id) || id <= 0) {
            cout << "Invalid ID! Enter a positive integer: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Duplicate Key Prevention across persistent file
        if (findRecordIndex(id) != -1) {
            cout << "[ERROR] Grievance ID " << id << " already exists in database!\n";
            return;
        }

        cout << "Enter Citizen Name      : ";
        cin.getline(name, sizeof(name));
        cout << "Enter Target Department : ";
        cin.getline(dept, sizeof(dept));
        cout << "Enter Issue Category    : ";
        cin.getline(cat, sizeof(cat));
        cout << "Enter Issue Description : ";
        cin.getline(desc, sizeof(desc));

        // Invoke Parameterized Constructor
        Grievance newRecord(id, name, dept, cat, desc);

        // Demonstrate Copy Constructor before serialization
        Grievance recordToSave(newRecord);

        ofstream outFile(DATA_FILE, ios::binary | ios::app);
        if (!outFile) {
            cout << "[ERROR] Unable to open binary storage file!\n";
            return;
        }
        outFile.write(reinterpret_cast<const char*>(&recordToSave), sizeof(Grievance));
        outFile.close();

        cout << "[SUCCESS] Grievance #" << id << " persisted to '" << DATA_FILE
             << "' (" << sizeof(Grievance) << " bytes).\n";
    }

    // Sequential Binary Read of all stored records
    void viewAllGrievances() {
        ifstream inFile(DATA_FILE, ios::binary | ios::in);
        if (!inFile) {
            cout << "\n[INFO] No records found. File '" << DATA_FILE << "' does not exist yet.\n";
            return;
        }

        cout << "\n====================================================================================\n";
        cout << left << setw(8) << "ID"
             << setw(18) << "Citizen Name"
             << setw(16) << "Department"
             << setw(14) << "Status"
             << setw(28) << "Admin Remarks" << endl;
        cout << "====================================================================================\n";

        Grievance temp;
        int count = 0;
        while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Grievance))) {
            if (temp.getActive()) {
                temp.displayRow();
                count++;
            }
        }
        inFile.close();

        if (count == 0) {
            cout << "No active grievance records available.\n";
        }
        cout << "====================================================================================\n";
        cout << "Total Persistent Records Loaded: " << count << "\n";
    }

    // Random-Access Binary Search using seekg()
    void searchGrievanceById() {
        int searchId;
        cout << "\nEnter Grievance ID to Search: ";
        cin >> searchId;

        int index = findRecordIndex(searchId);
        if (index == -1) {
            cout << "[NOT FOUND] No grievance record found with ID #" << searchId << ".\n";
            return;
        }

        ifstream inFile(DATA_FILE, ios::binary | ios::in);
        streampos byteOffset = static_cast<streampos>(index) * sizeof(Grievance);

        // Random access jump directly to record byte offset
        inFile.seekg(byteOffset, ios::beg);

        Grievance foundRecord;
        inFile.read(reinterpret_cast<char*>(&foundRecord), sizeof(Grievance));
        inFile.close();

        cout << "[SEEKG SUCCESS] Located at Record Index #" << index
             << " (Byte Offset: " << byteOffset << ")";
        foundRecord.displayDetails();
    }

    // In-Place Binary File Update using seekp()
    void updateGrievanceStatus() {
        int targetId;
        cout << "\nEnter Grievance ID to Update: ";
        cin >> targetId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int index = findRecordIndex(targetId);
        if (index == -1) {
            cout << "[NOT FOUND] Cannot update. Grievance ID #" << targetId << " does not exist.\n";
            return;
        }

        fstream ioFile(DATA_FILE, ios::binary | ios::in | ios::out);
        if (!ioFile) {
            cout << "[ERROR] Unable to open binary file in read/write mode.\n";
            return;
        }

        streampos byteOffset = static_cast<streampos>(index) * sizeof(Grievance);

        // 1. Seek read pointer to fetch existing record
        ioFile.seekg(byteOffset, ios::beg);
        Grievance record;
        ioFile.read(reinterpret_cast<char*>(&record), sizeof(Grievance));

        cout << "\nCurrent Record Details:";
        record.displayDetails();

        int statusChoice;
        char newStatus[20];
        char remarks[80];

        cout << "Select New Status:\n";
        cout << "  1. In-Progress\n  2. Resolved\n  3. Rejected\nChoice: ";
        cin >> statusChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (statusChoice == 1) safeCopy(newStatus, "In-Progress", sizeof(newStatus));
        else if (statusChoice == 2) safeCopy(newStatus, "Resolved", sizeof(newStatus));
        else if (statusChoice == 3) safeCopy(newStatus, "Rejected", sizeof(newStatus));
        else {
            cout << "[CANCELLED] Invalid status choice.\n";
            ioFile.close();
            return;
        }

        cout << "Enter Official Admin Remarks: ";
        cin.getline(remarks, sizeof(remarks));

        record.updateProgress(newStatus, remarks);

        // 2. Seek write pointer to exact same byte offset and overwrite in-place
        ioFile.seekp(byteOffset, ios::beg);
        ioFile.write(reinterpret_cast<const char*>(&record), sizeof(Grievance));
        ioFile.close();

        cout << "[SEEKP SUCCESS] Record #" << targetId << " updated in-place at Byte Offset "
             << byteOffset << ".\n";
    }
};

int main() {
    GrievanceTracker tracker;
    int choice;

    do {
        cout << "\n======================================================\n";
        cout << "  ONLINE COMPLAINT & GRIEVANCE TRACKING SYSTEM (CA2)  \n";
        cout << "======================================================\n";
        cout << "  1. Lodge New Grievance (Save to Binary File)\n";
        cout << "  2. View All Persisted Grievances (Sequential Read)\n";
        cout << "  3. Track Grievance by ID (Random Access - seekg)\n";
        cout << "  4. Update Grievance Status (In-Place - seekp)\n";
        cout << "  5. Exit Application\n";
        cout << "------------------------------------------------------\n";
        cout << "Enter your choice (1-5): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: tracker.lodgeGrievance(); break;
            case 2: tracker.viewAllGrievances(); break;
            case 3: tracker.searchGrievanceById(); break;
            case 4: tracker.updateGrievanceStatus(); break;
            case 5: cout << "Exiting system. All records safely persisted in '" << DATA_FILE << "'.\n"; break;
            default: cout << "Invalid choice! Please select 1-5.\n";
        }
    } while (choice != 5);

    return 0;
}

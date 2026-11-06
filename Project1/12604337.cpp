#include <string>
using namespace std;
#include <iostream>

class Complaint {
private:
    int complaintID;
    string studentName, category, description, status;

public:
    Complaint() {
        complaintID = 0;
        status = "Pending";
    }

    void setDetails(int id, string name, string type,
                    string detail) {
        this->complaintID = id;
        this->studentName = name;
        this->category = type;
        this->description = detail;
        this->status = "Pending";
    }

    int getID() {
        return complaintID;
    }

    void display() {
        cout << "\nComplaint ID: " << complaintID;
        cout << "\nStudent: " << studentName;
        cout << "\nCategory: " << category;
        cout << "\nDescription: " << description;
        cout << "\nStatus: " << status << "\n";
    }
};

int main() {
    Complaint records[100];
    int count = 0, choice;

    do {
        cout << "\n--- Complaint Tracking System ---\n";
        cout << "1. Register Complaint\n";
        cout << "2. View All Complaints\n";
        cout << "3. Search Complaint by ID\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            if (count == 100) {
                cout << "Record limit reached.\n";
                continue;
            }

            int id;
            string name, category, description;
            cout << "Enter complaint ID: ";
            cin >> id;

            bool duplicate = false;
            for (int i = 0; i < count; i++) {
                if (records[i].getID() == id) {
                    duplicate = true;
                    break;
                }
            }

            if (id <= 0 || duplicate) {
                cout << "Invalid or duplicate ID.\n";
                continue;
            }

            cin.ignore();
            cout << "Enter student name: ";
            getline(cin, name);
            cout << "Enter category: ";
            getline(cin, category);
            cout << "Enter description: ";
            getline(cin, description);

            if (name.empty() || category.empty()
                || description.empty()) {
                cout << "Fields cannot be empty.\n";
                continue;
            }

            records[count].setDetails(id, name, category,
                                      description);
            count++;
            cout << "Complaint registered successfully.\n";
        }
        else if (choice == 2) {
            if (count == 0)
                cout << "No complaints available.\n";
            for (int i = 0; i < count; i++)
                records[i].display();
        }
        else if (choice == 3) {
            int id;
            bool found = false;
            cout << "Enter complaint ID to search: ";
            cin >> id;

            for (int i = 0; i < count; i++) {
                if (records[i].getID() == id) {
                    records[i].display();
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Complaint not found.\n";
        }
        else if (choice != 4) {
            cout << "Invalid menu choice.\n";
        }
    } while (choice != 4);

    cout << "Thank you.\n";
    return 0;
}

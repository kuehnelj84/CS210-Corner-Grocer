#include <iostream>
#include <fstream>
#include <map>
#include <string>

// Stores grocery items and the number of times each was purchased.
class GroceryTracker {
private:
    std::map<std::string, int> itemCounts;

public:
    // Reads each item and increases its count.
    bool LoadItems(const std::string& filename) {
        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Could not open " << filename << "\n";
            return false;
        }

        itemCounts.clear();
        std::string item;

        while (inputFile >> item) {
            ++itemCounts[item];
        }

        if (inputFile.bad()) {
            std::cerr << "Error reading the input file.\n";
            return false;
        }

        return true;
    }

    // Saves the counts automatically before the menu appears.
    bool SaveBackup(const std::string& filename) const {
        std::ofstream outputFile(filename);

        if (!outputFile.is_open()) {
            std::cerr << "Could not create the backup file.\n";
            return false;
        }

        for (const auto& entry : itemCounts) {
            outputFile << entry.first << " " << entry.second << "\n";
        }

        outputFile.close();
        return !outputFile.fail();
    }

    // Returns zero when the requested item is absent.
    int GetFrequency(const std::string& item) const {
        auto found = itemCounts.find(item);

        if (found != itemCounts.end()) {
            return found->second;
        }

        return 0;
    }

    void DisplayFrequencies() const {
        for (const auto& entry : itemCounts) {
            std::cout << entry.first << " " << entry.second << "\n";
        }
    }

    // Each asterisk represents one purchase.
    void DisplayHistogram() const {
        for (const auto& entry : itemCounts) {
            std::cout << entry.first << " "
                << std::string(entry.second, '*') << "\n";
        }
    }
};

void DisplayMenu() {
    std::cout << "\n===== Corner Grocer =====\n";
    std::cout << "1. Search for an item\n";
    std::cout << "2. Display all item frequencies\n";
    std::cout << "3. Display histogram\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    GroceryTracker tracker;

    // Load the provided data and create the backup at startup.
    if (!tracker.LoadItems("CS210_Project_Three_Input_File.txt")) {
        return 1;
    }

    if (!tracker.SaveBackup("frequency.dat")) {
        std::cerr << "The backup could not be saved.\n";
        return 1;
    }

    std::string choice;

    do {
        DisplayMenu();

        if (!std::getline(std::cin, choice)) {
            break;
        }

        if (choice == "1") {
            std::string item;
            std::cout << "Enter an item (example: Spinach): ";

            if (!std::getline(std::cin, item)) {
                break;
            }

            std::cout << item << ": "
                << tracker.GetFrequency(item)
                << " purchases\n";
        }
        else if (choice == "2") {
            tracker.DisplayFrequencies();
        }
        else if (choice == "3") {
            tracker.DisplayHistogram();
        }
        else if (choice == "4") {
            std::cout << "Goodbye!\n";
        }
        else {
            std::cout << "Please enter 1, 2, 3, or 4.\n";
        }
    } while (choice != "4");

    return 0;
}
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <map>
using namespace std;
namespace fs = std::filesystem;

// Global map to store loyalty points (contactNumber -> points)
map<string, int> loyaltyPoints;

// Load loyalty points from file
void loadLoyaltyPoints() {
    ifstream file("loyalty.txt");
    string contact;
    int points;
    while (file >> contact >> points) {
        loyaltyPoints[contact] = points;
    }
    file.close();
}

// Save loyalty points to file
void saveLoyaltyPoints() {
    ofstream file("loyalty.txt");
    for (auto &entry : loyaltyPoints) {
        file << entry.first << " " << entry.second << endl;
    }
    file.close();
}

void findPreviousBill(){
    string keyword;
    cout << "\nEnter customer name or contact number to search: ";
    cin.ignore();
    getline(cin, keyword);
    for (char &ch : keyword) {
    if (ch == ' ') ch = '_';
    }
    bool found = false;
    for (const auto& entry : fs::directory_iterator(".")){
        string filename = entry.path().filename().string();

        if (filename.find("bill_") != string::npos && 
            (filename.find(keyword) != string::npos)){
            
            cout << "\nBill Found in File: " << filename << "\n";
            ifstream file(filename);
            string line;
            while (getline(file, line)){
                cout << line << endl;
            }
            file.close();
            found = true;
        }
    }

    if (!found){
        cout << "No matching bill found!\n";
    }
}

class Product{
    public:
        string name;
        int quantity;
        float pricePerUnit;

        void input(){
            cout << "\nEnter product name: ";
            cin.ignore();
            getline(cin, name);

            while(true){
                cout << "Enter quantity: ";
                cin >> quantity;
                if (cin.fail() || quantity < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Quantity must be a positive number.\n";
                } else break;
            }

            while(true){
                cout << "Enter price per unit: ";
                cin >> pricePerUnit;
                if (cin.fail() || pricePerUnit < 0) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input! Price must be a positive number.\n";
                } else break;
            }
        }

        float getTotalPrice(){
            return quantity * pricePerUnit;
        }

        void display(){
            cout << left << setw(20) << name
                 << setw(10) << quantity
                 << setw(15) << pricePerUnit
                 << setw(10) << getTotalPrice() << endl;
        }
}; 

class Bill{
    Product products[100];
    int productCount;
    float discountPercentage;
    string customerName;
    string contactNumber;
    int billNumber;

    public:
        Bill(){
            productCount = 0;
            discountPercentage = 0;
            billNumber = 1000 + rand() % (10000 - 1000 + 1);
        } 
        
        void getCustomerDetails(){
            cout << "\nEnter customer name: ";
            getline(cin, customerName);
            cout << "Enter contact number: ";
            getline(cin, contactNumber);
        }

        void addProduct(){
            products[productCount].input();
            productCount++;
        }

        void applyDiscount(){
            cout << "\nEnter discount percentage (if any): ";
            cin >> discountPercentage;
        }

        float calculateTotal(){
            float total = 0;
            for (int i = 0; i < productCount; i++){
                total += products[i].getTotalPrice();
            }
            return total;
        } 

        void printBill(){
            float total = calculateTotal();
            float discountAmount = (discountPercentage / 100) * total;

            // Redeem loyalty points
            int availablePoints = loyaltyPoints[contactNumber];
            float pointsDiscount = 0.0;

            if (availablePoints >= 1) {
                cout << "\nCustomer has " << availablePoints << " loyalty points.";
                cout << "\nDo you want to redeem points for discount? (y/n): ";
                char usePoints;
                cin >> usePoints;

                if (usePoints == 'y' || usePoints == 'Y') {
                    pointsDiscount = availablePoints * 0.1; // 1 point = 0.1 Tk
                    int pointsUsed = (int)(pointsDiscount / 0.1);
                    loyaltyPoints[contactNumber] -= pointsUsed;
                    cout << "Discount applied from points: " << pointsDiscount << " Tk\n";
                }
            }

            float grandTotal = total - discountAmount - pointsDiscount;
            if (grandTotal < 0) grandTotal = 0;

            // Earn new loyalty points (100 Tk spent = 10 points)
            int earnedPoints = (int)(grandTotal / 100) * 10;
            loyaltyPoints[contactNumber] += earnedPoints;

            // Display bill
            time_t now = time(0);
            string dt = ctime(&now);
            cout << "\n=============== Super Shop Bill ===============\n";
            cout << "Bill No: " << billNumber << "\t\tDate: " << dt;
            cout << "Customer: " << customerName << endl;
            cout << "Contact No: " << contactNumber << endl;
            cout << left << setw(20) << "Product"
                 << setw(10) << "Qty"
                 << setw(15) << "Price/unit"
                 << setw(10) << "Total" << endl;
            cout << "------------------------------------------------\n";
            for (int i = 0; i < productCount; i++){
                products[i].display();
            }
            cout << "------------------------------------------------\n";
            cout << right << setw(40) << "Total: " << total << endl;
            cout << right << setw(40) << "Discount: " << discountAmount << endl;
            cout << right << setw(40) << "Redeem Discount: " << pointsDiscount << endl;
            cout << right << setw(40) << "Grand Total: " << grandTotal << endl;
            cout << "Loyalty Points Earned: " << earnedPoints << endl;
            cout << "Total Loyalty Points: " << loyaltyPoints[contactNumber] << endl;
            cout << "================================================\n";
            cout << "\tThank You for Shopping!\n";

            saveLoyaltyPoints();
            saveToFile(dt, total, discountAmount, grandTotal);
        }

        void saveToFile(string dt, float total, float discountAmount, float grandTotal){
            string filename = "bill_" + to_string(billNumber) + "_" + customerName + "_" + contactNumber + ".txt";
            for (char &ch : filename){
                if (ch == ' ') ch = '_';
            }
            ofstream file(filename);
            if (!file){
                cout << "Error saving the bill to file!\n";
                return;
            }

            file << "=============== Super Shop Bill ===============\n";
            file << "Bill No: " << billNumber << "\t\tDate: " << dt;
            file << "Customer: " << customerName << endl;
            file << "Contact No: " << contactNumber << endl;
            file << left << setw(20) << "Product"
                 << setw(10) << "Qty"
                 << setw(15) << "Price/unit"
                 << setw(10) << "Total" << endl;
            file << "------------------------------------------------\n";
            for (int i = 0; i < productCount; i++){
                file << left << setw(20) << products[i].name
                     << setw(10) << products[i].quantity
                     << setw(15) << products[i].pricePerUnit
                     << setw(10) << products[i].getTotalPrice() << endl;
            }
            file << "------------------------------------------------\n";
            file << right << setw(40) << "Total: " << total << endl;
            file << right << setw(40) << "Discount: " << discountAmount << endl;
            file << right << setw(40) << "Grand Total: " << grandTotal << endl;
            file << "================================================\n";
            file << "\t\t\t\tThank You for Shopping!\n";
            file.close();
        }
}; 

int main(){
    srand(time(0));
    loadLoyaltyPoints(); // Load points at program start

    Bill customer[100];
    int choice, subChoice, customerCount = 0;

    cout << "===== Welcome to Super Shop Billing System =====\n";
    do{
        cout << "\n1. Create New Bill for Customer\n"
             << "2. Search Previous Bill\n"
             << "3. Exit\n";
        while(true){
            cout << "Enter your choice: ";
            cin >> choice;
            if (cin.fail() || choice <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Choose between 1-3.\n";
            } else break;
        }

        switch (choice){
            case 1:
                cin.ignore();
                customer[customerCount].getCustomerDetails(); 
                do{
                    cout << "\n1. Add Product\n2. Apply Discount\n3. Print and Save Bill\n";
                    while(true){
                        cout << "Enter your choice: ";
                        cin >> subChoice;
                        if (cin.fail() || subChoice <= 0) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid input! Choose between 1-3.\n";
                        } else break;
                    }

                    switch(subChoice){
                        case 1:
                            customer[customerCount].addProduct();
                            break;
                        case 2:
                            customer[customerCount].applyDiscount();
                            break;
                        case 3:
                            customer[customerCount].printBill();
                            break;
                        default:
                            cout << "Invalid choice! Try again.\n";
                    }
                } while (subChoice != 3);
                customerCount++;
                break;
            case 2:
                findPreviousBill();
                break;
            case 3:
                cout << "Thank you! Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 3);

    return 0;
}

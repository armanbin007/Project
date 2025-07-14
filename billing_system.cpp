#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
using namespace std;
class Product{
    public:
        string name;
        int quantity;
        float pricePerUnit;
        void input(){
            cout << "\nEnter product name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter quantity: ";
            cin >> quantity;
            cout << "Enter price per unit: ";
            cin >> pricePerUnit;
        } float getTotalPrice(){
            return quantity * pricePerUnit;
        } void display(){
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
    int billNumber;
    public:
        Bill(){
            productCount = 0;
            discountPercentage = 0;
            billNumber = 1000 + rand() % (10000 - 1000 + 1); // Random bill number between 1000-10000
        } 
        void getCustomerDetails(){
            cout << "\nEnter customer name: ";
            getline(cin, customerName);
        } void addProduct(){
                products[productCount].input();
                productCount++;
        } void applyDiscount(){
                cout << "\nEnter discount percentage (if any): ";
                cin >> discountPercentage;
        } float calculateTotal(){
                float total = 0;
                for (int i = 0; i < productCount; i++){
                    total += products[i].getTotalPrice();
                }
                return total;
        } 
        void printBill(){
            float total = calculateTotal();
            float discountAmount = (discountPercentage / 100) * total;
            float grandTotal = total - discountAmount;

            // Get current date and time
            time_t now = time(0);
            string dt = ctime(&now);

            cout << "\n=============== Super Shop Bill ===============\n";
            cout << "Bill No: " << billNumber << "\t\tDate: " << dt;
            cout << "Customer: " << customerName << endl;
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
            cout << right << setw(40) << "Grand Total: " << grandTotal << endl;
            cout << "================================================\n";
            cout << "\tThank You for Shopping!\n";

            saveToFile(dt, total, discountAmount, grandTotal);
        }
    void saveToFile(string dt, float total, float discountAmount, float grandTotal){
        ofstream file("bill.txt", ios::app);
        if (!file) {
            cout << "Error saving the bill to file!\n";
            return;
        }

        file << "=============== Super Shop Bill ===============\n";
        file << "Bill No: " << billNumber << "\t\tDate: " << dt;
        file << "Customer: " << customerName << endl;
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
        cout << "\nBill saved successfully to 'bill.txt'.\n";
    }
}; 
int main(){
    srand(time(0)); // Seed for random bill number
    Bill customer[100];
    int choice, subChoice, customerCount = 0;

    cout << "===== Welcome to Super Shop Billing System =====\n";
    do{
        cout << "\n1. Create New Bill for Customer\n"
             << "2. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        switch (choice){
    case 1:
        cin.ignore();
        customer[customerCount].getCustomerDetails(); 
    do{
        cout << "\n1. Add Product\n2. Apply Discount\n3. Print and Save Bill\n";
        cout << "Enter your choice: ";
        cin >> subChoice;

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
        cout << "Thank you! Exiting...\n";
        break;
    default:
        cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 2);
    return 0;
}

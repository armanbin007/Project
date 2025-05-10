#include <iostream>
#include <iomanip>
using namespace std;

class Account{
public:
    string accountHolder;
    int accountNumber;
    float balance;

    void createAccount(){
        cout << "\nEnter Account Holder's Name: ";
        cin.ignore();
        getline(cin, accountHolder);
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }


    void displayAccount(){
        cout << left << setw(20) << accountHolder
             << setw(15) << accountNumber
             << setw(10) << balance << endl;
    }
    void deposit(float amount){
        balance += amount;
    }

    int withdraw(float amount){
        if(amount > balance){
            return -1;
        } else{
            balance -= amount;
            return 1;
        }
    }
};


class Bank : public Account{
    Account accounts[100];
    int count;
public:
    Bank(){
        count = 0;
    }
    void addAccount(){
        accounts[count].createAccount();
        count++;
    }
    void showAllAccounts(){
        cout << "\n======= All Accounts =======\n";
        cout << left << setw(20) << "Name"
             << setw(15) << "Account No"
             << setw(10) << "Balance" << endl;
        for (int i = 0; i < count; i++){
            accounts[i].displayAccount();
        }
    }
    void depositMoney(){
        int accNo;
        float amount;
        cout << "\nEnter Account Number to Deposit: ";
        cin >> accNo;
        for (int i = 0; i < count; i++){
            if(accounts[i].accountNumber == accNo){
                cout << "Enter amount to deposit: ";
                cin >> amount;
                accounts[i].deposit(amount);
                cout << "Deposit successful!\n";
                return;
            }
        }
        cout << "Account not found!\n";
    }

    void withdrawMoney(){
        int accNo;
        float amount;
        cout << "\nEnter Account Number to Withdraw: ";
        cin >> accNo;
        for (int i = 0; i < count; i++){
            if(accounts[i].accountNumber == accNo){
                cout << "Enter amount to withdraw: ";
                cin >> amount;
                if(accounts[i].withdraw(amount) != -1){
                    cout << "Withdrawal successful!\n";
                } else{
                    cout << "Insufficient balance!\n";
                }
                return;
            }
        }
        cout << "Account not found!\n";
    }
};

int main(){
    Bank bank;
    int choice;

    cout << "====== Welcome to Bank Management System ======\n";

    do{
        cout << "\n1. Create Account\n2. Show All Accounts\n3. Deposit Money\n4. Withdraw Money\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice){
        case 1:
            bank.addAccount();
            break;
        case 2:
            bank.showAllAccounts();
            break;
        case 3:
            bank.depositMoney();
            break;
        case 4:
            bank.withdrawMoney();
            break;
        case 0:
            cout << "Thank you! Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while(choice != 0);
    return 0;
}

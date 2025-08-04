#include<iostream>
#include<string>
using namespace std;

//---------------------------------- Node Class ----------------------------------//
class TreeNode 
{
public:
    string bankName;
    int netAmount;
    int height;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string name, int amount)
        : bankName(name), netAmount(amount), height(1), left(nullptr), right(nullptr) {}
};

//---------------------------------- Stack Node ----------------------------------//
class StackNode {
public:
    string debtor, creditor;
    int amount;
    StackNode* next;

    StackNode(string d, string c, int a)
        : debtor(d), creditor(c), amount(a), next(nullptr) {}
};

//---------------------------------- Stack ----------------------------------//
class Stack
{
    StackNode* top;
public:
    Stack() 
        : top(nullptr) {}

    void push(string debtor, string creditor, int amount)
    {
        StackNode* newNode = new StackNode(debtor, creditor, amount);
        newNode->next = top;
        top = newNode;
    }

    bool peek(string& debtor, string& creditor, int& amount)
    {
        if (!top) return false;
        debtor = top->debtor;
        creditor = top->creditor;
        amount = top->amount;
        return true;
    }

    void pop()
    {
        if (!top) return;
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }

    bool isEmpty()
    {
        return top == nullptr;
    }
};

//---------------------------------- Queue Node ----------------------------------//
class QueueNode 
{
public:
    string debtor, creditor;
    int amount;
    QueueNode* next;

    QueueNode(string d, string c, int a) 
        : debtor(d), creditor(c), amount(a), next(nullptr) {}
};

//---------------------------------- Queue ----------------------------------//
class Queue
{
    QueueNode* front;
    QueueNode* rear;
public:
    Queue()
        : front(nullptr), rear(nullptr) {}

    void enqueue(string debtor, string creditor, int amount) 
    {
        QueueNode* newNode = new QueueNode(debtor, creditor, amount);
        if (!rear) front = rear = newNode;
        else 
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    bool dequeue(string& debtor, string& creditor, int& amount)
    {
        if (!front) return false;
        QueueNode* temp = front;
        debtor = temp->debtor;
        creditor = temp->creditor;
        amount = temp->amount;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        return true;
    }

    bool isEmpty()
    {
        return front == nullptr;
    }
};

//---------------------------------- AVL Tree ----------------------------------//
class AVLTree
{
    TreeNode* root;

    int getHeight(TreeNode* node)
    {
        return node ? node->height : 0;
    }

    int getBalance(TreeNode* node)
    {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(TreeNode* node)
    {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    TreeNode* rotateRight(TreeNode* y)
    {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    TreeNode* rotateLeft(TreeNode* x)
    {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    TreeNode* insert(TreeNode* node, string bankName, int netAmount)
    {
        if (!node)
            return new TreeNode(bankName, netAmount);

        if (bankName < node->bankName)
            node->left = insert(node->left, bankName, netAmount);
        else if (bankName > node->bankName)
            node->right = insert(node->right, bankName, netAmount);
        else return node;

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && bankName < node->left->bankName)
            return rotateRight(node);
        if (balance < -1 && bankName > node->right->bankName)
            return rotateLeft(node);
        if (balance > 1 && bankName > node->left->bankName) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && bankName < node->right->bankName) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    TreeNode* search(TreeNode* node, string bankName) 
    {
        if (!node || node->bankName == bankName) return node;
        if (bankName < node->bankName) return search(node->left, bankName);
        return search(node->right, bankName);
    }

    void inOrder(TreeNode* node)
    {
        if (!node) return;
        inOrder(node->left);
        cout << "Bank: " << node->bankName << ", Balance: " << node->netAmount << endl;
        inOrder(node->right);
    }

    void filter(TreeNode* node, bool positive)
    {
        if (!node) return;
        filter(node->left, positive);
        if ((positive && node->netAmount > 0) || (!positive && node->netAmount < 0))
            cout << "Bank: " << node->bankName << ", Balance: " << node->netAmount << endl;
        filter(node->right, positive);
    }

public:
    AVLTree() : root(nullptr) {}

    void insertBank(string bankName, int netAmount) {
        root = insert(root, bankName, netAmount);
    }

    bool processTransaction(string debtor, string creditor, int amount) {
        TreeNode* d = search(root, debtor);
        TreeNode* c = search(root, creditor);
        if (!d || !c) return false;
        d->netAmount -= amount;
        c->netAmount += amount;
        return true;
    }

    bool findBank(string bankName, int& amount) {
        TreeNode* node = search(root, bankName);
        if (!node) return false;
        amount = node->netAmount;
        return true;
    }

    void displayAll() {
        cout << "\nAll Banks (Sorted):\n";
        inOrder(root);
    }

    void displayFiltered(bool positive) {
        cout << (positive ? "\nBanks with Positive Balance:\n" : "\nBanks with Negative Balance:\n");
        filter(root, positive);
    }
};

//---------------------------------- Main ----------------------------------//
int main() {
    AVLTree tree;
    Stack history;
    Queue pending;
    int numBanks;

    cout << "Enter number of banks: ";
    cin >> numBanks;

    for (int i = 0; i < numBanks; i++) {
        string name; int amount;
        cout << "Enter bank name and initial amount: ";
        cin >> name >> amount;
        tree.insertBank(name, amount);
    }

    int choice;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Process Transaction\n2. View Transaction History\n3. Retry Failed Transactions\n4. Search Bank\n5. Display All Banks\n6. Show Positive Balances\n7. Show Negative Balances\n0. Exit\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            string d, c; int amt;
            cout << "Enter debtor, creditor, amount: ";
            cin >> d >> c >> amt;
            if (tree.processTransaction(d, c, amt)) {
                history.push(d, c, amt);
                cout << "Transaction successful.\n";
            }
            else {
                pending.enqueue(d, c, amt);
                cout << "Transaction failed. Queued.\n";
            }
        }
        else if (choice == 2) {
            string d, c; int amt;
            if (history.peek(d, c, amt)) {
                cout << "Last transaction: " << d << " -> " << c << " | Amount: " << amt << endl;
            }
            else {
                cout << "No transaction history.\n";
            }
        }
        else if (choice == 3) {
            string d, c; int amt;
            while (pending.dequeue(d, c, amt)) {
                if (tree.processTransaction(d, c, amt)) {
                    history.push(d, c, amt);
                    cout << "Retried & successful: " << d << " -> " << c << endl;
                }
                else {
                    cout << "Still failed: " << d << " -> " << c << endl;
                    pending.enqueue(d, c, amt);
                    break;
                }
            }
        }
        else if (choice == 4) {
            string name; int amt;
            cout << "Enter bank name to search: ";
            cin >> name;
            if (tree.findBank(name, amt))
                cout << "Bank: " << name << ", Net Amount: " << amt << endl;
            else
                cout << "Bank not found.\n";
        }
        else if (choice == 5) {
            tree.displayAll();
        }
        else if (choice == 6) {
            tree.displayFiltered(true);
        }
        else if (choice == 7) {
            tree.displayFiltered(false);
        }
    } while (choice != 0);

    return 0;
}

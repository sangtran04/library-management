#include <iostream>
#include <array>
#include <windows.h>
#include <sstream>
#include <stdlib.h>
#include <fstream>

using namespace std;

int quantity = 0;

int GenerateBookID(string basicString);

int GenerateAlphabetID(char c);

int queueArray[20];

struct BinarySearchTree *rack[10][5];

struct User *start = nullptr;

class Queue {
public:
    int top;

    Queue() {
        top = -1;
    }

    bool IsFull() {
        return top == 19;
    }

    bool IsEmpty() {
        return top == -1;
    }

    void EnQueue(int value) {
        if (!IsFull()) {
            top++;
            queueArray[top] = value;
        } else {
            cout << "Hàng đợi đã đầy!";
        }
    }

    int DeQueue() {
        int value = queueArray[0];
        for (int i = 0; i < top; ++i) {
            queueArray[i] = queueArray[i + 1];
        }
        top--;
        return value;
    }

    void Reset() {
        top = -1;
    }

    int GetTop() {
        return top;
    }
};

struct Book {
private:
    string name;
    string author;
    int id, amount;
    int rack;
    struct Book *next;

public:
    Book(string newName, string newAuthor, int newID, int newRack) {
        name = newName;
        author = newAuthor;
        id = newID;
        rack = newRack;
        next = nullptr;
        amount = 1;
    }
};

struct BinarySearchTree {
private:
    int id;
    struct BinarySearchTree *left, *right;
    struct Book *next;
public:
    BinarySearchTree(int newID) {
        id = newID;
        left = nullptr;
        right = nullptr;
        next = nullptr;
    }
};

struct User {
private:
    string name;
    struct Book *book;
    struct User *next;
public:
    User(string newName) {
        name = newName;
        next = nullptr;
        book = nullptr;
    }
};


BinarySearchTree *SearchBinarySearchTree(struct BinarySearchTree *root, int id);

Book *SearchBook(int rack_no, int shelve, string name, string author_name);

Book *SearchPreviousBook(struct BinarySearchTree *root, string name, string author_name);

bool SearchBookcheck(struct BinarySearchTree *root, string name, string author_name);

bool DeleteBook(string name, string author_name);

Book *SearchBook(struct BinarySearchTree *bst, string name, string author_name);

bool SearchingSearchBook(struct BinarySearchTree *bst, string name, int rack, int shelve);

void GenerateLibraryData();

Book *SearchBookmain(struct BinarySearchTree *root, string name, string author_name);

void AddBook(int shelve, string book_name, string author_name);

BinarySearchTree *CreateBinarySearchTree(int id, struct BinarySearchTree *root);

void DisplayLibrary(struct BinarySearchTree *root);

Queue queue;

int main(void) {
    GenerateLibraryData();

    int choice;

    while (true) {
        cout << "------MENU------\n";
        cout << "0. Thoát\n";
        cout << "1. Hiển thị tất cả các đầu sách\n";
        cout << "2. Tìm kiếm sách theo Tên\n";
        cout << "3. Thêm sách mới\n";
        cout << "3. Xóa sách theo Tên\n";
        cout << "\nVui lòng nhập lựa chọn của bạn (Nhập số tương ứng): ";

        cin >> choice;
        switch (choice) {
            case 0: {
                exit(0);
            }
            case 1: {
                cout << "------DANH SÁCH TẤT CẢ CÁC ĐẦU SÁCH TRONG THƯ VIỆN------";
                for (int rackIndex = 0; rackIndex < 3; ++rackIndex) {
                    cout << "\n---   GIÁ SÁCH SỐ " << rackIndex + 1 << "   ---n";

                    for (int shelveIndex = 0; shelveIndex < 10; ++shelveIndex) {
                        cout << "\n+++ KỆ SÁCH SỐ " << shelveIndex + 1 << " +++\n";
                        DisplayLibrary(rack[shelveIndex][rackIndex]);
                    }
                }
                break;
            }
            case 2: {
                string name;
                bool found = false;
                cout << "------TÌM KIẾM SÁCH THEO TÊN------\n";
                cout << "\nNhập tên sách bạn muốn tìm: ";
                cin.ignore();
                getline(cin, name);

                int id = GenerateBookID(name);
                for (int rackIndex = 0; rackIndex < 3; ++rackIndex) {
                    int shelveIndex = id % 10;

                    struct BinarySearchTree *bst = SearchBinarySearchTree(rack[shelveIndex][rackIndex], id);

                    if (bst != nullptr) {
                        found = SearchingSearchBook(bst, name, rackIndex, shelveIndex);
                        break;
                    }
                }
                if (!found) {
                    cout << "\n\nRất tiếc! Thư viện không tồn tại sách \"" << name << "\"";
                }
                break;
            }
            case 3: {
                string book_name;
                string author_name;

                cout << "------THÊM SÁCH MỚI------\n";
                cout << "0. Trở lại\n";
                cout << "1. Toán học\n";
                cout << "2. Văn học\n";
                cout << "3. Khác\n";
                cout << "\nVui lòng nhập lựa chọn của bạn (Nhập số tương ứng): ";
                cin >> choice;
                if (choice == 0) break;
                else {
                    cout << "\nTên sách: ";

                    cin.ignore();
                    getline(cin, book_name);

                    cout << "\nTên tác giả: ";
                    getline(cin, author_name);

                    AddBook(choice - 1, book_name, author_name);
                }
                break;

            }
            case 4: {
                string name, author_name;
                cout << "------XÓA SÁCH THEO TÊN------\n";
                cout << "\nNhập tên sách bạn muốn xóa: ";
                cin.ignore();
                getline(cin, name);
                cout << "\nNhập tên tác giả: ";
                getline(cin, author_name);
                bool deleted = DeleteBook(name, author_name);
                if (deleted) {
                    cout << "\n\nXóa sách thành công";
                }
                else {
                    cout << "\n\nRất tiếc! Thư viện không tồn tại sách \"" << name << "\" của tác giả \"" << author_name << "\"";
                }
                break;
            }
            default: {
                cout << "\nChức năng không tồn tại. Vui lòng nhập lại!";
            }
        }
    }
}

int GenerateBookID(string str) {
    queue.Reset();
    int value = 0;
    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == ' ') {
            queue.EnQueue(value);
            value = 0;
        } else {
            value += GenerateAlphabetID(str[i]);
        }
    }
    queue.EnQueue(value);
    value = 0;

    stringstream string_stream;

    while (!queue.IsEmpty()) {
        string_stream << queue.DeQueue();
    }

    string combining_values = string_stream.str();

    istringstream integer_string_stream(combining_values);

    integer_string_stream >> value;
    return value;
}

int GenerateAlphabetID(char c) {

    char alphabets[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                        't', 'u', 'v', 'w', 'x', 'y', 'z'};
    c = tolower(c);

    for (int i = 0; i < 26; ++i) {
        if (c == alphabets[i])
            return ++i;
    }

    return -1;
}

BinarySearchTree *SearchBinarySearchTree(struct BinarySearchTree *root, int id) {

    if (root == nullptr) {
        return nullptr;
    } else {
        while (root != nullptr) {
            if (root->id > id) root = root->left;
            else if (root->id < id) root = root->right;
            else return root;
        }
        return nullptr;
    }
}

void AddBook(int shelve, string book_name, string author_name) {
    int id = GenerateBookID(book_name);
    int row = id % 10;
    struct Book *new_book = new Book(book_name, author_name, id, row);

    if (rack[row][shelve] != nullptr) {
        struct BinarySearchTree *root = rack[row][shelve];
        struct BinarySearchTree *searched = SearchBinarySearchTree(root, id);

        if (searched == nullptr) {
            struct BinarySearchTree *new_BinarySearchTree = CreateBinarySearchTree(id, root);
            new_BinarySearchTree->next = new_book;
        }
        else {
            struct Book *searched_book = SearchBook(searched, book_name, author_name);

            if (searched_book != nullptr)
                searched_book->amount++;
            else {
                new_book->next = searched->next;
                searched->next = new_book;
            }
        }
    }
    else {
        rack[row][shelve] = new BinarySearchTree(id);
        rack[row][shelve]->next = new_book;
    }
}

BinarySearchTree *CreateBinarySearchTree(int id, struct BinarySearchTree *root) {
    struct BinarySearchTree *current = new BinarySearchTree(id);

    struct BinarySearchTree *p = root;
    struct BinarySearchTree *k = p;

    if (p == nullptr) {
        root = current;
    } else {
        while (p != nullptr) {
            k = p;
            if (p->id > id) p = p->left;
            else p = p->right;
        }

        if (k->id > id) k->left = current;
        else k->right = current;
    }
    return current;
}

void DisplayLibrary(struct BinarySearchTree *root) {
    if (root != nullptr) {
        DisplayLibrary(root->left);
        struct Book *book = root->next;
        while (book != nullptr) {
            printf("Mã sách: %-10d Tên sách: %-30s Tên tác giả: %10s Số lượng: %-10d \n", 
                   book->id,
                   book->name.c_str(),
                   book->author.c_str(),
                   book->amount);
            book = book->next;

        }

        DisplayLibrary(root->right);
    }
}

Book *SearchBook(struct BinarySearchTree *bst, string name, string author_name) {
    int id = GenerateBookID(name);
    struct Book *book = bst->next;

    while (book != nullptr) {
        if (book->name == name && book->author == author_name)
            return book;
        book = book->next;
    }

    return nullptr;
}

bool SearchingSearchBook(struct BinarySearchTree *bst, string name, int rack, int shelve) {
    int id = GenerateBookID(name);
    struct Book *book = bst->next;
    bool found = false;

    while (book != nullptr) {
        if (book->name == name) {
            found = true;
            cout << "\nTên sách: " << book->name << "       Mã sách: " << book->id << "   Giá sách số: " << rack + 1
                 << "  Kệ sách số: "
                 << shelve + 1 << "  Tên tác giả: " << book->author;
        }
        book = book->next;
    }

    return found;
}

bool DeleteBook(string name, string author_name) {
    int id = GenerateBookID(name);
    for (int c = 0; c < 5; ++c) {
        int shelve = id % 10;

        struct BinarySearchTree *bst = SearchBinarySearchTree(rack[shelve][c], id);

        if (bst != nullptr) {
            if (SearchBookcheck(bst, name, author_name)) {
                struct Book *book = SearchBookmain(bst, name, author_name);
                struct Book *prevbook = SearchPreviousBook(bst, name, author_name);
                if (book->name == name && book->author == author_name) {
                    struct Book *del_book = book;
                    if (book->amount > 1) {
                        book->amount--;
                    } else {
                        if (book->next == nullptr) {
                            bst->next = nullptr;
                        } else {
                            bst->next = book->next;
                        }
                    }
                    return true;
                } else if (prevbook != nullptr) {
                    struct Book *del_book = book->next;
                    if (book->next->name == name && book->next->author == author_name) {
                        if (book->next->amount > 1) {
                            book->next->amount--;
                            return true;
                        }
                        if (book->next->next != nullptr) {
                            book->next = book->next->next;
                        } else {
                            book->next = nullptr;
                        }
                        delete (del_book);
                        return true;
                    }

                }
            }
        }
    }

    return false;
}

Book *SearchPreviousBook(struct BinarySearchTree *root, string name, string author_name) {
    int id = GenerateBookID(name);
    struct Book *book = root->next;
    struct Book *prev_book = book;

    while (book != nullptr) {
        if (book->name == name && book->author == author_name)
            return prev_book;
        prev_book = book;
        book = book->next;
    }
    return nullptr;
}

Book *SearchBookmain(struct BinarySearchTree *root, string name, string author_name) {
    int id = GenerateBookID(name);
    struct Book *book = root->next;

    while (book != nullptr) {
        if (book->name == name && book->author == author_name)
            return root->next;

        book = book->next;
    }
    return nullptr;
}

bool SearchBookcheck(struct BinarySearchTree *root, string name, string author_name) {
    int id = GenerateBookID(name);
    struct Book *book = root->next;

    while (book != nullptr) {
        if (book->name == name && book->author == author_name)
            return true;

        book = book->next;
    }
    return false;
}

void GenerateLibraryData() {
    ifstream inFile;
    inFile.open("D:\\sach.txt");
    if (!inFile) {
        cerr << "Không thể mở file \"sach.txt\". Vui lòng đưa file này vào đúng thư mục sau: \"D:\\sach.txt\"";
        exit(1); 
    }

    int shelve;
    string shelve_str;
    string book_name;
    string author_name;
    while (getline(inFile, shelve_str)) {
        shelve = stoi(shelve_str);

        getline(inFile, book_name);
        getline(inFile, author_name);
        AddBook(shelve, book_name, author_name);
    }
    inFile.close();
}
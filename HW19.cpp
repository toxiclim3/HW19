#include <iostream>
#include <cstdio>

using namespace std;

/*
Створіть структуру Product (назва продукту, ціна, кількість, наявність на складі)
Запишіть у бінарний файл масив об'єктів структури типу Product.
Прочитайте їх з бінарного файлу.
*/

struct product 
{
    char* name;
    double price;
    bool stocked;
    int stockedAmount;
};

struct productList
{
    product* list;
    int size;
};

void makeProduct(product& pr) 
{
    const short gSize = 254;
    char tempChar[gSize];

    printf("Please enter the product's name: \n");
    cin.ignore();
    cin.getline(tempChar, gSize);

    int size = strlen(tempChar);
    pr.name = new char[size + 1];
    strcpy_s(pr.name, size + 1, tempChar);

    printf("Please enter the product's price: \n");
    cin >> pr.price;

    printf("Please enter the amount of %s stocked: \n", pr.name);
    cin >> pr.stockedAmount;

    pr.stocked = pr.stockedAmount > 0;
}

void makeProductList(productList &list)
{
    cout << "enter the ammount of products to add\n";
    cin >> list.size;
    

    list.list = new product[list.size];
    for (int i = 0; i < list.size; i++)
    {
        printf("Product [%i]\n", i+1);
       makeProduct(list.list[i]);
    }
}

bool openFile(FILE*& file, const char* filePath, const char* mode) 
{
    if (fopen_s(&file, filePath, mode) != 0) {
        cout << "Error opening file: " << filePath << endl;
        return false;
    }
    return true;
}

void writeProductList(FILE*& file, const productList& pr)
{
    fwrite(&pr.size, sizeof(int), 1, file);
    fwrite(pr.list, sizeof(product), pr.size, file);
}

int readProducts(FILE*& file, productList& products, int maxSize)
{
    fread(&products.size, sizeof(int), 1, file);
    products.list = new product[products.size];
    fread(products.list, sizeof(product), products.size, file);
    return products.size;
}

void closeFile(FILE*& file) 
{
    if (fclose(file) == EOF) {
        cout << "The file has NOT been closed\n";
    }
    else {
        cout << "The file has been closed\n";
    }
}

void clearMemory(productList list)
{
    for (int i = 0; i < list.size; i++)
    {
        delete[] list.list[i].name;
    }
    delete[] list.list;
}

int main() 
{
    const char* filePath = "file.bin";

    FILE* file;
    if (!openFile(file, filePath, "ab")) {
        //return 1;
    }

    productList list;
    makeProductList(list);

    writeProductList(file, list);
    closeFile(file);

    if (!openFile(file, filePath, "rb")) {
        return 1;
    }
    int productCount = readProducts(file, list, 10);
    closeFile(file);

    for (int i = 0; i < productCount; ++i) {
        cout << "Name: " << list.list[i].name << endl;
        cout << "Price: " << list.list[i].price << endl;
        cout << "Stocked Amount: " << list.list[i].stockedAmount << endl;
        cout << "In Stock: " << (list.list[i].stocked ? "Yes" : "No") << endl;
        cout << endl;
    }

    clearMemory(list);
    return 0;
}

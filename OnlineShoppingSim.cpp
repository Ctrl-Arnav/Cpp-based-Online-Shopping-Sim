#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
class Product {
public:
int id;
string name, cat;
float price, disc, revs;
int qty;
Product(int i, string n, string c, float p, float d, int q, float r) :
id(i), name(n), cat(c), price(p), disc(d), qty(q), revs(r) {}
void show() const {
cout << "\nID: " << id
<< "\nName: " << name
<< "\nCategory: " << cat
<< "\nPrice: " << price
<< "\nDiscount: " << disc << "%"
<< "\nQuantity: " << qty
<< "\nReviews: " << revs << " stars\n";
}
float priceAfterDisc() const {
return price - (price * disc / 100);
}
};
vector<Product> catlog;
vector<Product> cart;
vector<Product> ords;
class User {
private:
string uid;
public:
string phone = "88551144";
string addr = "JIIT Sector-62 Noida, U.P.";
void login() {
cout << "Enter your name or login ID: ";
getline(cin, uid);
cout << "Welcome, " << uid << "!\n";
}
void cartMenu() {
int ch;
while (true) {
cout << "\n--- Cart Menu ---\n";
if (cart.empty()) cout << "Your cart is empty.\n";
else {
cout << "Products in Cart:\n";
for (const auto& p : cart) p.show();
}
cout << "\n1. Purchase\n2. Remove from Cart\n3. Exit Cart\nChoice: ";
cin >> ch; cin.ignore();
if (ch == 1) {
int pid;
cout << "Enter product ID to purchase: ";
cin >> pid; cin.ignore();
for (const auto& p : cart) {
if (p.id == pid) {
buy(p);
return;
}
}
cout << "Product not found in cart.\n";
} else if (ch == 2) {
int pid;
cout << "Enter product ID to remove: ";
cin >> pid; cin.ignore();
for (auto it = cart.begin(); it != cart.end(); ++it) {
if (it->id == pid) {
cart.erase(it);
cout << "Product removed from cart.\n";
break;
}
}
} else if (ch == 3) return;
else cout << "Invalid choice.\n";
}
}
void showOrds() {
cout << "\n--- Your Orders ---\n";
if (ords.empty()) cout << "No orders yet.\n";
else for (const auto& p : ords) p.show();
}
void settings() {
cout << "\n--- User Settings ---\n";
cout << "Current Contact: " << phone << "\nCurrent Address: " << addr << "\n";
cout << "Enter new contact: ";
getline(cin, phone);
cout << "Enter new address: ";
getline(cin, addr);
cout << "Details updated.\n";
}
void buy(Product p) {
cout << "\n--- Purchase Summary ---\n";
p.show();
float final = p.priceAfterDisc();
cout << "Price after discount: " << final << "\n";
cout << "Order placed successfully!\n";
cout << "Delivering to: " << addr << "\n";
ords.push_back(p);
for (auto it = cart.begin(); it != cart.end(); ++it) {
if (it->id == p.id) {
cart.erase(it);
break;
}
}
}
};
User user;
void saveProd(Product p) {
int ch;
cout << "\n1. Add to Cart\n2. Purchase Now\n3. Cancel\nChoice: ";
cin >> ch; cin.ignore();
if (ch == 1) {
cart.push_back(p);
cout << "Product added to cart.\n";
} else if (ch == 2) {
user.buy(p);
} else if (ch == 3) cout << "Purchase canceled";
else cout << "Invalid option.\n";
}
string toUpp(string s) {
for (char& c : s) if (c >= 'a' && c <= 'z') c -= 32;
return s;
}
void loadFile(const string& fname) {
ifstream f(fname);
string line;
int id = 1;
while (getline(f, line)) {
stringstream ss(line);
string name, cat;
float pr, dis, rev;
int qty;
getline(ss, name, ',');
getline(ss, cat, ',');
ss >> pr; ss.ignore();
ss >> dis; ss.ignore();
ss >> qty; ss.ignore();
ss >> rev;
catlog.push_back(Product(id++, name, cat, pr, dis, qty, rev));
}
f.close();
}
void sortList(vector<Product>& list, int mode) {
for (int i = 0; i < list.size(); i++) {
for (int j = i + 1; j < list.size(); j++) {
bool need = false;
if (mode == 3 && list[i].price > list[j].price) need = true;
if (mode == 4 && list[i].price < list[j].price) need = true;
if (mode == 5 && list[i].disc < list[j].disc) need = true;
if (mode == 6 && list[i].revs < list[j].revs) need = true;
if (need) swap(list[i], list[j]);
}
}
}
void find() {
int ch;
cout << "Choose a filter:\n";
cout << "1. Keyword\n2. Category\n3. Price (Low to High)\n4. Price (High to Low)\n5. Discount\n6.
Reviews\nChoice: ";
cin >> ch; cin.ignore();
vector<Product> res;
if (ch == 1) {
string kw;
cout << "Enter product name keyword: ";
getline(cin, kw);
kw = toUpp(kw);
for (const auto& p : catlog) {
if (toUpp(p.name).rfind(kw, 0) == 0) {
res.push_back(p);
break;
}
}
if (res.empty()) {
cout << "No product found.\n";
return;
}
res[0].show();
saveProd(res[0]);
} else {
res = catlog;
if (ch == 2) {
string cat;
cout << "Enter category: ";
getline(cin, cat);
vector<Product> tmp;
for (auto& p : res) if (p.cat == cat) tmp.push_back(p);
res = tmp;
} else sortList(res, ch);
for (auto& p : res) p.show();
char c;
cout << "Do you want to purchase a product? (y/n): ";
cin >> c;
if (c == 'y') {
int id;
cout << "Enter product ID to purchase: ";
cin >> id;
for (const auto& p : res) {
if (p.id == id) {
saveProd(p);
return;
}
}
cout << "Product with ID " << id << " not found in results.\n";
}
}
}
void showDeals() {
cout << "\n---Best Offers ---\n";
vector<Product> best;
for (const auto& p : catlog) if (p.disc > 0) best.push_back(p);
for (int i = 0; i < best.size(); i++)
for (int j = i + 1; j < best.size(); j++)
if (best[i].disc < best[j].disc) swap(best[i], best[j]);
for (int i = 0; i < min(3, (int)best.size()); i++) best[i].show();
char c;
cout << "Do you want to purchase a product? (y/n): ";
cin >> c;
if (c == 'y') {
int id;
cout << "Enter product ID: ";
cin >> id;
for (const auto& p : best) {
if (p.id == id) {
saveProd(p);
return;
}
}
cout << "Product not found in offers.\n";
}
}
void showCats() {
cout << "\n--- Categories ---\n";
cout << "1. Electronic Devices\n2. Clothes\n3. Toys\n4. Books\n5. Furniture\nChoose: ";
int ch;
cin >> ch; cin.ignore();
string cat;
switch (ch) {
case 1: cat = "Electronic Devices"; break;
case 2: cat = "Clothes"; break;
case 3: cat = "Toys"; break;
case 4: cat = "Books"; break;
case 5: cat = "Furniture"; break;
default: cout << "Invalid category.\n"; return;
}
vector<Product> res;
for (auto& p : catlog) if (p.cat == cat) res.push_back(p);
for (auto& p : res) p.show();
char c;
cout << "Do you want to purchase a product? (y/n): ";
cin >> c;
if (c == 'y') {
int pid;
cout << "Enter product ID: ";
cin >> pid;
for (const auto& p : res) {
if (p.id == pid) {
saveProd(p);
return;
}
}
cout << "Product not found in category.\n";
}
}
void menu(User& u) {
int ch;
while (true) {
cout << "\n--- Main Menu ---\n";
cout << "1. Cart\n2. Your Orders\n3. Search\n4. Offers\n5. Categories\n6. User Settings\n7.
Exit\nChoice: ";
cin >> ch; cin.ignore();
switch (ch) {
case 1: u.cartMenu(); break;
case 2: u.showOrds(); break;
case 3: find(); break;
case 4: showDeals(); break;
case 5: showCats(); break;
case 6: u.settings(); break;
case 7: cout << "Exiting. Thank you!\n"; return;
default: cout << "Invalid choice!\n";
}
}
}
int main() {
loadFile("Catalogue.txt");
user.login();
menu(user);
return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MenuItem {
    char *name;
    double price;
    struct MenuItem *next;
} MenuItem;

typedef struct PromoItem {
    char *name;
    double discount;
    struct PromoItem *next;
} PromoItem;

typedef struct Order {
    char *itemName;
    int quantity;
    double totalPrice;
    struct Order *next;
} Order;

typedef struct FavoriteItem {
    char *name;
    struct FavoriteItem *next;
} FavoriteItem;

typedef struct BSTNode {
    char *name;
    double price;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

void displayMainMenu();
void displaySubMenu(const char *fileName);
void placeOrder(MenuItem *menu, PromoItem *promo, Order **orders);
void viewOrder(Order *orders);
void viewBillAndPay(double totalBill);
void viewFavorites(FavoriteItem *favorites);
void addFavorite(MenuItem *menu, FavoriteItem **favorites);
BSTNode* createBSTNode(char *name, double price);
BSTNode* insertBSTNode(BSTNode *root, char *name, double price);
BSTNode* searchBSTNode(BSTNode *root, const char *name);
BSTNode* convertMenuListToBST(MenuItem *menu);

int main() {
    MenuItem *menuHead = NULL;
    PromoItem *promoHead = NULL;
    FavoriteItem *favoritesHead = NULL;
    Order *ordersHead = NULL;
    int choice;

    FILE *file;
    char *line = NULL;
    size_t len = 0;

    file = fopen("MUtama.txt", "r");
    if (file == NULL) {
        printf("Error opening menu file!\n");
        return 1;
    }
    while (getline(&line, &len, file) != -1) {
        char *name = strtok(line, "#\n");
        char *price = strtok(NULL, "#\n");

        MenuItem *newItem = malloc(sizeof(MenuItem));
        newItem->name = strdup(name);
        newItem->price = atof(price);
        newItem->next = menuHead;
        menuHead = newItem;
    }
    fclose(file);
    if (line) free(line);

    file = fopen("MPromo.txt", "r");
    if (file == NULL) {
        printf("Error opening promo file!\n");
        return 1;
    }
    while (getline(&line, &len, file) != -1) {
        char *name = strtok(line, "#\n");
        char *price = strtok(NULL, "#\n");

        PromoItem *newItem = malloc(sizeof(PromoItem));
        newItem->name = strdup(name);
        newItem->discount = atof(price);
        newItem->next = promoHead;
        promoHead = newItem;
    }
    fclose(file);
    if (line) free(line);

    // Convert MenuItem list to BST
    BSTNode *bstRoot = convertMenuListToBST(menuHead);

    do {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Choose a menu category:\n");
                printf("1. Menu Ayam\n");
                printf("2. Menu Minuman\n");
                printf("3. Menu Makanan Ringan\n");
                printf("4. Menu Sarapan\n");
                printf("5. Menu Makanan Utama\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                switch (choice) {
                    case 1:
                        displaySubMenu("MAyam.txt");
                        break;
                    case 2:
                        displaySubMenu("MMinuman.txt");
                        break;
                    case 3:
                        displaySubMenu("MMRingan.txt");
                        break;
                    case 4:
                        displaySubMenu("MSarapan.txt");
                        break;
                    case 5:
                        displaySubMenu("MUtama.txt");
                        break;
                    default:
                        printf("Invalid choice!\n");
                        continue;
                }
                break;
            case 2:
                printf("Displaying Promotions...\n");
                PromoItem *promoPtr = promoHead;
                while (promoPtr != NULL) {
                    printf("%s - Discount: %.2f%%\n", promoPtr->name, promoPtr->discount);
                    promoPtr = promoPtr->next;
                }
                break;
            case 3:
                placeOrder(menuHead, promoHead, &ordersHead);
                break;
            case 4:
                viewOrder(ordersHead);
                break;
            case 5:
                viewFavorites(favoritesHead);
                break;
            case 6:
                addFavorite(menuHead, &favoritesHead);
                break;
            case 7: {
                char itemName[100];
                printf("Enter the name of the item to search in the BST: ");
                scanf("%s", itemName);
                BSTNode *foundNode = searchBSTNode(bstRoot, itemName);
                if (foundNode) {
                    printf("Item found: %s - Rp %.2f\n", foundNode->name, foundNode->price);
                } else {
                    printf("Item not found in the BST.\n");
                }
                break;
            }
            case 0:
                printf("Exiting McDonald's app. Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 0);

    // Free allocated memory for linked lists
    while (menuHead != NULL) {
        MenuItem *temp = menuHead;
        menuHead = menuHead->next;
        free(temp->name);
        free(temp);
    }
    while (promoHead != NULL) {
        PromoItem *temp = promoHead;
        promoHead = promoHead->next;
        free(temp->name);
        free(temp);
    }
    while (favoritesHead != NULL) {
        FavoriteItem *temp = favoritesHead;
        favoritesHead = favoritesHead->next;
        free(temp->name);
        free(temp);
    }
    while (ordersHead != NULL) {
        Order *temp = ordersHead;
        ordersHead = ordersHead->next;
        free(temp->itemName);
        free(temp);
    }

    return 0;
}

void displayMainMenu() {
    printf("===========================================================\n");
    printf("                   Welcome to McDonald\n");
    printf("1.) View Menu\n");
    printf("2.) View Promo\n");
    printf("3.) Order\n");
    printf("4.) View Order\n");
    printf("5.) View Favorite\n");
    printf("6.) Add Menu to Favorite\n");
    printf("7.) Search Menu Item in BST\n");
    printf("0.) Exit\n");
    printf("===========================================================\n");
}

void displaySubMenu(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Menu:\n");
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        char *name = strtok(line, "#\n");
        char *price = strtok(NULL, "#\n");
        printf("%s - Rp %s\n", name, price);
    }
    fclose(file);
    if (line) free(line);
}

void placeOrder(MenuItem *menu, PromoItem *promo, Order **orders) {
    char itemName[100];
    int quantity;
    double totalPrice = 0.0;
    int found = 0;
    char continueOrdering = 'y';

    while (continueOrdering == 'y' || continueOrdering == 'Y') {
        printf("Enter item name: ");
        scanf("%s", itemName);
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        MenuItem *menuPtr = menu;
        while (menuPtr != NULL) {
            if (strcmp(menuPtr->name, itemName) == 0) {
                found = 1;
                totalPrice = menuPtr->price * quantity;
                printf("Total Price for %s: Rp %.2f\n", itemName, totalPrice);

                Order *newOrder = malloc(sizeof(Order));
                newOrder->itemName = strdup(itemName);
                newOrder->quantity = quantity;
                newOrder->totalPrice = totalPrice;
                newOrder->next = *orders;
                *orders = newOrder;
                break;
            }
            menuPtr = menuPtr->next;
        }
        if (!found) {
            printf("Item not found in the menu!\n");
        }
        printf("Do you want to order another item? (y/n): ");
        scanf(" %c", &continueOrdering);
    }
    printf("Order placed successfully!\n");
}

void viewOrder(Order *orders) {
    double totalBill = 0.0;
    printf("Viewing orders...\n");
    while (orders != NULL) {
        printf("Item: %s, Quantity: %d, Total Price: Rp %.2f\n", orders->itemName, orders->quantity, orders->totalPrice);
        totalBill += orders->totalPrice;
        orders = orders->next;
    }
    printf("Total Bill: Rp %.2f\n", totalBill);
    viewBillAndPay(totalBill);
}

void viewBillAndPay(double totalBill) {
    char paymentConfirmation;
    printf("Your total bill is: Rp %.2f\n", totalBill);
    printf("Do you want to proceed with Mobile Payment? (y/n): ");
    scanf(" %c", &paymentConfirmation);
    if (paymentConfirmation == 'y' || paymentConfirmation == 'Y') {
        printf("Processing Mobile Payment...\n");
        printf("Payment Successful! Thank you for using Mobile Payment.\n");
    } else {
        printf("Payment Cancelled.\n");
    }
}

void viewFavorites(FavoriteItem *favorites) {
    printf("Viewing favorites...\n");
    while (favorites != NULL) {
        printf("%s\n", favorites->name);
        favorites = favorites->next;
    }
}

void addFavorite(MenuItem *menu, FavoriteItem **favorites) {
    char itemName[100];
    printf("Enter the name of the item to add to favorites: ");
    scanf("%s", itemName);

    MenuItem *menuPtr = menu;
    while (menuPtr != NULL) {
        if (strcmp(menuPtr->name, itemName) == 0) {
            FavoriteItem *newFavorite = malloc(sizeof(FavoriteItem));
            newFavorite->name = strdup(itemName);
            newFavorite->next = *favorites;
            *favorites = newFavorite;
            printf("%s has been added to your favorites.\n", itemName);
            return;
        }
        menuPtr = menuPtr->next;
    }

    printf("Item not found in the menu!\n");
}

BSTNode* createBSTNode(char *name, double price) {
    BSTNode *newNode = (BSTNode*)malloc(sizeof(BSTNode));
    newNode->name = strdup(name);
    newNode->price = price;
    newNode->left = newNode->right = NULL;
    return newNode;
}

BSTNode* insertBSTNode(BSTNode *root, char *name, double price) {
    if (root == NULL)
        return createBSTNode(name, price);
    if (strcmp(name, root->name) < 0)
        root->left = insertBSTNode(root->left, name, price);
    else if (strcmp(name, root->name) > 0)
        root->right = insertBSTNode(root->right, name, price);
    return root;
}

BSTNode* searchBSTNode(BSTNode *root, const char *name) {
    if (root == NULL || strcmp(root->name, name) == 0)
        return root;
    if (strcmp(name, root->name) < 0)
        return searchBSTNode(root->left, name);
    return searchBSTNode(root->right, name);
}

BSTNode* convertMenuListToBST(MenuItem *menu) {
    BSTNode *root = NULL;
    while (menu != NULL) {
        root = insertBSTNode(root, menu->name, menu->price);
        menu = menu->next;
    }
    return root;
}
tes
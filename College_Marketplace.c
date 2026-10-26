/*
    DATA STRUCTURE MINI PROJECT
    Project Title: College Marketplace
    Language: C
    Students:
      1. Yash Jogani     - 12502120601126
      2. Patel Rahi      - 12502120601087
      3. Patel Hetvi     - 12502120601086

    Department: B.Tech AI & DS
    Semester: 3rd
    Faculty Guide: Rinkal Mam

    Data Structures Used:
      - Linked List : Students and marketplace services
      - Queue       : Pending orders (FIFO)
      - Stack       : Recently completed orders
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_TITLE 80
#define MAX_CAT 40
#define MAX_HISTORY 100

typedef struct Student {
    int id;
    char name[MAX_NAME];
    char department[20];
    struct Student *next;
} Student;

typedef struct Service {
    int serviceId;
    int sellerId;
    char sellerName[MAX_NAME];
    char title[MAX_TITLE];
    char category[MAX_CAT];
    float price;
    float rating;
    int ratingCount;
    struct Service *next;
} Service;

typedef struct Order {
    int orderId;
    int buyerId;
    char buyerName[MAX_NAME];
    int serviceId;
    char serviceTitle[MAX_TITLE];
    float amount;
    struct Order *next;
} Order;

typedef struct StackNode {
    int orderId;
    char serviceTitle[MAX_TITLE];
    struct StackNode *next;
} StackNode;

Student *studentHead = NULL;
Service *serviceHead = NULL;
Order *front = NULL, *rear = NULL;
StackNode *top = NULL;

int nextStudentId = 1001;
int nextServiceId = 2001;
int nextOrderId = 5001;

/* ---------- Utility ---------- */

void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void readLine(char *str, int size) {
    if (fgets(str, size, stdin)) {
        str[strcspn(str, "\n")] = '\0';
    }
}

Student* findStudent(int id) {
    Student *p = studentHead;
    while (p != NULL) {
        if (p->id == id) return p;
        p = p->next;
    }
    return NULL;
}

Service* findService(int id) {
    Service *p = serviceHead;
    while (p != NULL) {
        if (p->serviceId == id) return p;
        p = p->next;
    }
    return NULL;
}

/* ---------- Student Linked List ---------- */

void addStudent(void) {
    Student *newNode = (Student*)malloc(sizeof(Student));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->id = nextStudentId++;
    printf("Enter student name: ");
    clearInput();
    readLine(newNode->name, MAX_NAME);
    strcpy(newNode->department, "AI & DS");

    newNode->next = studentHead;
    studentHead = newNode;

    printf("Student registered successfully. Student ID: %d\n", newNode->id);
}

void displayStudents(void) {
    Student *p = studentHead;

    if (!p) {
        printf("No students registered.\n");
        return;
    }

    printf("\n%-8s %-25s %-12s\n", "ID", "Name", "Department");
    printf("------------------------------------------------\n");
    while (p) {
        printf("%-8d %-25s %-12s\n", p->id, p->name, p->department);
        p = p->next;
    }
}

/* ---------- Service Linked List ---------- */

void addService(void) {
    int sellerId;
    Student *seller;
    Service *newNode;

    printf("Enter seller/student ID: ");
    scanf("%d", &sellerId);
    seller = findStudent(sellerId);

    if (!seller) {
        printf("Student not found. Register the student first.\n");
        return;
    }

    newNode = (Service*)malloc(sizeof(Service));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->serviceId = nextServiceId++;
    newNode->sellerId = sellerId;
    strcpy(newNode->sellerName, seller->name);

    printf("Enter service/product title: ");
    clearInput();
    readLine(newNode->title, MAX_TITLE);

    printf("Enter category (Notes/Design/Tutoring/Books/Other): ");
    readLine(newNode->category, MAX_CAT);

    printf("Enter price: ");
    scanf("%f", &newNode->price);

    newNode->rating = 0.0f;
    newNode->ratingCount = 0;
    newNode->next = serviceHead;
    serviceHead = newNode;

    printf("Listing added successfully. Service ID: %d\n", newNode->serviceId);
}

void displayServices(void) {
    Service *p = serviceHead;

    if (!p) {
        printf("No services/products listed.\n");
        return;
    }

    printf("\n%-6s %-25s %-15s %-20s %-10s %-8s\n",
           "ID", "Title", "Category", "Seller", "Price", "Rating");
    printf("--------------------------------------------------------------------------------\n");

    while (p) {
        if (p->ratingCount == 0)
            printf("%-6d %-25s %-15s %-20s Rs.%-6.2f %-8s\n",
                   p->serviceId, p->title, p->category,
                   p->sellerName, p->price, "N/A");
        else
            printf("%-6d %-25s %-15s %-20s Rs.%-6.2f %.1f/5\n",
                   p->serviceId, p->title, p->category,
                   p->sellerName, p->price, p->rating);

        p = p->next;
    }
}

void searchService(void) {
    char keyword[MAX_TITLE];
    Service *p = serviceHead;
    int found = 0;

    printf("Enter title/category keyword: ");
    clearInput();
    readLine(keyword, MAX_TITLE);

    printf("\nSearch Results:\n");
    while (p) {
        if (strstr(p->title, keyword) || strstr(p->category, keyword)) {
            printf("ID: %d | %s | %s | Seller: %s | Rs. %.2f\n",
                   p->serviceId, p->title, p->category,
                   p->sellerName, p->price);
            found = 1;
        }
        p = p->next;
    }

    if (!found) printf("No matching listing found.\n");
}

/* ---------- Queue: Orders ---------- */

void enqueueOrder(void) {
    int buyerId, serviceId;
    Student *buyer;
    Service *service;
    Order *newOrder;

    printf("Enter buyer/student ID: ");
    scanf("%d", &buyerId);
    buyer = findStudent(buyerId);

    if (!buyer) {
        printf("Buyer not found. Register the student first.\n");
        return;
    }

    printf("Enter service ID to order: ");
    scanf("%d", &serviceId);
    service = findService(serviceId);

    if (!service) {
        printf("Service not found.\n");
        return;
    }

    if (buyerId == service->sellerId) {
        printf("A student cannot order their own listing.\n");
        return;
    }

    newOrder = (Order*)malloc(sizeof(Order));
    if (!newOrder) {
        printf("Memory allocation failed.\n");
        return;
    }

    newOrder->orderId = nextOrderId++;
    newOrder->buyerId = buyerId;
    strcpy(newOrder->buyerName, buyer->name);
    newOrder->serviceId = service->serviceId;
    strcpy(newOrder->serviceTitle, service->title);
    newOrder->amount = service->price;
    newOrder->next = NULL;

    if (rear == NULL) {
        front = rear = newOrder;
    } else {
        rear->next = newOrder;
        rear = newOrder;
    }

    printf("Order placed successfully. Order ID: %d\n", newOrder->orderId);
}

void displayPendingOrders(void) {
    Order *p = front;

    if (!p) {
        printf("No pending orders.\n");
        return;
    }

    printf("\nPending Orders (FIFO Queue):\n");
    while (p) {
        printf("Order %d | Buyer: %s | %s | Rs. %.2f\n",
               p->orderId, p->buyerName, p->serviceTitle, p->amount);
        p = p->next;
    }
}

void pushHistory(int orderId, const char *title) {
    StackNode *node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) return;

    node->orderId = orderId;
    strcpy(node->serviceTitle, title);
    node->next = top;
    top = node;
}

void processNextOrder(void) {
    Order *temp;

    if (!front) {
        printf("No pending order to process.\n");
        return;
    }

    temp = front;
    front = front->next;
    if (!front) rear = NULL;

    pushHistory(temp->orderId, temp->serviceTitle);

    printf("Processed Order %d successfully: %s | Amount: Rs. %.2f\n",
           temp->orderId, temp->serviceTitle, temp->amount);

    free(temp);
}

void displayHistory(void) {
    StackNode *p = top;

    if (!p) {
        printf("No completed orders.\n");
        return;
    }

    printf("\nRecently Completed Orders (Stack - LIFO):\n");
    while (p) {
        printf("Order %d | %s\n", p->orderId, p->serviceTitle);
        p = p->next;
    }
}

/* ---------- Rating ---------- */

void rateService(void) {
    int serviceId, rating;
    Service *service;

    printf("Enter service ID: ");
    scanf("%d", &serviceId);
    service = findService(serviceId);

    if (!service) {
        printf("Service not found.\n");
        return;
    }

    printf("Enter rating (1-5): ");
    scanf("%d", &rating);

    if (rating < 1 || rating > 5) {
        printf("Invalid rating.\n");
        return;
    }

    service->rating =
        ((service->rating * service->ratingCount) + rating) /
        (service->ratingCount + 1);

    service->ratingCount++;

    printf("Rating added successfully. New rating: %.1f/5\n",
           service->rating);
}

/* ---------- Memory Cleanup ---------- */

void freeAll(void) {
    Student *s;
    Service *v;
    Order *o;
    StackNode *h;

    while (studentHead) {
        s = studentHead;
        studentHead = studentHead->next;
        free(s);
    }

    while (serviceHead) {
        v = serviceHead;
        serviceHead = serviceHead->next;
        free(v);
    }

    while (front) {
        o = front;
        front = front->next;
        free(o);
    }

    while (top) {
        h = top;
        top = top->next;
        free(h);
    }
}

/* ---------- Menu ---------- */

void showMenu(void) {
    printf("\n===============================================\n");
    printf("          COLLEGE MARKETPLACE\n");
    printf("===============================================\n");
    printf("1. Register Student\n");
    printf("2. Display Students\n");
    printf("3. Add Service/Product\n");
    printf("4. Display Marketplace\n");
    printf("5. Search Service/Product\n");
    printf("6. Place Order\n");
    printf("7. Display Pending Orders\n");
    printf("8. Process Next Order\n");
    printf("9. Display Completed Order History\n");
    printf("10. Rate Service/Product\n");
    printf("0. Exit\n");
    printf("===============================================\n");
    printf("Enter choice: ");
}

int main(void) {
    int choice;

    printf("\n*** College Marketplace - Data Structure Mini Project ***\n");

    do {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: addService(); break;
            case 4: displayServices(); break;
            case 5: searchService(); break;
            case 6: enqueueOrder(); break;
            case 7: displayPendingOrders(); break;
            case 8: processNextOrder(); break;
            case 9: displayHistory(); break;
            case 10: rateService(); break;
            case 0:
                printf("Thank you for using College Marketplace!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    freeAll();
    return 0;
}

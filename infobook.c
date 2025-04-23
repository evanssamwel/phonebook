#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_NAME 50
#define MAX_PHONE 15
#define FILENAME "infobook.txt"

struct person {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
};

int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void delay(int milliseconds) {
    usleep(milliseconds * 1000);
}

void menu();
void addContact();
void viewContacts();
void searchContact();
void deleteContact();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while (1) {
        system("clear");
        printf("\n\n\t\t********** INFOBOOK **********\n");
        printf("\t\t1. Add New Contact\n");
        printf("\t\t2. View All Contacts\n");
        printf("\t\t3. Search Contact\n");
        printf("\t\t4. Delete Contact\n");
        printf("\t\t5. Exit\n");
        printf("\t\tEnter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5:
                printf("Exiting...");
                delay(1000);
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
                delay(1000);
        }
    }
}

void addContact() {
    FILE *fp;
    struct person p;

    system("clear");
    fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        printf("Unable to open file!\n");
        return;
    }

    printf("Enter Name: ");
    fgets(p.name, MAX_NAME, stdin);
    p.name[strcspn(p.name, "\n")] = 0;

    printf("Enter Phone Number: ");
    fgets(p.phone, MAX_PHONE, stdin);
    p.phone[strcspn(p.phone, "\n")] = 0;

    fwrite(&p, sizeof(p), 1, fp);
    fclose(fp);

    printf("Contact saved successfully!\n");
    delay(1000);
}

void viewContacts() {
    FILE *fp;
    struct person p;

    system("clear");
    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No contacts found!\n");
        return;
    }

    printf("\n\tContacts List:\n");
    printf("----------------------------\n");

    while (fread(&p, sizeof(p), 1, fp)) {
        printf("Name : %s\n", p.name);
        printf("Phone: %s\n", p.phone);
        printf("----------------------------\n");
    }
    fclose(fp);
    printf("Press any key to return to menu...");
    getch();
}

void searchContact() {
    FILE *fp;
    struct person p;
    char name[MAX_NAME];
    int found = 0;

    system("clear");
    printf("Enter name to search: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No contacts found!\n");
        return;
    }

    while (fread(&p, sizeof(p), 1, fp)) {
        if (strcasecmp(p.name, name) == 0) {
            printf("\nContact Found:\n");
            printf("Name : %s\n", p.name);
            printf("Phone: %s\n", p.phone);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nContact not found.\n");

    fclose(fp);
    printf("Press any key to return to menu...");
    getch();
}

void deleteContact() {
    FILE *fp, *temp;
    struct person p;
    char name[MAX_NAME];
    int found = 0;

    system("clear");
    printf("Enter name to delete: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("No contacts found!\n");
        return;
    }
    temp = fopen("temp.txt", "w");

    while (fread(&p, sizeof(p), 1, fp)) {
        if (strcasecmp(p.name, name) != 0) {
            fwrite(&p, sizeof(p), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("Contact deleted successfully.\n");
    else
        printf("Contact not found.\n");

    delay(1000);
}

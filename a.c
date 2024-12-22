#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char name[50];
    int hour;
    int minute;
    char date[11];      // Date in format YYYY-MM-DD
    char end_date[11];  // End date in format YYYY-MM-DD
} Medicine;

typedef struct {
    char username[50];
    char password[50];
} User;

// Function prototypes
void registerUser();
int loginUser();
void addMedicine();
void viewMedicines();
void viewHistory();
void checkReminders();
void updateReminder();
void deleteReminder();
void exitProgram();

const char *medicinesFilename = "./medicines.dat";
const char *usersFilename = "./users.dat";
const char *historyFilename = "./history.dat";

int main() {
    int choice;

    printf("..............WELCOME...............\n\n .........Smart Medicine Reminder..........\n");

    int loggedIn = 0;
    while (!loggedIn) {
        printf("1. Register\n");
        printf("2. Login\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loggedIn = loginUser();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    while (1) {
        printf("\nSmart Medicine Reminder\n");
        printf("1. Add medicine reminder\n");
        printf("2. View all reminders\n");
        printf("3. View history of deleted reminders\n");
        printf("4. Check reminders\n");
        printf("5. Update reminder\n");
        printf("6. Delete reminder\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addMedicine();
                break;
            case 2:
                viewMedicines();
                break;
            case 3:
                viewHistory();
                break;
            case 4:
                checkReminders();
                break;
            case 5:
                updateReminder();
                break;
            case 6:
                deleteReminder();
                break;
            case 7:
                exitProgram();
                return 0;
            default:
                printf("Wrong choice! Please try again.\n");
        }
    }

    return 0;
}

// Function to register a new user
void registerUser() {
    FILE *file = fopen(usersFilename, "ab");
    if (file == NULL) {
        perror("Error opening users file");
        return;
    }

    User user;
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);

    fwrite(&user, sizeof(User), 1, file);
    fclose(file);

    printf("Registration successful! Please log in.\n");
}

// Function to login a user
int loginUser() {
    FILE *file = fopen(usersFilename, "rb");
    if (file == NULL) {
        printf("No registered users found. Please register first.\n");
        return 0;
    }

    char username[50], password[50];
    User user;
    int found = 0;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Incorrect username or password! Please try again.\n");
        return 0;
    }
}

// Function to add a medicine reminder
void addMedicine() {
    FILE *file = fopen(medicinesFilename, "ab");
    if (file == NULL) {
        perror("Error opening medicines file");
        return;
    }

    Medicine med;
    printf("Enter medicine name: ");
    scanf("%s", med.name);
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", med.date);
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", med.end_date);
    printf("Enter reminder hour (24-hour format): ");
    scanf("%d", &med.hour);
    printf("Enter reminder minute: ");
    scanf("%d", &med.minute);

    fwrite(&med, sizeof(Medicine), 1, file);
    fclose(file);

    printf("Medicine reminder added successfully!\n");
}

// Function to view all medicine reminders
void viewMedicines() {
    FILE *file = fopen(medicinesFilename, "rb");
    if (file == NULL) {
        printf("No reminders found!\n");
        return;
    }

    Medicine med;
    printf("All Medicine Reminders\n");
    while (fread(&med, sizeof(Medicine), 1, file)) {
        printf("Medicine: %s, Start Date: %s, End Date: %s, Time: %02d:%02d\n", med.name, med.date, med.end_date, med.hour, med.minute);
    }

    fclose(file);
}

// Function to view history of deleted reminders
void viewHistory() {
    FILE *file = fopen(historyFilename, "rb");
    if (file == NULL) {
        printf("No history found!\n");
        return;
    }

    Medicine med;
    printf("Deleted Medicine Reminders\n");
    while (fread(&med, sizeof(Medicine), 1, file)) {
        printf("Medicine: %s, Date: %s, Time: %02d:%02d\n", med.name, med.date, med.hour, med.minute);
    }

    fclose(file);
}

// Function to check reminders for the current time
void checkReminders() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int currentHour = tm.tm_hour;
    int currentMinute = tm.tm_min;

    FILE *file = fopen(medicinesFilename, "rb");
    if (file == NULL) {
        printf("No reminders found!\n");
        return;
    }

    Medicine med;
    int found = 0;
    printf("Checking reminders for current time %02d:%02d\n", currentHour, currentMinute);
    while (fread(&med, sizeof(Medicine), 1, file)) {
        if (med.hour == currentHour && med.minute == currentMinute) {
            printf("Reminder: Take your medicine '%s' now!\n", med.name);
            found = 1;
        }
    }

    if (!found) {
        printf("No reminders for this time.\n");
    }

    fclose(file);
}

// Function to update a medicine reminder
void updateReminder() {
    FILE *file = fopen(medicinesFilename, "rb+");
    if (file == NULL) {
        printf("Error opening medicines file\n");
        return;
    }

    Medicine med;
    char medName[50];
    int found = 0;

    printf("Enter the name of the medicine to update: ");
    scanf("%s", medName);

    while (fread(&med, sizeof(Medicine), 1, file)) {
        if (strcmp(med.name, medName) == 0) {
            printf("Enter new end date (YYYY-MM-DD): ");
            scanf("%s", med.date);
            printf("Enter new hour (24-hour format): ");
            scanf("%d", &med.hour);
            printf("Enter new minute: ");
            scanf("%d", &med.minute);
            fseek(file, -sizeof(Medicine), SEEK_CUR);
            fwrite(&med, sizeof(Medicine), 1, file);
            found = 1;
            printf("Reminder updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("No reminder found with that name!\n");
    }

    fclose(file);
}

// Function to delete a medicine reminder and save it to history
void deleteReminder() {
    FILE *file = fopen(medicinesFilename, "rb");
    if (file == NULL) {
        printf("No reminders found!\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    FILE *historyFile = fopen(historyFilename, "ab");
    if (tempFile == NULL || historyFile == NULL) {
        perror("Error creating temporary or history file");
        fclose(file);
        return;
    }

    Medicine med;
    char medName[50];
    int found = 0;

    printf("Enter the name of the medicine to delete: ");
    scanf("%s", medName);

    while (fread(&med, sizeof(Medicine), 1, file)) {
        if (strcmp(med.name, medName) != 0) {
            fwrite(&med, sizeof(Medicine), 1, tempFile);
        } else {
            fwrite(&med, sizeof(Medicine), 1, historyFile);  // Save to history
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);
    fclose(historyFile);

    if (found) {
        remove(medicinesFilename);
        rename("temp.dat", medicinesFilename);  // Replace the original file with the temp file
        printf("Reminder deleted successfully!\n");
    } else {
        remove("temp.dat");
        printf("No reminder found with that name!\n");
    }
}

// Function to exit the program
void exitProgram() {
    printf("Thank you for using Smart Medicine Reminder. Goodbye!\n");
} 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#define MAX_SEATS 1000
#define USER_FILE "users.bin"
#define SEAT_FILE "seats.bin"
#define COMPLAINT_FILE "complaints.txt"

struct user {
    int id;
    char name[50];
    char mobile_number[20];
    char username[50];
    char password[15];
    char role[10]; 
};

typedef struct {
    int seatID;
    int isReserved;
    char studentName[100]; 
} Seat;

Seat seats[MAX_SEATS];

void main_menu();
void admin_interface(char* admin_username);
void student_interface(char* student_username);
void visitor_interface();
void login(const char* role);
void registration();
void initializeSeats();
void loadSeats();
void saveSeats();
void seatManagement(char* student_username);
void reserveSeat(char* student_username);
void renewSeat(char* student_username);
void cancelSeat(char* student_username);
void submitComplaint(char* student_username);
void viewComplaints();
void view_rules();
void view_payment();
void view_hostel_info();
void view_student_info(char* student_username);


int main() {
    loadSeats();
    main_menu(); 
    return 0;
}

void main_menu() {
    int choice;
    while (1) {
        system(CLEAR_SCREEN);
        printf("\n******************************************************\n");
        printf("******************************************************\n");
        printf("\n");
        printf("** Welcome To DIU Hostel Management System   **\n");
        printf("\n");
        printf("******************************************************\n");
        printf("******************************************************\n\n");

        printf(" *** press 1 for Administrator   ***\n");
        printf(" *** press 2 for Student         ***\n");
        printf(" *** press 3 for Visitor         ***\n");
        printf(" *** press 0 for Exit            ***\n");
        printf("******************************************************\n");
        printf("\nEnter your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                login("admin");
                break;
            case 2:
                student_interface(NULL); 
                break;
            case 3:
                visitor_interface();
                break;
            case 0:
                printf("\nThank you for using the system. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please try again.\n");
                getchar(); 
                getchar(); 
        }
    }
}

void student_interface(char* logged_in_user) {
    int choice;

    if (logged_in_user == NULL) {
        system(CLEAR_SCREEN);
        printf("\nWelcome Student\n");
        printf("Press '1' for Registration\n");
        printf("Press '2' for Login\n");
        printf("Press '0' to return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registration();
                break;
            case 2:
                login("student");
                break;
            case 0: return;
            default: printf("Invalid choice.\n"); getchar(); getchar();
        }
        return;
    }

   
    while (1) {
        system(CLEAR_SCREEN);
        printf("\n******************************************************\n");
        printf("                  STUDENT DASHBOARD                  \n");
        printf("******************************************************\n");
        printf("   Welcome, %s!\n\n", logged_in_user);
        printf("   press 1 for Own Information\n");
        printf("   press 2 for Hostel Rule Regulation\n");
        printf("   press 3 for Food Menu\n");
        printf("   press 4 for Payment Menu\n");
        printf("   press 5 for Seat Management Menu\n");
        printf("   press 6 for Complaints\n");
        printf("   press 7 for Logout\n");
        printf("******************************************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: view_student_info(logged_in_user); break;
            case 2: view_rules(); break;
            case 3:
                system(CLEAR_SCREEN);
                printf("\n--- Food Menu ---\n");
                printf("Daily Meal Schedule is updated on the notice board.\n");
                printf("\nPress any key to continue...");
                getchar(); getchar();
                break;
            case 4: view_payment(); break;
            case 5: seatManagement(logged_in_user); break;
            case 6: submitComplaint(logged_in_user); break;
            case 7: printf("\nLogging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    }
}

void admin_interface(char* admin_username) {
    int choice;
     while (1) {
        system(CLEAR_SCREEN);
        printf("\n******************************************************\n");
        printf("                   ADMIN DASHBOARD                   \n");
        printf("******************************************************\n");
        printf("   Welcome, %s!\n\n", admin_username);
        printf("   press 1 to View All Seat Status\n");
        printf("   press 2 to View Complaints\n");
        printf("   press 3 to View All Student Records\n");
        printf("   press 4 to Logout\n");
        printf("******************************************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system(CLEAR_SCREEN);
                printf("\n--- All Hostel Seats ---\n");
                printf("%-10s | %-12s | %-20s\n", "Seat ID", "Status", "Reserved By (Username)");
                printf("--------------------------------------------------\n");
                for (int i = 0; i < MAX_SEATS; i++) {
                    if (seats[i].isReserved) {
                        printf("%-10d | %-12s | %-20s\n", seats[i].seatID, "Reserved", seats[i].studentName);
                    }
                }
                printf("\nPress any key to continue...");
                getchar(); getchar();
                break;
            case 2: viewComplaints(); break;
            case 3:
                system(CLEAR_SCREEN);
                FILE* fp = fopen(USER_FILE, "rb");
                struct user u_read;
                if(fp == NULL){
                    printf("No student records found.\n");
                } else {
                     printf("\n--- All Student Records ---\n");
                     printf("%-15s | %-20s | %-15s | %-15s\n", "ID", "Name", "Mobile", "Username");
                     printf("----------------------------------------------------------------\n");
                     while(fread(&u_read, sizeof(struct user), 1, fp)){
                         if(strcmp(u_read.role, "student") == 0){
                            printf("%-15d | %-20s | %-15s | %-15s\n", u_read.id, u_read.name, u_read.mobile_number, u_read.username);
                         }
                     }
                     fclose(fp);
                }
                printf("\nPress any key to continue...");
                getchar(); getchar();
                break;
            case 4: printf("\nLogging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    }
}

void visitor_interface() {
    int choice;
    while(1) {
        system(CLEAR_SCREEN);
        printf("\n******************************************************\n");
        printf("                    VISITOR MENU                     \n");
        printf("******************************************************\n");
        printf("   press 1 to View Hostel Information\n");
        printf("   press 2 to View Hostel Rules\n");
        printf("   press 0 to return to Main Menu\n");
        printf("******************************************************\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: view_hostel_info(); break;
            case 2: view_rules(); break;
            case 0: return;
            default: printf("Invalid choice. Please try again.\n"); getchar(); getchar();
        }
    }
}

void registration() {
    struct user u;
    FILE *fp = fopen(USER_FILE, "ab+");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    system(CLEAR_SCREEN);
    printf("\nWelcome to DIU Hostel Management System. You need to enter some details for Registration.\n");

    printf("Enter the id: ");
    scanf("%d", &u.id);

    rewind(fp);
    struct user u_read;
    while(fread(&u_read, sizeof(struct user), 1, fp)){
        if(u_read.id == u.id){
            printf("Error: A user with this ID already exists.\n");
            fclose(fp);
            getchar(); getchar();
            return;
        }
    }

    printf("Enter your name: ");
    scanf(" %[^\n]s", u.name); // Read full name with spaces
    printf("Enter your mobile number: ");
    scanf("%s", u.mobile_number);
    printf("Now choose appropriate Username & Password for this system\n");
    printf("Enter Username: ");
    scanf("%s", u.username);
    printf("Enter Password: ");
    scanf("%s", u.password);
    strcpy(u.role, "student"); 

    fseek(fp, 0, SEEK_END); 
    fwrite(&u, sizeof(struct user), 1, fp);
    fclose(fp);

    printf("\nRegistration Successful!\n");
    printf("Press any Key to Continue...");
    getchar(); getchar();
}

void login(const char* role) {
    char username[50], password[15];
    FILE *fp;
    struct user u;
    int found = 0;

    system(CLEAR_SCREEN);
    printf("\nPlease Enter your Login credentials below\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(role, "admin") == 0 && strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        found = 1;
        strcpy(u.username, "admin");
    } else {
        fp = fopen(USER_FILE, "rb");
        if (fp != NULL) {
            while (fread(&u, sizeof(struct user), 1, fp)) {
                if (strcmp(u.username, username) == 0 && strcmp(u.password, password) == 0 && strcmp(u.role, role) == 0) {
                    found = 1;
                    break;
                }
            }
            fclose(fp);
        }
    }

    if (found) {
        printf("\nSuccessful Login\n");
        printf("Press any key to continue...");
        getchar(); getchar();
        if (strcmp(role, "student") == 0) {
            student_interface(u.username);
        } else if (strcmp(role, "admin") == 0) {
            admin_interface(u.username);
        }
    } else {
        printf("\nLogin Failed! Invalid credentials or role.\n");
        printf("Press any key to continue...");
        getchar(); getchar();
    }
}

void view_rules() {
    system(CLEAR_SCREEN);
    printf("\n--- Rules and Regulations ---\n\n");
    printf("1. You have to pay hostel charge every month within first 10 days.\n");
    printf("   As a late fee, after 10 day you have to pay 20tk for each day & 200tk for each month.\n");
    printf("2. You have to enter in hostel in due time.\n");
    printf("   Summer: last time 7.00pm\n");
    printf("   Winter: last time 6.30pm\n");
    printf("3. Before 6 month you cant leave the hostel.\n");
    printf("4. If you want to leave this hostel you have to inform the hostel\n");
    printf("   authority before 2 months.\n");
    printf("5. You can't use electrical applications like Water Kettle, Iron, Induction Stove & Cooker.\n");
    printf("6. Only your two local guardian can come to meet with you\n");
    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void view_payment() {
    system(CLEAR_SCREEN);
    printf("\n--- Payment Status ---\n");
    printf("Enter paid amount: 30789\n");
    printf("Payable Amount: 21000.00\n");
    printf("Paid Amount: 30789.00\n");
    printf("Due Amount: -9789.00\n");
    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void view_hostel_info() {
    system(CLEAR_SCREEN);
    printf("\n--- Hostel Information ---\n\n");
    printf("DIU offers well-maintained hostels with a variety of room options.\n");
    printf("Our facilities are secure and provide a conducive environment for learning.\n\n");
    printf("Room Types & Pricing:\n");
    printf("- 4-person room: BDT 5,000 / month\n");
    printf("- 2-person room: BDT 8,000 / month\n");
    printf("- Single room: BDT 12,000 / month\n\n");
    printf("Services include: 24/7 security, Wi-Fi, cleaning services, and meal options.\n");
    printf("Location: Close to the main campus for easy access.\n");
    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void seatManagement(char* student_username) {
    int choice;
    while(1) {
        system(CLEAR_SCREEN);
        printf("\n--- Seat Reservation Management ---\n");
        printf("1. Reserve a New Seat\n");
        printf("2. Renew Seat Reservation (Current Student)\n");
        printf("3. Cancel Seat Reservation\n");
        printf("4. View Available Seats\n");
        printf("5. Exit Seat Management\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: reserveSeat(student_username); break;
            case 2: renewSeat(student_username); break;
            case 3: cancelSeat(student_username); break;
            case 4:
                system(CLEAR_SCREEN);
                printf("\n--- Available Seats ---\n");
                int available_count = 0;
                 for (int i = 0; i < MAX_SEATS; i++) {
                    if (!seats[i].isReserved) {
                        printf("Seat ID: %d\n", seats[i].seatID);
                        available_count++;
                    }
                }
                if (available_count == 0) printf("Sorry, no seats are currently available.\n");
                printf("\nPress any key to continue...");
                getchar(); getchar();
                break;
            case 5: return;
            default: printf("Invalid Choice.\n");
        }
    }
}

void reserveSeat(char* student_username) {
    int seat_id;
    printf("\nEnter Seat ID (1-%d): ", MAX_SEATS);
    scanf("%d", &seat_id);

    if (seat_id < 1 || seat_id > MAX_SEATS) {
        printf("Invalid Seat ID.\n");
    } else if (seats[seat_id - 1].isReserved) {
        printf("Sorry, Seat %d is already reserved by %s.\n", seat_id, seats[seat_id - 1].studentName);
    } else {
        seats[seat_id - 1].isReserved = 1;
        strcpy(seats[seat_id - 1].studentName, student_username);
        saveSeats();
        printf("Seat %d reserved successfully for %s. Thank You\n", seat_id, student_username);
    }
    printf("Press any key to continue...");
    getchar(); getchar();
}

void renewSeat(char* student_username) {
    int seat_id = -1;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seats[i].isReserved && strcmp(seats[i].studentName, student_username) == 0) {
            seat_id = seats[i].seatID;
            break;
        }
    }

    if (seat_id != -1) {
        printf("\nYour current reservation for Seat %d has been renewed successfully.\n", seat_id);
    } else {
        printf("\nYou do not have an active seat reservation to renew.\n");
    }
     printf("Press any key to continue...");
     getchar(); getchar();
}

void cancelSeat(char* student_username) {
     int seat_id = -1, index = -1;
    for (int i = 0; i < MAX_SEATS; i++) {
        if (seats[i].isReserved && strcmp(seats[i].studentName, student_username) == 0) {
            seat_id = seats[i].seatID;
            index = i;
            break;
        }
    }

    if (index != -1) {
        seats[index].isReserved = 0;
        strcpy(seats[index].studentName, "");
        saveSeats();
        printf("\nReservation for Seat %d has been successfully cancelled.\n", seat_id);
    } else {
        printf("\nNo active seat reservation found for your account.\n");
    }
    printf("Press any key to continue...");
    getchar(); getchar();
}

void submitComplaint(char* student_username) {
    char complaint[500];
    FILE *fp = fopen(COMPLAINT_FILE, "a");
    if (fp == NULL) {
        printf("Error: Could not open complaints file.\n");
        return;
    }
    system(CLEAR_SCREEN);
    printf("\n--- Submit a Complaint ---\n");
    printf("Please enter your complaint below:\n");
    getchar(); // Consume the newline character
    fgets(complaint, sizeof(complaint), stdin);
    fprintf(fp, "User: %s\nComplaint: %s\n--------------------\n", student_username, complaint);
    fclose(fp);
    printf("\nYour complaint has been submitted successfully.\n");
    printf("Press any key to continue...");
    getchar();
}

void viewComplaints() {
    char ch;
    FILE *fp = fopen(COMPLAINT_FILE, "r");
    system(CLEAR_SCREEN);
    printf("\n--- All Submitted Complaints ---\n\n");
    if (fp == NULL) {
        printf("No complaints have been submitted yet.\n");
    } else {
        while ((ch = fgetc(fp)) != EOF) { putchar(ch); }
        fclose(fp);
    }
    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void view_student_info(char* student_username) {
    FILE* fp = fopen(USER_FILE, "rb");
    struct user u;
    int found = 0;

    if (fp == NULL) { printf("User file not found.\n"); return; }

    system(CLEAR_SCREEN);
    printf("\n--- Your Information ---\n");
    while(fread(&u, sizeof(struct user), 1, fp)) {
        if(strcmp(u.username, student_username) == 0) {
            printf("* ID         : %d\n", u.id);
            printf("* Name       : %s\n", u.name);
            printf("* Mobile no  : %s\n", u.mobile_number);
            printf("* Username   : %s\n", u.username);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) printf("Could not find your information.\n");

    printf("\nPress any key to continue...");
    getchar(); getchar();
}

void initializeSeats() {
    for (int i = 0; i < MAX_SEATS; i++) {
        seats[i].seatID = i + 1;
        seats[i].isReserved = 0;
        strcpy(seats[i].studentName, "");
    }
    saveSeats();
}

void loadSeats() {
    FILE *fp = fopen(SEAT_FILE, "rb");
    if (fp == NULL) {
        initializeSeats();
    } else {
        fread(seats, sizeof(Seat), MAX_SEATS, fp);
        fclose(fp);
    }
}

void saveSeats() {
    FILE *fp = fopen(SEAT_FILE, "wb");
    if (fp == NULL) {
        printf("Error saving seat data!\n");
        return;
    }
    fwrite(seats, sizeof(Seat), MAX_SEATS, fp);
    fclose(fp);
}

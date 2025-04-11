#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_NAME_LENGTH 100
#define MAX_CONTACT_LENGTH 15
#define MAX_SOURCE_DEST_LENGTH 50
#define MAX_BUS_COUNT 20
#define MAX_SEATS 40
#define FILENAME_USERS "users.dat"
#define FILENAME_BUSES "buses.dat"
#define FILENAME_BOOKINGS "bookings.dat"
#define FILENAME_TEMP "temp.dat"

// Structures
typedef struct {
    int id;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char name[MAX_NAME_LENGTH];
    char contact[MAX_CONTACT_LENGTH];
} User;

typedef struct {
    int id;
    char source[MAX_SOURCE_DEST_LENGTH];
    char destination[MAX_SOURCE_DEST_LENGTH];
    char departureTime[20];
    char arrivalTime[20];
    float fare;
    int totalSeats;
    int availableSeats;
    int seatMap[MAX_SEATS];  
} Bus;

typedef struct {
    int id;
    int userId;
    int busId;
    int seatNumber;
    char bookingDate[20];
    char journeyDate[20];
    float fare;
    bool cancelled;
} Booking;

// Function prototypes
void clearScreen();
void displayMainMenu();
void displayAdminMenu();
void displayUserMenu();
int registerUser();
int loginUser(char*, char*);
void addBus();
void viewBuses();
void modifyBus();
void deleteBus();
void bookTicket(int);
void viewBookings(int);
void cancelBooking(int);
void generateTicket(Booking);
int generateId(const char*);
void displayBusDetails(Bus);
void displayBookingDetails(Booking, Bus, User);
void initializeFiles();
Bus getBusById(int);
User getUserById(int);
void displayAvailableSeats(Bus);
void saveBooking(Booking);
void updateBusSeats(int, int, int);

int currentUserId = -1;
bool isAdmin = false;

int main() {
    int choice;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    
    initializeFiles();
    
    while (1) {
        if (currentUserId == -1) {
            displayMainMenu();
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); 
            
            switch (choice) {
                case 1:
                    printf("Enter username: ");
                    fgets(username, MAX_USERNAME_LENGTH, stdin);
                    username[strcspn(username, "\n")] = 0;
                    
                    printf("Enter password: ");
                    fgets(password, MAX_PASSWORD_LENGTH, stdin);
                    password[strcspn(password, "\n")] = 0;
                    
                    currentUserId = loginUser(username, password);
                    if (currentUserId == -1) {
                        printf("Invalid credentials. Please try again.\n");
                    } else if (currentUserId == 0) {
                        isAdmin = true;
                        printf("Admin login successful!\n");
                    } else {
                        printf("Login successful!\n");
                    }
                    break;
                    
                case 2:
                    if (registerUser() == 1) {
                        printf("Registration successful! Please login to continue.\n");
                    } else {
                        printf("Registration failed. Please try again.\n");
                    }
                    break;
                    
                case 3:
                    printf("Thank you for using Bus Reservation System!\n");
                    exit(0);
                    
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } else if (isAdmin) {
            displayAdminMenu();
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();
            
            switch (choice) {
                case 1:
                    addBus();
                    break;
                    
                case 2:
                    viewBuses();
                    break;
                    
                case 3:
                    modifyBus();
                    break;
                    
                case 4:
                    deleteBus();
                    break;
                    
                case 5:
                    currentUserId = -1;
                    isAdmin = false;
                    printf("Logged out successfully!\n");
                    break;
                    
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } else {
            displayUserMenu();
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar(); 
            
            switch (choice) {
                case 1:
                    bookTicket(currentUserId);
                    break;
                    
                case 2:
                    viewBookings(currentUserId);
                    break;
                    
                case 3:
                    cancelBooking(currentUserId);
                    break;
                    
                case 4:
                    viewBuses();
                    break;
                    
                case 5:
                    currentUserId = -1;
                    printf("Logged out successfully!\n");
                    break;
                    
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        }
        
        printf("\nPress Enter to continue...");
        getchar();
        clearScreen();
    }
    
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    printf("\n===== BUS RESERVATION SYSTEM =====\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
}

void displayAdminMenu() {
    printf("\n===== ADMIN MENU =====\n");
    printf("1. Add Bus\n");
    printf("2. View Buses\n");
    printf("3. Modify Bus\n");
    printf("4. Delete Bus\n");
    printf("5. Logout\n");
}

void displayUserMenu() {
    printf("\n===== USER MENU =====\n");
    printf("1. Book Ticket\n");
    printf("2. View Bookings\n");
    printf("3. Cancel Booking\n");
    printf("4. View Available Buses\n");
    printf("5. Logout\n");
}

int registerUser() {
    User user;
    FILE *file;
    
    printf("\n===== REGISTER =====\n");
    
    user.id = generateId(FILENAME_USERS);
    
    printf("Enter username: ");
    fgets(user.username, MAX_USERNAME_LENGTH, stdin);
    user.username[strcspn(user.username, "\n")] = 0;
    
    // Check if username already exists
    FILE *checkFile = fopen(FILENAME_USERS, "rb");
    if (checkFile != NULL) {
        User tempUser;
        while (fread(&tempUser, sizeof(User), 1, checkFile)) {
            if (strcmp(tempUser.username, user.username) == 0) {
                printf("Username already exists!\n");
                fclose(checkFile);
                return 0;
            }
        }
        fclose(checkFile);
    }
    
    printf("Enter password: ");
    fgets(user.password, MAX_PASSWORD_LENGTH, stdin);
    user.password[strcspn(user.password, "\n")] = 0;
    
    printf("Enter full name: ");
    fgets(user.name, MAX_NAME_LENGTH, stdin);
    user.name[strcspn(user.name, "\n")] = 0;
    
    printf("Enter contact number: ");
    fgets(user.contact, MAX_CONTACT_LENGTH, stdin);
    user.contact[strcspn(user.contact, "\n")] = 0;
    
    file = fopen(FILENAME_USERS, "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
    
    return 1;
}

int loginUser(char *username, char *password) {
    FILE *file;
    User user;
    
    // Check for admin login
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        return 0; // Admin ID is 0
    }
    
    file = fopen(FILENAME_USERS, "rb");
    if (file == NULL) {
        printf("No users registered yet!\n");
        return -1;
    }
    
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            fclose(file);
            return user.id;
        }
    }
    
    fclose(file);
    return -1;
}

void addBus() {
    Bus bus;
    FILE *file;
    
    printf("\n===== ADD BUS =====\n");
    
    bus.id = generateId(FILENAME_BUSES);
    
    printf("Enter source: ");
    fgets(bus.source, MAX_SOURCE_DEST_LENGTH, stdin);
    bus.source[strcspn(bus.source, "\n")] = 0;
    
    printf("Enter destination: ");
    fgets(bus.destination, MAX_SOURCE_DEST_LENGTH, stdin);
    bus.destination[strcspn(bus.destination, "\n")] = 0;
    
    printf("Enter departure time (HH:MM): ");
    fgets(bus.departureTime, 20, stdin);
    bus.departureTime[strcspn(bus.departureTime, "\n")] = 0;
    
    printf("Enter arrival time (HH:MM): ");
    fgets(bus.arrivalTime, 20, stdin);
    bus.arrivalTime[strcspn(bus.arrivalTime, "\n")] = 0;
    
    printf("Enter fare: ");
    scanf("%f", &bus.fare);
    getchar();
    
    printf("Enter total seats (max %d): ", MAX_SEATS);
    scanf("%d", &bus.totalSeats);
    getchar();
    
    if (bus.totalSeats > MAX_SEATS) {
        bus.totalSeats = MAX_SEATS;
    }
    
    bus.availableSeats = bus.totalSeats;
    
    // Initialize seat map
    for (int i = 0; i < MAX_SEATS; i++) {
        bus.seatMap[i] = 0; // 0 means available
    }
    
    file = fopen(FILENAME_BUSES, "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    fwrite(&bus, sizeof(Bus), 1, file);
    fclose(file);
    
    printf("Bus added successfully!\n");
}

void viewBuses() {
    FILE *file;
    Bus bus;
    int count = 0;
    
    printf("\n===== AVAILABLE BUSES =====\n");
    printf("ID\tSource\t\tDestination\t\tDeparture\tArrival\t\tFare\tAvailable Seats\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    
    file = fopen(FILENAME_BUSES, "rb");
    if (file == NULL) {
        printf("No buses available!\n");
        return;
    }
    
    while (fread(&bus, sizeof(Bus), 1, file)) {
        printf("%d\t%-15s\t%-15s\t%-10s\t%-10s\t%.2f\t%d/%d\n", 
               bus.id, bus.source, bus.destination, bus.departureTime, 
               bus.arrivalTime, bus.fare, bus.availableSeats, bus.totalSeats);
        count++;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No buses available!\n");
    }
}

void modifyBus() {
    FILE *file, *tempFile;
    Bus bus;
    int id, found = 0;
    
    printf("\n===== MODIFY BUS =====\n");
    viewBuses();
    
    printf("Enter bus ID to modify: ");
    scanf("%d", &id);
    getchar();
    
    file = fopen(FILENAME_BUSES, "rb");
    if (file == NULL) {
        printf("No buses available!\n");
        return;
    }
    
    tempFile = fopen(FILENAME_TEMP, "wb");
    if (tempFile == NULL) {
        printf("Error opening file!\n");
        fclose(file);
        return;
    }
    
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.id == id) {
            displayBusDetails(bus);
            
            printf("Enter new source (leave empty to keep current): ");
            char temp[MAX_SOURCE_DEST_LENGTH];
            fgets(temp, MAX_SOURCE_DEST_LENGTH, stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(bus.source, temp);
            }
            
            printf("Enter new destination (leave empty to keep current): ");
            fgets(temp, MAX_SOURCE_DEST_LENGTH, stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(bus.destination, temp);
            }
            
            printf("Enter new departure time (leave empty to keep current): ");
            fgets(temp, 20, stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(bus.departureTime, temp);
            }
            
            printf("Enter new arrival time (leave empty to keep current): ");
            fgets(temp, 20, stdin);
            temp[strcspn(temp, "\n")] = 0;
            if (strlen(temp) > 0) {
                strcpy(bus.arrivalTime, temp);
            }
            
            printf("Enter new fare (0 to keep current): ");
            float newFare;
            scanf("%f", &newFare);
            getchar();
            if (newFare > 0) {
                bus.fare = newFare;
            }
            
            found = 1;
        }
        
        fwrite(&bus, sizeof(Bus), 1, tempFile);
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove(FILENAME_BUSES);
        rename(FILENAME_TEMP, FILENAME_BUSES);
        printf("Bus details modified successfully!\n");
    } else {
        remove(FILENAME_TEMP);
        printf("Bus with ID %d not found!\n", id);
    }
}

void deleteBus() {
    FILE *file, *tempFile;
    Bus bus;
    int id, found = 0;
    
    printf("\n===== DELETE BUS =====\n");
    viewBuses();
    
    printf("Enter bus ID to delete: ");
    scanf("%d", &id);
    getchar();
    
    file = fopen(FILENAME_BUSES, "rb");
    if (file == NULL) {
        printf("No buses available!\n");
        return;
    }
    
    tempFile = fopen(FILENAME_TEMP, "wb");
    if (tempFile == NULL) {
        printf("Error opening file!\n");
        fclose(file);
        return;
    }
    
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.id == id) {
            found = 1;
        } else {
            fwrite(&bus, sizeof(Bus), 1, tempFile);
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        remove(FILENAME_BUSES);
        rename(FILENAME_TEMP, FILENAME_BUSES);
        printf("Bus deleted successfully!\n");
    } else {
        remove(FILENAME_TEMP);
        printf("Bus with ID %d not found!\n", id);
    }
}

void bookTicket(int userId) {
    FILE *file;
    Bus bus;
    Booking booking;
    int busId, seatNumber, found = 0;
    
    printf("\n===== BOOK TICKET =====\n");
    viewBuses();
    
    printf("Enter bus ID: ");
    scanf("%d", &busId);
    getchar();
    
    file = fopen(FILENAME_BUSES, "rb");
    if (file == NULL) {
        printf("No buses available!\n");
        return;
    }
    
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.id == busId) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("Bus with ID %d not found!\n", busId);
        return;
    }
    
    if (bus.availableSeats == 0) {
        printf("Sorry, no seats available on this bus!\n");
        return;
    }
    
    // Display available seats
    displayAvailableSeats(bus);
    
    printf("Enter seat number: ");
    scanf("%d", &seatNumber);
    getchar();
    
    if (seatNumber < 1 || seatNumber > bus.totalSeats) {
        printf("Invalid seat number!\n");
        return;
    }
    
    if (bus.seatMap[seatNumber - 1] == 1) {
        printf("Seat already booked!\n");
        return;
    }
    
    // Prepare booking data
    booking.id = generateId(FILENAME_BOOKINGS);
    booking.userId = userId;
    booking.busId = busId;
    booking.seatNumber = seatNumber;
    booking.fare = bus.fare;
    booking.cancelled = false;
    
    // Get current date for booking date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(booking.bookingDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    printf("Enter journey date (DD/MM/YYYY): ");
    fgets(booking.journeyDate, 20, stdin);
    booking.journeyDate[strcspn(booking.journeyDate, "\n")] = 0;
    
    // Save booking
    saveBooking(booking);
    
    // Update bus seat availability
    updateBusSeats(busId, seatNumber, 1); // 1 for booked
    
    printf("Ticket booked successfully!\n");
    generateTicket(booking);
}

void viewBookings(int userId) {
    FILE *file;
    Booking booking;
    Bus bus;
    User user;
    int count = 0;
    
    printf("\n===== YOUR BOOKINGS =====\n");
    
    file = fopen(FILENAME_BOOKINGS, "rb");
    if (file == NULL) {
        printf("No bookings found!\n");
        return;
    }
    
    while (fread(&booking, sizeof(Booking), 1, file)) {
        if (booking.userId == userId && !booking.cancelled) {
            bus = getBusById(booking.busId);
            user = getUserById(booking.userId);
            
            displayBookingDetails(booking, bus, user);
            count++;
        }
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No bookings found!\n");
    }
}

void cancelBooking(int userId) {
    FILE *file, *tempFile;
    Booking booking;
    int bookingId, found = 0;
    
    printf("\n===== CANCEL BOOKING =====\n");
    
    // Display all active bookings
    file = fopen(FILENAME_BOOKINGS, "rb");
    if (file == NULL) {
        printf("No bookings found!\n");
        return;
    }
    
    printf("Your active bookings:\n");
    printf("ID\tBus ID\tSeat\tJourney Date\tFare\n");
    printf("------------------------------------------------\n");
    
    while (fread(&booking, sizeof(Booking), 1, file)) {
        if (booking.userId == userId && !booking.cancelled) {
            printf("%d\t%d\t%d\t%s\t%.2f\n", 
                   booking.id, booking.busId, booking.seatNumber, 
                   booking.journeyDate, booking.fare);
            found = 1;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("No active bookings found!\n");
        return;
    }
    
    printf("Enter booking ID to cancel: ");
    scanf("%d", &bookingId);
    getchar();
    
    file = fopen(FILENAME_BOOKINGS, "rb");
    tempFile = fopen(FILENAME_TEMP, "wb");
    
    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    found = 0;
    while (fread(&booking, sizeof(Booking), 1, file)) {
        if (booking.id == bookingId && booking.userId == userId && !booking.cancelled) {
            booking.cancelled = true;
            found = 1;
            
            // Free up the seat
            updateBusSeats(booking.busId, booking.seatNumber, 0); // 0 for available
        }
        
        fwrite(&booking, sizeof(Booking), 1, tempFile);
    }
    
    fclose(file);
    fclose(tempFile);
    
    remove(FILENAME_BOOKINGS);
    rename(FILENAME_TEMP, FILENAME_BOOKINGS);
    
    if (found) {
        printf("Booking cancelled successfully!\n");
    } else {
        printf("Booking with ID %d not found or already cancelled!\n", bookingId);
    }
}

void generateTicket(Booking booking) {
    Bus bus = getBusById(booking.busId);
    User user = getUserById(booking.userId);
    
    printf("\n========================================\n");
    printf("              BUS TICKET                \n");
    printf("========================================\n");
    printf("Booking ID: %d\n", booking.id);
    printf("Passenger Name: %s\n", user.name);
    printf("Contact: %s\n", user.contact);
    printf("----------------------------------------\n");
    printf("Bus Details: %d\n", bus.id);
    printf("From: %s\n", bus.source);
    printf("To: %s\n", bus.destination);
    printf("Departure: %s\n", bus.departureTime);
    printf("Arrival: %s\n", bus.arrivalTime);
    printf("----------------------------------------\n");
    printf("Journey Date: %s\n", booking.journeyDate);
    printf("Seat Number: %d\n", booking.seatNumber);
    printf("Fare: $%.2f\n", booking.fare);
    printf("Booking Date: %s\n", booking.bookingDate);
    printf("========================================\n");
    printf("     Thank you for choosing us!        \n");
    printf("========================================\n");
}

int generateId(const char *filename) {
    FILE *file = fopen(filename, "rb");
    
    if (file == NULL) {
        return 1; // First ID
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fclose(file);
    
    if (filename == FILENAME_USERS) {
        return (fileSize / sizeof(User)) + 1;
    } else if (filename == FILENAME_BUSES) {
        return (fileSize / sizeof(Bus)) + 1;
    } else if (filename == FILENAME_BOOKINGS) {
        return (fileSize / sizeof(Booking)) + 1;
    }
    
    return 1;
}

void displayBusDetails(Bus bus) {
    printf("\nBus Details:\n");
    printf("ID: %d\n", bus.id);
    printf("Route: %s to %s\n", bus.source, bus.destination);
    printf("Departure: %s\n", bus.departureTime);
    printf("Arrival: %s\n", bus.arrivalTime);
    printf("Fare: $%.2f\n", bus.fare);
    printf("Available Seats: %d/%d\n", bus.availableSeats, bus.totalSeats);
}

void displayBookingDetails(Booking booking, Bus bus, User user) {
    printf("\n----------------------------------------\n");
    printf("Booking ID: %d\n", booking.id);
    printf("Passenger: %s\n", user.name);
    printf("Journey: %s to %s\n", bus.source, bus.destination);
    printf("Date: %s\n", booking.journeyDate);
    printf("Departure: %s\n", bus.departureTime);
    printf("Seat: %d\n", booking.seatNumber);
    printf("Fare: $%.2f\n", booking.fare);
    printf("Status: %s\n", booking.cancelled ? "Cancelled" : "Confirmed");
    printf("----------------------------------------\n");
}

void initializeFiles() {
    FILE *userFile = fopen(FILENAME_USERS, "ab+");
    FILE *busFile = fopen(FILENAME_BUSES, "ab+");
    FILE *bookingFile = fopen(FILENAME_BOOKINGS, "ab+");
    
    if (userFile) fclose(userFile);
    if (busFile) fclose(busFile);
    if (bookingFile) fclose(bookingFile);
}

Bus getBusById(int id) {
    FILE *file = fopen(FILENAME_BUSES, "rb");
    Bus bus;
    
    while (file && fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.id == id) {
            fclose(file);
            return bus;
        }
    }
    
    if (file) fclose(file);
    
    // Return empty bus if not found
    Bus emptyBus;
    emptyBus.id = -1;
    strcpy(emptyBus.source, "Unknown");
    strcpy(emptyBus.destination, "Unknown");
    return emptyBus;
}

User getUserById(int id) {
    FILE *file = fopen(FILENAME_USERS, "rb");
    User user;
    
    while (file && fread(&user, sizeof(User), 1, file)) {
        if (user.id == id) {
            fclose(file);
            return user;
        }
    }
    
    if (file) fclose(file);
    
    // Return empty user if not found
    User emptyUser;
    emptyUser.id = -1;
    strcpy(emptyUser.name, "Unknown");
    return emptyUser;
}

void displayAvailableSeats(Bus bus) {
    printf("\nAvailable Seats:\n");
    
    for (int i = 0; i < bus.totalSeats; i++) {
        if (i % 4 == 0) printf("\n");
        
        if (bus.seatMap[i] == 0) {
            printf("[%2d]", i + 1);
        } else {
            printf("[XX]");
        }
        
        if (i % 2 == 1) printf("  ");
    }
    printf("\n\n");
}

void saveBooking(Booking booking) {
    FILE *file = fopen(FILENAME_BOOKINGS, "ab");
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    fwrite(&booking, sizeof(Booking), 1, file);
    fclose(file);
}

void updateBusSeats(int busId, int seatNumber, int status) {
    FILE *file = fopen(FILENAME_BUSES, "rb+");
    Bus bus;
    int found = 0;
    
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    while (fread(&bus, sizeof(Bus), 1, file)) {
        if (bus.id == busId) {
            // Update seat status (0: available, 1: booked)
            bus.seatMap[seatNumber - 1] = status;
            
            // Update available seats count
            if (status == 1) {
                bus.availableSeats--;
            } else {
                bus.availableSeats++;
            }
            
            // Move file pointer back to update this record
            fseek(file, -sizeof(Bus), SEEK_CUR);
            fwrite(&bus, sizeof(Bus), 1, file);
            found = 1;
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        printf("Error updating bus seat information!\n");
    }
}
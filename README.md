# Bus Reservation System

## Overview

A console-based Bus Reservation System implemented in C that allows users to book bus tickets and administrators to manage bus schedules.

## Features

### User Features
- ✅ User registration & login  
- 🚌 View available buses  
- 🎫 Book tickets  
- 📋 View booking history  
- ❌ Cancel bookings  

### Admin Features
- ➕ Add new buses  
- 👀 View all buses  
- ✏️ Modify bus details  
- 🗑️ Delete buses  

## Installation

1. Clone the repository:
```bash
git clone https://github.com/yourusername/bus-reservation-system.git
# Bus Reservation System

## 🛠️ Compile the Program
```bash
gcc bus_reservation.c -o bus_reservation
```

## 🚀 Run the Executable
```bash
./bus_reservation
```

## 🔐 Admin Access
**Default admin credentials:**
```
Username: admin
Password: admin123
```

## 💾 Data Storage
The system uses binary files for data persistence:
- `users.dat` - User accounts
- `buses.dat` - Bus information
- `bookings.dat` - Booking records
- `temp.dat` - Temporary operations file

## 📁 Code Structure
```
bus_reservation.c
├── Data Structures
│   ├── User (id, username, password, name, contact)
│   ├── Bus (id, route, schedule, seats, fare)
│   └── Booking (id, user, bus, seat, dates)
│
├── Core Functions
│   ├── User authentication
│   ├── Bus management
│   ├── Booking system
│   └── File operations
│
└── UI Components
    ├── Main menu
    ├── Admin console
    └── User dashboard
```

## ⚠️ Limitations
- Console-only interface
- Basic file-based storage
- No payment integration
- Single admin account

## 🔮 Future Improvements
- Add GUI interface
- Implement SQL database
- Add payment processing
- Multi-admin support
- Enhanced reporting

## 🖼️ Screenshots
*(Console screenshots would be added here)*

## 📄 License
**MIT License** - Free for educational use

## 📘 How to Use This README
1. Copy all the content above
2. Create a new file named `README.md`
3. Paste the content
4. Save the file

## 🧩 Customization Tips
You can further customize it by:
- Adding actual screenshots
- Including your repository link
- Adding contribution guidelines
- Expanding the license details


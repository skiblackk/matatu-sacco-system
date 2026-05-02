# 🚌 Matatu SACCO Fleet & Fare Tracking System

**Course:** Object Oriented Programming — Cooperative University of Kenya  
**Assignment:** GitHub-Based Automation Tool (CAT Take Away)  
**Student:** Joshua Musumbi | C023/408880/2025

---

## 📋 Project Description

Many Matatu SACCOs in Kenya still manage their fleets manually using notebooks and WhatsApp messages. This leads to:
- Lost or disputed fare records
- Missed vehicle service schedules
- Untracked member penalties and debts

This C++ program automates the core operations of a Matatu SACCO, including:
- Registering member-owners and tracking their vehicles
- Deploying matatus and recording daily fare collections
- Calculating shortfall penalties automatically
- Tracking vehicle service schedules and flagging overdue ones
- Generating reports on fleet status, service dues, and member balances
- Persisting all data to CSV files between sessions

---

## 🏗️ Class Structure

| Class | Role |
|-------|------|
| `Date` | Handles all date arithmetic, formatting, and comparisons |
| `Matatu` | Represents a single vehicle — fare, status, service tracking |
| `Member` | Represents a SACCO member-owner — vehicles, balances, payments |
| `SACCO` | Main controller — manages fleet, members, reports, and file I/O |

---

## ⚙️ Setup Instructions

### Requirements
- A C++ compiler: `g++` (GCC 11+) or any C++17-compatible compiler
- A terminal / command prompt

### How to Compile

```bash
g++ -std=c++17 -Wall -o matatu main.cpp
```

### How to Run

```bash
./matatu        # Linux / macOS
matatu.exe      # Windows
```

> On first run, no CSV files exist and the system starts fresh. On exit (option 13), data is saved to `fleet.csv` and `members.csv`.

---

## 💻 Usage Examples

### Typical Workflow

```
1. Register New Member       → Enter owner details (name, phone, national ID)
2. Add New Matatu to Fleet   → Enter plate, route, driver, daily target, last service date
3. Assign Vehicle to Member  → Link the vehicle to its owner
4. Deploy Vehicle on Route   → Mark vehicle as active for the day
5. Record Fare Collection    → Add fare amounts throughout the day
6. End Vehicle Day           → System calculates penalty if below target
8. Member Payment            → Record payment toward outstanding balance
10. Fleet Status Report      → See all vehicles, status, and service dates
```

### Sample Interaction

```
  -- Register New Member --
  Full Name  : John Kamau
  Phone      : 0722112233
  ID Number  : 12345678
  [OK] Member registered. ID: 1001

  -- Add New Matatu --
  Plate Number: KBZ 001A
  Route       : Nairobi CBD - Thika
  Driver Name : Peter Mwangi
  Daily Target (KES): 8000
  Last Service Date (DD MM YYYY): 01 01 2024
  [OK] Matatu KBZ 001A added to fleet.

  -- Record Fare Collection --
  Plate Number: KBZ 001A
  Fare Amount (KES): 5500
  [OK] KES 5500.00 recorded. Total today: KES 5500.00

  -- End Vehicle Day --
  === End-of-Day Summary: KBZ 001A ===
  Route          : Nairobi CBD - Thika
  Daily Target   : KES 8000.00
  Collected      : KES 5500.00
  Penalty        : KES 600.00
  Net Remittance : KES 4900.00
```

> Screenshots from a compiled run would appear here in a full submission.

---

## 📁 Repository Structure

```
matatu-sacco-system/
├── main.cpp        ← Full C++ source code (all 4 classes + main)
├── README.md       ← This file
├── DESIGN.md       ← OOP design explanation
├── fleet.csv       ← Auto-generated on save (vehicle data)
├── members.csv     ← Auto-generated on save (member data)
└── .gitignore      ← C++ build artifacts ignored
```

---

## 👤 Author

**Name:** Joshua Musumbi  
**Reg No:** C023/408880/2025  
**Institution:** Cooperative University of Kenya  
**GitHub:** skiblackk

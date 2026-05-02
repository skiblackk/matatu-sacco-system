# 🏛️ System Design Document

**Project:** Matatu SACCO Fleet & Fare Tracking System  
**Author:** Joshua Musumbi | C023/408880/2025

---

## OOP Principles Applied

### 1. Encapsulation

Every class keeps its data `private` and exposes only controlled `public` methods.

**Example — `Matatu` class:**
```cpp
class Matatu {
private:
    string plateNumber;
    double collectedFare;    // Cannot be edited directly from outside
    double dailyTarget;

public:
    void collectFare(double amount);   // Controlled entry point — validates amount
    double calculatePenalty() const;   // Read-only calculation
};
```

This prevents invalid states, e.g., a negative fare or a penalty being manually overridden.

**Example — `Member` class:**
```cpp
private:
    double outstandingBalance;   // Never set directly

public:
    void addCharge(double amount, const string& reason);  // Only way to add debt
    void makePayment(double amount);                       // Only way to clear debt
```

### 2. Abstraction

I hid the complicated stuff inside each class so users of the class don't need to understand how it works.

- `Date` handles all the leap year math and stuff - users just call `daysBetween()` or use the `-` operator without worrying about the details.
- `SACCO` does searches for vehicles/members internally - the main code just calls `deployVehicle(plate, date)` and it figures things out.
- `Matatu::endDay()` does everything at once - calculates penalty, prints output, resets status.

### 3. Inheritance (Could Add Later)

Right now I used composition - the SACCO holds Matatus and Members. But I structured it so inheritance could work later if needed:

```
Person (base)
  └── Member (derived) — adds vehicle list, balance
  └── Driver (derived) — adds license, trips_done counter

Vehicle (base)
  └── Matatu (derived) — adds route, SACCO-specific fields
  └── Bus (derived) — adds capacity, fare zones
```

The `Date` class could also be a base for a `DateTime` class in a future version.

### 4. Operator Overloading

The `Date` class overloads the `-` operator to calculate the number of days between two dates in a natural, readable way:

```cpp
// Instead of: int days = date1.daysBetween(date2);
// We can write:
int days = date1 - date2;
```

This is used internally in penalty and service calculations.

---

## Class Design Justification

### Why a separate `Date` class?
I needed to do a lot of date math - checking if service is overdue, calculating penalties per day, that kind of thing. Instead of writing the same date code everywhere, I put it all in one place. Every `Matatu` uses it for tracking service dates.

### Why does `Matatu` not inherit from a base class?
Since all the vehicles in the system are matatus, I didn't need to make a base Vehicle class. Just having one Matatu class is way simpler. The SACCO class manages everything.

### Why does `SACCO` not store vehicle ownership?
Members own the vehicles, not the SACCO. So I store the vehicle list inside the Member class. When a vehicle has a bad day and gets a penalty, it automatically goes to the member who owns it.

### Why CSV for persistence?
I used CSV files because they're simple - I can read/write them with basic C++ file operations. People can also open them in Excel if they want to check the data manually. No need for a complex database.

---

## Alternative Approaches Considered

| Approach | Reason Not Used |
|----------|-----------------|
| Inheritance: `Vehicle → Matatu` | All vehicles are matatus anyway |
| Database (SQLite) | Would need extra libraries; CSV works fine |
| `struct` instead of `class` | Need `private` members for protection |
| Global variables | Would be messy; better to keep things in classes |
| One big main.cpp | Wouldn't show OOP and would be hard to understand |

---

## Data Flow Diagram

```
User Input (keyboard)
        │
        ▼
    main() menu
        │
        ▼
    SACCO (controller)
     ├── fleet: vector<Matatu>
     │       └── uses Date for service tracking
     └── members: vector<Member>
             └── tracks plates and outstanding balances

    On Save: SACCO → fleet.csv, members.csv
    On Load: fleet.csv, members.csv → SACCO
```

---

*End of Design Document*

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

Each class hides complex implementation details behind a clean interface.

- `Date` hides leap year logic, month-day tables, and the conversion to a "total days" number. Callers just use `daysBetween()` or the `-` operator.
- `SACCO` hides the linear search algorithms for locating vehicles/members. Callers just call `deployVehicle(plate, date)` without knowing how the lookup works.
- `Matatu::endDay()` bundles penalty calculation, console output, and status reset into one call.

### 3. Inheritance (Potential Extension)

The current design uses composition (SACCO *has* Matatus and Members), which is appropriate for this domain. However, the system is structured to support inheritance easily in a Phase 2:

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
The assignment requires date arithmetic (service scheduling every 90 days, penalty per day, overdue detection). Centralizing this logic in one reusable class avoids duplication and makes testing easier. Every `Matatu` holds two `Date` objects and relies on `addDays()` and the `-` operator.

### Why does `Matatu` not inherit from a base class?
In this domain, all fleet vehicles are matatus. A single concrete class with clear responsibilities is simpler and less error-prone than premature abstraction. The `SACCO` class acts as the system coordinator.

### Why does `SACCO` not store vehicle ownership?
Vehicle ownership is stored in `Member` (as a vector of plate strings) rather than in `SACCO` or `Matatu`. This mirrors the real world: a member *owns* a vehicle; the SACCO just manages the fleet. Penalties are pushed to the owning member automatically when `endVehicleDay()` is called.

### Why CSV for persistence?
CSV files are human-readable, can be opened in Excel/Sheets for manual inspection, require no database dependency, and are simple to parse with `getline` and `istringstream`. This matches the skill level of the course and the constraints of a standalone C++ program.

---

## Alternative Approaches Considered

| Approach | Reason Not Used |
|----------|-----------------|
| Inheritance: `Vehicle → Matatu` | Only one vehicle type needed; over-engineering |
| Database (SQLite) | Requires external library; CSV is sufficient for scope |
| `struct` instead of `class` | `class` enforces encapsulation with `private` by default |
| Global variables for fleet/members | Violates encapsulation; `SACCO` class manages state cleanly |
| Single large `main.cpp` without classes | Does not demonstrate OOP; difficult to maintain |

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

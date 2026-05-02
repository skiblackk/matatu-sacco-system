// Author: Joshua Musumbi
// Reg No: C023/408880/2025
// Course: Object Oriented Programming - CUK
// Date: May 2026

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <cmath>

using namespace std;

// ============================================================
// CLASS 1: Date
// Handles all date-related operations for the system
// ============================================================
class Date {
private:
    int day, month, year;

    // Helper: check if a year is a leap year
    bool isLeapYear(int y) const {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    // Helper: days in a given month
    int daysInMonth(int m, int y) const {
        int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2 && isLeapYear(y)) return 29;
        return days[m - 1];
    }

    // Convert date to total days since a reference point (for arithmetic)
    long toDays() const {
        long total = 0;
        for (int y = 1; y < year; y++) {
            total += isLeapYear(y) ? 366 : 365;
        }
        for (int m = 1; m < month; m++) {
            total += daysInMonth(m, year);
        }
        total += day;
        return total;
    }

public:
    // Constructor with parameters
    Date(int d, int m, int y) : day(d), month(m), year(y) {
        if (m < 1 || m > 12 || d < 1 || d > daysInMonth(m, y)) {
            throw invalid_argument("Invalid date provided.");
        }
    }

    // Default constructor - defaults to 1 Jan 2024
    Date() : day(1), month(1), year(2024) {}

    // Getters
    int getDay()   const { return day; }
    int getMonth() const { return month; }
    int getYear()  const { return year; }

    // Calculate the number of days between two dates
    int daysBetween(const Date& other) const {
        return abs((int)(this->toDays() - other.toDays()));
    }

    // Return a new Date after adding a given number of days
    Date addDays(int days) const {
        int d = day, m = month, y = year;
        d += days;
        while (d > daysInMonth(m, y)) {
            d -= daysInMonth(m, y);
            m++;
            if (m > 12) { m = 1; y++; }
        }
        return Date(d, m, y);
    }

    // Return formatted date string DD/MM/YYYY
    string toString() const {
        ostringstream ss;
        ss << setfill('0') << setw(2) << day << "/"
           << setfill('0') << setw(2) << month << "/"
           << year;
        return ss.str();
    }

    // Overload - operator: returns days difference between two dates
    int operator-(const Date& other) const {
        return daysBetween(other);
    }

    // Comparison operators
    bool operator<(const Date& other) const { return toDays() < other.toDays(); }
    bool operator>(const Date& other) const { return toDays() > other.toDays(); }
    bool operator==(const Date& other) const { return toDays() == other.toDays(); }
};


// ============================================================
// CLASS 2: Matatu
// Represents a single matatu vehicle in the SACCO fleet
// ============================================================
class Matatu {
private:
    string plateNumber;       // e.g. KDA 123A
    string route;             // e.g. Nairobi CBD - Thika
    string driverName;
    bool isActive;            // true if currently on route
    double dailyTarget;       // KES target the vehicle must collect
    double collectedFare;     // KES collected today
    Date lastServiceDate;     // date of last mechanical service
    Date nextServiceDate;     // due date for next service

public:
    // Constructor
    Matatu(string plate, string rt, string driver, double target, Date lastService)
        : plateNumber(plate), route(rt), driverName(driver),
          isActive(false), dailyTarget(target), collectedFare(0.0),
          lastServiceDate(lastService), nextServiceDate(lastService.addDays(90)) {}

    // Getters
    string getPlate()       const { return plateNumber; }
    string getRoute()       const { return route; }
    string getDriver()      const { return driverName; }
    bool   getIsActive()    const { return isActive; }
    double getDailyTarget() const { return dailyTarget; }
    double getCollected()   const { return collectedFare; }
    Date   getLastService() const { return lastServiceDate; }
    Date   getNextService() const { return nextServiceDate; }

    // Setters
    void setDriver(string d)   { driverName = d; }
    void setRoute(string r)    { route = r; }
    void setActive(bool status){ isActive = status; }

    // Deploy matatu on its route
    void deploy(Date currentDate) {
        if (isActive) {
            cout << "  [!] Matatu " << plateNumber << " is already on route.\n";
            return;
        }
        isActive = true;
        collectedFare = 0.0;
        cout << "  [OK] Matatu " << plateNumber << " deployed on route: " << route << "\n";
    }

    // Record a fare collection (one trip payment in KES)
    void collectFare(double amount) {
        if (!isActive) {
            cout << "  [!] Matatu is not active. Deploy first.\n";
            return;
        }
        if (amount <= 0) {
            throw invalid_argument("Fare amount must be positive.");
        }
        collectedFare += amount;
        cout << "  [OK] KES " << fixed << setprecision(2) << amount
             << " recorded. Total today: KES " << collectedFare << "\n";
    }

    // Calculate shortfall penalty: KES 200 per KES 1000 below target
    double calculatePenalty() const {
        if (collectedFare >= dailyTarget) return 0.0;
        double shortfall = dailyTarget - collectedFare;
        // KES 200 penalty per KES 1000 shortfall (rounded up)
        return ceil(shortfall / 1000.0) * 200.0;
    }

    // End the day: return summary and reset
    double endDay() {
        double penalty = calculatePenalty();
        double remittance = collectedFare - penalty;
        cout << "\n  === End-of-Day Summary: " << plateNumber << " ===\n";
        cout << "  Route          : " << route << "\n";
        cout << "  Driver         : " << driverName << "\n";
        cout << "  Daily Target   : KES " << fixed << setprecision(2) << dailyTarget << "\n";
        cout << "  Collected      : KES " << collectedFare << "\n";
        cout << "  Penalty        : KES " << penalty << "\n";
        cout << "  Net Remittance : KES " << remittance << "\n";
        isActive = false;
        return remittance;
    }

    // Check if service is overdue
    bool isServiceDue(const Date& currentDate) const {
        return !(currentDate < nextServiceDate) || currentDate == nextServiceDate;
    }

    // Record a service
    void recordService(Date serviceDate) {
        lastServiceDate = serviceDate;
        nextServiceDate = serviceDate.addDays(90);
        cout << "  [OK] Service recorded. Next due: " << nextServiceDate.toString() << "\n";
    }

    // Display vehicle info
    void display() const {
        cout << "  Plate   : " << plateNumber << "\n"
             << "  Route   : " << route << "\n"
             << "  Driver  : " << driverName << "\n"
             << "  Status  : " << (isActive ? "On Route" : "Parked") << "\n"
             << "  Target  : KES " << fixed << setprecision(2) << dailyTarget << "\n"
             << "  Last Svc: " << lastServiceDate.toString() << "\n"
             << "  Next Svc: " << nextServiceDate.toString() << "\n";
    }

    // Serialize to CSV row
    string toCSV() const {
        ostringstream ss;
        ss << plateNumber << "," << route << "," << driverName << ","
           << (isActive ? "1" : "0") << "," << dailyTarget << "," << collectedFare << ","
           << lastServiceDate.getDay() << "," << lastServiceDate.getMonth() << ","
           << lastServiceDate.getYear();
        return ss.str();
    }
};


// ============================================================
// CLASS 3: Member (Driver/Owner registered in SACCO)
// ============================================================
class Member {
private:
    int memberID;
    string name;
    string phone;
    string idNumber;              // National ID
    vector<string> ownedPlates;   // Plate numbers this member owns
    double outstandingBalance;    // KES owed to SACCO

public:
    // Constructor
    Member(int id, string nm, string ph, string idNo)
        : memberID(id), name(nm), phone(ph), idNumber(idNo), outstandingBalance(0.0) {}

    // Getters
    int    getMemberID()   const { return memberID; }
    string getName()       const { return name; }
    string getPhone()      const { return phone; }
    string getIDNumber()   const { return idNumber; }
    double getBalance()    const { return outstandingBalance; }
    const vector<string>& getOwnedPlates() const { return ownedPlates; }

    // Setters
    void setPhone(string ph)   { phone = ph; }
    void setName(string nm)    { name = nm; }

    // Add a vehicle plate to member's ownership
    void addVehicle(const string& plate) {
        ownedPlates.push_back(plate);
        cout << "  [OK] Vehicle " << plate << " linked to member " << name << "\n";
    }

    // Remove a vehicle from member's ownership
    void removeVehicle(const string& plate) {
        auto it = find(ownedPlates.begin(), ownedPlates.end(), plate);
        if (it != ownedPlates.end()) {
            ownedPlates.erase(it);
            cout << "  [OK] Vehicle " << plate << " unlinked from " << name << "\n";
        } else {
            cout << "  [!] Vehicle not found under this member.\n";
        }
    }

    // Add a charge (e.g. penalty or SACCO levy) to member's balance
    void addCharge(double amount, const string& reason) {
        outstandingBalance += amount;
        cout << "  [OK] Charge of KES " << fixed << setprecision(2) << amount
             << " (" << reason << ") added to " << name << ". Balance: KES "
             << outstandingBalance << "\n";
    }

    // Record a payment toward outstanding balance
    void makePayment(double amount) {
        if (amount <= 0) throw invalid_argument("Payment must be positive.");
        if (amount > outstandingBalance) {
            cout << "  [!] Payment exceeds balance. Accepting KES "
                 << fixed << setprecision(2) << outstandingBalance << "\n";
            outstandingBalance = 0.0;
        } else {
            outstandingBalance -= amount;
            cout << "  [OK] Payment of KES " << fixed << setprecision(2) << amount
                 << " accepted. Remaining: KES " << outstandingBalance << "\n";
        }
    }

    // Display member info
    void display() const {
        cout << "  Member ID : " << memberID << "\n"
             << "  Name      : " << name << "\n"
             << "  Phone     : " << phone << "\n"
             << "  ID Number : " << idNumber << "\n"
             << "  Balance   : KES " << fixed << setprecision(2) << outstandingBalance << "\n"
             << "  Vehicles  : ";
        if (ownedPlates.empty()) cout << "None";
        for (const auto& p : ownedPlates) cout << p << " ";
        cout << "\n";
    }

    // Serialize to CSV
    string toCSV() const {
        ostringstream ss;
        ss << memberID << "," << name << "," << phone << "," << idNumber << ","
           << outstandingBalance;
        return ss.str();
    }
};


// ============================================================
// CLASS 4: SACCO (Main controller class)
// Manages the entire fleet and membership
// ============================================================
class SACCO {
private:
    string saccoName;
    vector<Matatu> fleet;
    vector<Member> members;
    int nextMemberID;

    // Find matatu index by plate (returns -1 if not found)
    int findMatatu(const string& plate) const {
        for (int i = 0; i < (int)fleet.size(); i++) {
            if (fleet[i].getPlate() == plate) return i;
        }
        return -1;
    }

    // Find member index by ID (returns -1 if not found)
    int findMember(int id) const {
        for (int i = 0; i < (int)members.size(); i++) {
            if (members[i].getMemberID() == id) return i;
        }
        return -1;
    }

public:
    // Constructor
    SACCO(string name) : saccoName(name), nextMemberID(1001) {}

    // --- Vehicle Management ---

    // Add a new matatu to the fleet
    void addMatatu(Matatu m) {
        if (findMatatu(m.getPlate()) != -1) {
            cout << "  [!] Vehicle " << m.getPlate() << " already in fleet.\n";
            return;
        }
        fleet.push_back(m);
        cout << "  [OK] Matatu " << m.getPlate() << " added to fleet.\n";
    }

    // Deploy a vehicle on its route
    void deployVehicle(const string& plate, Date currentDate) {
        int idx = findMatatu(plate);
        if (idx == -1) { cout << "  [!] Vehicle not found.\n"; return; }
        fleet[idx].deploy(currentDate);
    }

    // Record fare collection for a vehicle
    void recordFare(const string& plate, double amount) {
        int idx = findMatatu(plate);
        if (idx == -1) { cout << "  [!] Vehicle not found.\n"; return; }
        fleet[idx].collectFare(amount);
    }

    // End the day for a vehicle and apply any penalty to the owner
    void endVehicleDay(const string& plate) {
        int idx = findMatatu(plate);
        if (idx == -1) { cout << "  [!] Vehicle not found.\n"; return; }
        double penalty = fleet[idx].calculatePenalty();
        fleet[idx].endDay();
        // Apply penalty to the owning member
        if (penalty > 0) {
            for (auto& m : members) {
                auto& plates = m.getOwnedPlates();
                if (find(plates.begin(), plates.end(), plate) != plates.end()) {
                    m.addCharge(penalty, "Shortfall penalty - " + plate);
                }
            }
        }
    }

    // Record a service for a vehicle
    void recordVehicleService(const string& plate, Date serviceDate) {
        int idx = findMatatu(plate);
        if (idx == -1) { cout << "  [!] Vehicle not found.\n"; return; }
        fleet[idx].recordService(serviceDate);
    }

    // --- Member Management ---

    // Register a new SACCO member
    void registerMember(string name, string phone, string idNo) {
        // Check duplicate ID number
        for (const auto& m : members) {
            if (m.getIDNumber() == idNo) {
                cout << "  [!] Member with ID number " << idNo << " already registered.\n";
                return;
            }
        }
        members.emplace_back(nextMemberID++, name, phone, idNo);
        cout << "  [OK] Member registered. ID: " << (nextMemberID - 1) << "\n";
    }

    // Link a vehicle to a member
    void assignVehicleToMember(int memberID, const string& plate) {
        int midx = findMember(memberID);
        int vidx = findMatatu(plate);
        if (midx == -1) { cout << "  [!] Member not found.\n"; return; }
        if (vidx == -1) { cout << "  [!] Vehicle not found.\n"; return; }
        members[midx].addVehicle(plate);
    }

    // Record a member payment
    void memberPayment(int memberID, double amount) {
        int idx = findMember(memberID);
        if (idx == -1) { cout << "  [!] Member not found.\n"; return; }
        members[idx].makePayment(amount);
    }

    // --- Reports ---

    // Display all vehicles and their current status
    void generateFleetReport(const Date& currentDate) const {
        cout << "\n  ========== FLEET REPORT: " << saccoName << " ==========\n";
        cout << "  Date: " << currentDate.toString() << "\n";
        cout << "  Total Vehicles: " << fleet.size() << "\n\n";
        int i = 1;
        for (const auto& v : fleet) {
            cout << "  --- Vehicle " << i++ << " ---\n";
            v.display();
            if (v.isServiceDue(currentDate)) {
                cout << "  *** SERVICE OVERDUE ***\n";
            }
            cout << "\n";
        }
    }

    // Display all vehicles with overdue service
    void generateServiceReport(const Date& currentDate) const {
        cout << "\n  ========== OVERDUE SERVICE REPORT ==========\n";
        cout << "  Date: " << currentDate.toString() << "\n";
        bool found = false;
        for (const auto& v : fleet) {
            if (v.isServiceDue(currentDate)) {
                cout << "  Plate: " << v.getPlate()
                     << " | Driver: " << v.getDriver()
                     << " | Due: " << v.getNextService().toString() << "\n";
                found = true;
            }
        }
        if (!found) cout << "  All vehicles are up to date with service.\n";
    }

    // Display members with outstanding balances
    void generateDebtReport() const {
        cout << "\n  ========== OUTSTANDING BALANCES REPORT ==========\n";
        bool found = false;
        for (const auto& m : members) {
            if (m.getBalance() > 0) {
                cout << "  [" << m.getMemberID() << "] " << m.getName()
                     << " - KES " << fixed << setprecision(2) << m.getBalance() << "\n";
                found = true;
            }
        }
        if (!found) cout << "  No outstanding balances.\n";
    }

    // Display full member list
    void listMembers() const {
        cout << "\n  ========== SACCO MEMBERS ==========\n";
        if (members.empty()) { cout << "  No members registered.\n"; return; }
        for (const auto& m : members) {
            m.display();
            cout << "  ---\n";
        }
    }

    // --- File I/O ---

    // Save fleet and members to CSV files
    void saveToFile(const string& fleetFile, const string& memberFile) const {
        // Save fleet
        ofstream fout(fleetFile);
        if (!fout) { cout << "  [!] Could not open " << fleetFile << "\n"; return; }
        fout << "plate,route,driver,active,target,collected,svc_day,svc_month,svc_year\n";
        for (const auto& v : fleet) fout << v.toCSV() << "\n";
        fout.close();

        // Save members
        ofstream mout(memberFile);
        if (!mout) { cout << "  [!] Could not open " << memberFile << "\n"; return; }
        mout << "id,name,phone,idnumber,balance\n";
        for (const auto& m : members) mout << m.toCSV() << "\n";
        mout.close();

        cout << "  [OK] Data saved to " << fleetFile << " and " << memberFile << "\n";
    }

    // Load fleet from CSV file
    void loadFromFile(const string& fleetFile, const string& memberFile) {
        // Load fleet
        ifstream fin(fleetFile);
        if (!fin) { cout << "  [i] No existing fleet file found. Starting fresh.\n"; }
        else {
            string line;
            getline(fin, line); // skip header
            while (getline(fin, line)) {
                istringstream ss(line);
                string plate, route, driver, activeStr, targetStr, collStr,
                       sdayStr, smonStr, syrStr;
                getline(ss, plate,    ','); getline(ss, route,    ',');
                getline(ss, driver,   ','); getline(ss, activeStr,',');
                getline(ss, targetStr,','); getline(ss, collStr,  ',');
                getline(ss, sdayStr,  ','); getline(ss, smonStr,  ',');
                getline(ss, syrStr,   ',');
                try {
                    Date svcDate(stoi(sdayStr), stoi(smonStr), stoi(syrStr));
                    Matatu m(plate, route, driver, stod(targetStr), svcDate);
                    fleet.push_back(m);
                } catch (...) {}
            }
            fin.close();
            cout << "  [OK] Fleet loaded (" << fleet.size() << " vehicles).\n";
        }

        // Load members
        ifstream min2(memberFile);
        if (!min2) { cout << "  [i] No existing member file found. Starting fresh.\n"; }
        else {
            string line;
            getline(min2, line); // skip header
            while (getline(min2, line)) {
                istringstream ss(line);
                string idStr, name, phone, idNo, balStr;
                getline(ss, idStr, ','); getline(ss, name,  ',');
                getline(ss, phone, ','); getline(ss, idNo,  ',');
                getline(ss, balStr,',');
                try {
                    int id = stoi(idStr);
                    Member m(id, name, phone, idNo);
                    members.push_back(m);
                    if (id >= nextMemberID) nextMemberID = id + 1;
                } catch (...) {}
            }
            min2.close();
            cout << "  [OK] Members loaded (" << members.size() << " members).\n";
        }
    }
};

// ============================================================
// UTILITY: Get validated integer input from user
// ============================================================
int getInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(); return val; }
        cin.clear(); cin.ignore(1000, '\n');
        cout << "  [!] Please enter a valid number.\n";
    }
}

double getDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val > 0) { cin.ignore(); return val; }
        cin.clear(); cin.ignore(1000, '\n');
        cout << "  [!] Please enter a valid positive amount.\n";
    }
}

Date getDate(const string& prompt) {
    while (true) {
        cout << prompt << " (DD MM YYYY): ";
        int d, m, y;
        if (cin >> d >> m >> y) {
            cin.ignore();
            try { return Date(d, m, y); }
            catch (...) { cout << "  [!] Invalid date. Try again.\n"; }
        } else {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "  [!] Invalid input. Try again.\n";
        }
    }
}

// ============================================================
// MAIN MENU
// ============================================================
int main() {
    SACCO sacco("Nairobi Express SACCO");

    const string FLEET_FILE  = "fleet.csv";
    const string MEMBER_FILE = "members.csv";

    cout << "\n  ================================================\n";
    cout << "     Matatu SACCO Fleet & Fare Tracking System  \n";
    cout << "     " << "Nairobi Express SACCO" << "\n";
    cout << "  ================================================\n\n";

    // 1. Load existing data
    sacco.loadFromFile(FLEET_FILE, MEMBER_FILE);

    int choice;
    do {
        cout << "\n  ============ MAIN MENU ============\n";
        cout << "  1.  Register New Member\n";
        cout << "  2.  Add New Matatu to Fleet\n";
        cout << "  3.  Assign Vehicle to Member\n";
        cout << "  4.  Deploy Vehicle on Route\n";
        cout << "  5.  Record Fare Collection\n";
        cout << "  6.  End Vehicle Day (calc penalty)\n";
        cout << "  7.  Record Vehicle Service\n";
        cout << "  8.  Member Payment\n";
        cout << "  9.  View All Members\n";
        cout << "  10. Fleet Status Report\n";
        cout << "  11. Service Overdue Report\n";
        cout << "  12. Outstanding Balances Report\n";
        cout << "  13. Save & Exit\n";
        cout << "  ===================================\n";

        choice = getInt("  Enter choice: ");

        switch (choice) {

        case 1: { // Register member
            cout << "\n  -- Register New Member --\n";
            string name, phone, idNo;
            cout << "  Full Name  : "; getline(cin, name);
            cout << "  Phone      : "; getline(cin, phone);
            cout << "  ID Number  : "; getline(cin, idNo);
            if (name.empty() || phone.empty() || idNo.empty()) {
                cout << "  [!] All fields required.\n"; break;
            }
            sacco.registerMember(name, phone, idNo);
            break;
        }

        case 2: { // Add matatu
            cout << "\n  -- Add New Matatu --\n";
            string plate, route, driver;
            cout << "  Plate Number: "; getline(cin, plate);
            cout << "  Route       : "; getline(cin, route);
            cout << "  Driver Name : "; getline(cin, driver);
            double target = getDouble("  Daily Target (KES): ");
            Date lastSvc = getDate("  Last Service Date");
            if (plate.empty() || route.empty() || driver.empty()) {
                cout << "  [!] All fields required.\n"; break;
            }
            Matatu m(plate, route, driver, target, lastSvc);
            sacco.addMatatu(m);
            break;
        }

        case 3: { // Assign vehicle to member
            cout << "\n  -- Assign Vehicle to Member --\n";
            int memberID = getInt("  Member ID: ");
            string plate;
            cout << "  Plate Number: "; getline(cin, plate);
            sacco.assignVehicleToMember(memberID, plate);
            break;
        }

        case 4: { // Deploy vehicle
            cout << "\n  -- Deploy Vehicle --\n";
            string plate;
            cout << "  Plate Number: "; getline(cin, plate);
            Date today = getDate("  Today's Date");
            sacco.deployVehicle(plate, today);
            break;
        }

        case 5: { // Record fare
            cout << "\n  -- Record Fare Collection --\n";
            string plate;
            cout << "  Plate Number: "; getline(cin, plate);
            double amount = getDouble("  Fare Amount (KES): ");
            sacco.recordFare(plate, amount);
            break;
        }

        case 6: { // End vehicle day
            cout << "\n  -- End Vehicle Day --\n";
            string plate;
            cout << "  Plate Number: "; getline(cin, plate);
            sacco.endVehicleDay(plate);
            break;
        }

        case 7: { // Record service
            cout << "\n  -- Record Vehicle Service --\n";
            string plate;
            cout << "  Plate Number: "; getline(cin, plate);
            Date svcDate = getDate("  Service Date");
            sacco.recordVehicleService(plate, svcDate);
            break;
        }

        case 8: { // Member payment
            cout << "\n  -- Member Payment --\n";
            int memberID = getInt("  Member ID: ");
            double amount = getDouble("  Payment Amount (KES): ");
            sacco.memberPayment(memberID, amount);
            break;
        }

        case 9: { // List members
            sacco.listMembers();
            break;
        }

        case 10: { // Fleet report
            Date today = getDate("  Current Date");
            sacco.generateFleetReport(today);
            break;
        }

        case 11: { // Service report
            Date today = getDate("  Current Date");
            sacco.generateServiceReport(today);
            break;
        }

        case 12: { // Debt report
            sacco.generateDebtReport();
            break;
        }

        case 13: { // Save and exit
            sacco.saveToFile(FLEET_FILE, MEMBER_FILE);
            cout << "\n  Goodbye! Asante.\n\n";
            break;
        }

        default:
            cout << "  [!] Invalid choice. Please enter 1-13.\n";
        }

    } while (choice != 13);

    return 0;
}

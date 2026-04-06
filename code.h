#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <filesystem>

using namespace std;

// ================= ENUM =================
enum class Error 
{
    SUCCESS,
    INVALID,
    MISSING,
    UNAVAILABLE,
    NUM_ERRORS
};

// ================= STRUCTS =================
struct Flight 
{
    string flight_number = "missing";
    string airline = "missing";
    string origin = "missing";
    string destination = "missing";
    int seat_capacity = 0;
    vector<string> passengers;
};

struct Airport 
{
    string name = "missing";
    vector<Flight*> flights;
};

// ================= FUNCTION DECLARATIONS =================
void trim(string& str);
string tolowerString(const string& str);
bool destinationExists(const string& destination, const string& destinations_dir = "destinations/");
vector<string> parseCSVLine(const string& line);
Airport* loadAirportFromFile(const string& destination, const string& destinations_dir = "destinations/");
void deleteAirport(Airport* airport);
Error printFlight(const string& destination, const string& destinations_dir = "destinations/", ostream& out = cout);
string validateUsername(const string& username);
bool usernameExists(const string& username, const string& users_dir = "users/");
string getUsername(istream& in = cin, ostream& out = cout, const string& users_dir = "users/");
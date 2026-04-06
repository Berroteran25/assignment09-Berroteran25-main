#include "code.h"

using namespace std;

// ================= trim =================
void trim(string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos) 
    {
        str = "";
    } else 
    {
        str = str.substr(start, end - start + 1);
    }
}

// ================= tolowerString =================
string tolowerString(const string& str) 
{
    string result = str;
    for (char& c : result) 
    {
        c = tolower(static_cast<unsigned char>(c));
    }
    return result;
}

// ================= destinationExists =================
bool destinationExists(const string& destination, const string& destinations_dir) 
{
    filesystem::path path = filesystem::path(destinations_dir) / (destination + ".csv");
    return filesystem::exists(path);
}

// ================= parseCSVLine =================
vector<string> parseCSVLine(const string& line) 
{
    vector<string> result;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) 
    {
        trim(field);
        result.push_back(field);
    }

    if (!line.empty() && line.back() == ',') 
    {
        result.push_back("");
    }

    return result;
}

// ================= loadAirportFromFile =================
Airport* loadAirportFromFile(const string& destination, const string& destinations_dir) 
{
    filesystem::path path = filesystem::path(destinations_dir) / (destination + ".csv");
    ifstream file(path);

    if (!file.is_open()) 
    {
        return nullptr;
    }

    Airport* airport = new Airport;
    airport->name = destination;

    string line;
    while (getline(file, line)) 
    {
        trim(line);
        if (line.empty()) 
        {
            continue;
        }

        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 4) 
        {
            continue;
        }

        try 
        {
            int capacity = stoi(fields[3]);

            Flight* flight = new Flight;
            flight->flight_number = fields[0];
            flight->airline = fields[1];
            flight->origin = fields[2];
            flight->destination = destination;
            flight->seat_capacity = capacity;

            airport->flights.push_back(flight);
        }
        catch (...) 
        {
            continue;
        }
    }

    return airport;
}

// ================= deleteAirport =================
void deleteAirport(Airport* airport) 
{
    if (airport == nullptr) 
    {
        return;
    }

    for (Flight* flight : airport->flights) 
    {
        delete flight;
    }

    delete airport;
}

// ================= printFlight =================
Error printFlight(const string& destination, const string& destinations_dir, ostream& out) 
{
    Airport* airport = loadAirportFromFile(destination, destinations_dir);

    if (airport == nullptr || airport->flights.empty()) 
    {
        deleteAirport(airport);
        return Error::UNAVAILABLE;
    }

    out << left
        << setw(15) << "Flight"
        << setw(15) << "Airline"
        << setw(15) << "Origin"
        << setw(15) << "Capacity" << "\n";

    for (Flight* flight : airport->flights) 
    {
        out << setw(15) << flight->flight_number
            << setw(15) << flight->airline
            << setw(15) << flight->origin
            << setw(15) << flight->seat_capacity << "\n";
    }

    deleteAirport(airport);
    return Error::SUCCESS;
}

// ================= validateUsername =================
string validateUsername(const string& username) 
{
    if (username.empty()) 
    {
        return "Username cannot be empty.";
    }
    if (username.length() < 3) 
    {
        return "Username must be at least 3 characters.";
    }
    if (username.length() > 20) 
    {
        return "Username must be at most 20 characters.";
    }
    if (!isalpha(static_cast<unsigned char>(username[0]))) 
    {
        return "Username must start with a letter.";
    }

    for (size_t i = 0; i < username.size(); i++) 
    {
        char c = username[i];

        if (!isalnum(static_cast<unsigned char>(c)) && c != '_' && c != '-') 
        {
            return "Username can only contain letters, numbers, underscores, and hyphens.";
        }

        if (i > 0) 
        {
            bool current_special = (c == '_' || c == '-');
            bool previous_special = (username[i - 1] == '_' || username[i - 1] == '-');

            if (current_special && previous_special) 
            {
                return "Username cannot have consecutive underscores or hyphens.";
            }
        }
    }

    return "";
}

// ================= usernameExists =================
bool usernameExists(const string& username, const string& users_dir) 
{
    namespace fs = filesystem;

    if (!fs::exists(users_dir)) 
    {
        return false;
    }

    string target = tolowerString(username);

    for (const auto& entry : fs::directory_iterator(users_dir)) 
    {
        if (!entry.is_regular_file()) 
        {
            continue;
        }

        string stem = entry.path().stem().string();
        if (tolowerString(stem) == target) 
        {
            return true;
        }
    }

    return false;
}

// ================= getUsername =================
string getUsername(istream& in, ostream& out, const string& users_dir) 
{
    namespace fs = filesystem;
    fs::create_directories(users_dir);

    while (true) 
    {
        out << "Do you have a username? (yes/no or cancel):";

        string answer;
        if (!getline(in, answer)) 
        {
            return "";
        }

        trim(answer);
        answer = tolowerString(answer);

        if (answer == "cancel") 
        {
            return "";
        }
        else if (answer == "yes") 
        {
            while (true) 
            {
                out << "\nEnter your username (or type 'cancel'):";

                string username;
                if (!getline(in, username)) 
                {
                    return "";
                }

                trim(username);
                string lowered = tolowerString(username);

                if (lowered == "cancel") 
                {
                    break;
                }

                if (!usernameExists(lowered, users_dir)) 
                {
                    out << "\nThe username provided does not exist. Please try again.\n";
                    continue;
                }

                return lowered;
            }
        }
        else if (answer == "no") 
        {
            while (true) 
            {
                out << "\nPick a username (or type 'cancel'):";

                string username;
                if (!getline(in, username)) 
                {
                    return "";
                }

                trim(username);
                string lowered = tolowerString(username);

                if (lowered == "cancel")
                {
                    break;
                }

                if (username.empty()) 
                {
                    out << "\nUsername cannot be empty. Please try again.\n";
                    continue;
                }

                if (usernameExists(lowered, users_dir)) 
                {
                    out << "\nUsername already taken. Please pick another.\n";
                    continue;
                }

                string error = validateUsername(username);
                if (!error.empty()) 
                {
                    out << "\n" << error << "\n";
                    continue;
                }

                fs::path filepath = fs::path(users_dir) / (lowered + ".csv");
                ofstream file(filepath);

                if (!file.is_open()) 
                {
                    return "";
                }

                out << "\nUsername " << lowered << " created.\n";
                return lowered;
            }
        }
        else {
            out << "\nPick an option and try again!\n";
        }
    }
}
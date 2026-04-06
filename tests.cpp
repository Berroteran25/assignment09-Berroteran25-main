#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "code.h"
#include <iostream>
#include <sstream>

#include <fstream>
#include <filesystem>



/*
TEST_CASE("Enums and structs", "[5 points]") 
{
    // Error
    {
    INFO("Check if Error is an enum");
        STATIC_REQUIRE(std::is_enum<Error>::value);
    }    
    {
    INFO("Check if Error is an enum class and not a plain enum");
        STATIC_REQUIRE(!std::is_convertible<Error, int>::value);
    }
    INFO("Check if Error enum has the correct number of elements.");
        CHECK(static_cast<int>(Error::NUM_ERRORS) == 4);
    // Flight
    {
    INFO("Check if Flight is a class/struct type");
        STATIC_REQUIRE(std::is_class<Flight>::value);
    }
    {
    INFO("Check if Flight has correct element names and default initialization");
        Flight flight;
        CHECK(flight.flight_number == "missing");
        CHECK(flight.airline == "missing");
        CHECK(flight.origin == "missing");
        CHECK(flight.destination == "missing");
        CHECK(flight.seat_capacity == 0);
        CHECK(flight.passengers.empty());
        STATIC_REQUIRE(std::is_same_v<decltype(flight.passengers), std::vector<std::string>>);
    }
    {
    INFO("Check the number of elements in Flight");
        struct TestFlight {
            std::string flight_number = "missing";
            std::string airline = "missing";
            std::string origin = "missing";
            std::string destination  = "missing";
            int seat_capacity = 0;
            std::vector<std::string> passengers {};
        } test_flight;
        Flight flight;
        CHECK(sizeof(test_flight) == sizeof(flight));
    }
    // Airport
    {
    INFO("Check if Airport is a class/struct type");
        STATIC_REQUIRE(std::is_class<Airport>::value);
    }
    {
    INFO("Check if Airport has correct element names and default initialization");
        Airport airport;
        CHECK(airport.name == "missing");
        CHECK(airport.flights.empty());
        STATIC_REQUIRE(std::is_same_v<decltype(airport.flights), std::vector<Flight*>>);
    }
    {
    INFO("Check the number of elements in Barista");
        struct TestAirport{
            std::string name = "missing"; 
            std::vector<Flight*> flights {};
        } test_airport;
        Airport airport;
        CHECK(sizeof(test_airport) == sizeof(airport));
    }
}

TEST_CASE("trim() function", "[10 points]") {
    
    SECTION("Standard inputs with leading and/or trailing whitespace") {
        std::string s1 = "  hello  ";
        trim(s1);
        REQUIRE(s1 == "hello");
        
        std::string s2 = "\thello\t";
        trim(s2);
        REQUIRE(s2 == "hello");
        
        std::string s3 = "\r\nhello\r\n";
        trim(s3);
        REQUIRE(s3 == "hello");
        
        std::string s4 = "  \t  hello  \r\n  ";
        trim(s4);
        REQUIRE(s4 == "hello");
    }
    
    SECTION("Input with multiple words and whitespace") {
        std::string s1 = "   hello  world  ";
        trim(s1);
        REQUIRE(s1 == "hello  world");
        
        std::string s2 = "\t\nhello\tworld\n";
        trim(s2);
        REQUIRE(s2 == "hello\tworld");
    }
    
    
    SECTION("Input with no whitespace to trim") {
        std::string s1 = "hello";
        trim(s1);
        REQUIRE(s1 == "hello");
        
        std::string s2 = "";
        trim(s2);
        REQUIRE(s2 == "");
        
        std::string s3 = "a";
        trim(s3);
        REQUIRE(s3 == "a");
    }
    
    SECTION("Edge cases - only whitespace") {
        std::string s1 = "   ";
        trim(s1);
        REQUIRE(s1 == "");
        
        std::string s2 = "\t\t";
        trim(s2);
        REQUIRE(s2 == "");
        
        std::string s3 = "\r\n\r\n";
        trim(s3);
        REQUIRE(s3 == "");
        
        std::string s4 = " \t \r \n ";
        trim(s4);
        REQUIRE(s4 == "");
    }
    
    SECTION("Edge cases - empty string") {
        std::string s1 = "";
        trim(s1);
        REQUIRE(s1 == "");
    }
    
    SECTION("Edge cases - mixed whitespace characters") {
        std::string s1 = "  \t  \r  \n  content  \t  ";
        trim(s1);
        REQUIRE(s1 == "content");
        
        std::string s2 = " \tcontent\r\n";
        trim(s2);
        REQUIRE(s2 == "content");
    }
}

TEST_CASE("tolowerString() function", "[5 points]") {
    
    SECTION("Standard inputs - mixed case strings") {
        std::string s2 = "HeLLo WoRLD";
        REQUIRE(tolowerString(s2) == "hello world");
    }
    
    SECTION("Input with only lowercase - should remain unchanged") {      
        std::string s3 = "already lowercase";
        REQUIRE(tolowerString(s3) == "already lowercase");
    }
    
    SECTION("Input with only uppercase") {
        std::string s2 = "HELLO WORLD";
        REQUIRE(tolowerString(s2) == "hello world");
    }
    
    SECTION("Input with special characters and numbers") {      
        std::string s2 = "TEST@EMAIL.COM";
        REQUIRE(tolowerString(s2) == "test@email.com");
        
        std::string s3 = "Price: $100";
        REQUIRE(tolowerString(s3) == "price: $100");
    }
    
    SECTION("Edge case - empty string") {
        std::string s1 = "";
        REQUIRE(tolowerString(s1) == "");
    }
    
    SECTION("Edge case - string with only special characters") {
        std::string s1 = "!@#$%";
        REQUIRE(tolowerString(s1) == "!@#$%");
    }
}

TEST_CASE("destinationExists() function", "[5 points]")  {

    // Create test destinations directory
    auto createDestDirectory = []() {
        std::filesystem::create_directory("test_destinations");
    };

    // Create test dest file in a specified directory
    auto createDestinationFile = [](const std::string& destination,  const std::string& dest_dir, const std::string& content) {
        std::ofstream file(dest_dir + destination + ".csv");
        file << content;
        file.close();
    };
    
    // Remove test destination file
    auto removeDestinationFile = [](const std::string& destination,  const std::string& dest_dir) {
        std::filesystem::remove(dest_dir + destination + ".csv");
    };
    
    // Cleanup test directory
    auto cleanupDirectory = [](const std::string& dest_dir) {
        std::filesystem::remove_all(dest_dir);
    };

    // Create test directory before tests
    const std::string dest_dir = "test_destinations/";
    createDestDirectory();
    createDestinationFile("NewYork", dest_dir,
        "AA100,American Airlines,Los Angeles,150\n"
        "UA200,United Airlines,Chicago,180\n"
        "DL300,Delta,Miami,200\n"
    );
    createDestinationFile("DC", dest_dir,
        "AA100,American Airlines,Los Angeles,150\n"
        "UA200,United Airlines,Chicago,180\n"
        "DL300,Delta,Miami,200\n"
    );
    createDestinationFile("Roanoke", dest_dir,
        "AA100,American Airlines,Los Angeles,150\n"
        "UA200,United Airlines,Chicago,180\n"
        "DL300,Delta,Miami,200\n"
    );
    
    SECTION("Valid destination that exists") {
        REQUIRE(destinationExists("NewYork", dest_dir) == true);
        REQUIRE(destinationExists("DC", dest_dir) == true);
        REQUIRE(destinationExists("Roanoke", dest_dir) == true);
    }
    
    SECTION("Invalid destination that does not exist") {
        REQUIRE(destinationExists("NonExistentCity", dest_dir) == false);
        REQUIRE(destinationExists("Nowhere", dest_dir) == false);
        REQUIRE(destinationExists("FakeDestination123", dest_dir) == false);
    }
    
    SECTION("Invalid inputs - empty string") {
        // Empty string would try to open "destinations/.csv"
        REQUIRE(destinationExists("", dest_dir) == false);
    }
    
    SECTION("Invalid inputs - special characters") {
        // These should not exist in our test directory
        REQUIRE(destinationExists("NewYork!", dest_dir) == false);
        REQUIRE(destinationExists("Paris@#", dest_dir) == false);
        REQUIRE(destinationExists("test$file", dest_dir) == false);
    }
    removeDestinationFile("NewYork", dest_dir);
    removeDestinationFile("DC", dest_dir);
    removeDestinationFile("Roanoke", dest_dir);
    cleanupDirectory(dest_dir);
}

TEST_CASE("parseCSVLine() function", "[10 points]")  {
   
    SECTION("Standard CSV line with multiple fields") {
        auto result = parseCSVLine("name,age,city");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "name");
        REQUIRE(result[1] == "age");
        REQUIRE(result[2] == "city");

        result = parseCSVLine("John Doe,30,New York");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "John Doe");
        REQUIRE(result[1] == "30");
        REQUIRE(result[2] == "New York");
    }
    
    SECTION("Fields with leading/trailing whitespace") {
        auto result = parseCSVLine("  John,   Jane,  Bob");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "John");
        REQUIRE(result[1] == "Jane");
        REQUIRE(result[2] == "Bob");

        result = parseCSVLine("John  ,Jane  ,Bob  ");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "John");
        REQUIRE(result[1] == "Jane");
        REQUIRE(result[2] == "Bob");
    }
    
    SECTION("Fields with mixed whitespace (tabs and spaces)") {
        auto result = parseCSVLine("\tJohn\t,\tJane\t,\tBob\t");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "John");
        REQUIRE(result[1] == "Jane");
        REQUIRE(result[2] == "Bob");
    }
    
    SECTION("Empty fields - consecutive commas") {
        auto result = parseCSVLine("John,,Jane");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "John");
        REQUIRE(result[1] == "");
        REQUIRE(result[2] == "Jane");

        result = parseCSVLine(",,,");
        REQUIRE(result.size() == 4);
        for (const auto& field : result) {
            REQUIRE(field == "");
        }
    }
    
    SECTION("INFO: Fields containing spaces within") {
        auto result = parseCSVLine("New York,Los Angeles,San Francisco");
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "New York");
        REQUIRE(result[1] == "Los Angeles");
        REQUIRE(result[2] == "San Francisco");
    }
}

TEST_CASE("loadAirportFromFile() function", "[15 points]") {

    // Create test destinations directory
    auto createDestDirectory = []() {
        std::filesystem::create_directory("test_destinations");
    };

    // Create test dest file in a specified directory
    auto createDestinationFile = [](const std::string& destination,  const std::string& dest_dir, const std::string& content) {
        std::ofstream file(dest_dir + destination + ".csv");
        file << content;
        file.close();
    };
    
    // Remove test destination file
    auto removeDestinationFile = [](const std::string& destination,  const std::string& dest_dir) {
        std::filesystem::remove(dest_dir + destination + ".csv");
    };
    
    // Cleanup flight memory
    auto cleanupFlights = [](Airport* airport) {
        for (auto* f : airport->flights) {
            delete f;
        }
        airport->flights.clear();
    };
    
    // Cleanup entire airport object
    auto cleanupAirport = [cleanupFlights](Airport* airport) {
        cleanupFlights(airport);
        delete airport;
    };
    
    // Cleanup test directory
    auto cleanupDirectory = [](const std::string& dest_dir) {
        std::filesystem::remove_all(dest_dir);
    };

    // Create test directory before tests
    const std::string dest_dir = "test_destinations/";
    createDestDirectory();
    
    SECTION("Standard valid destination with multiple flights") {
        std::string destination = "NewYork";
        createDestinationFile(destination, dest_dir,
            "AA100,American Airlines,Los Angeles,150\n"
            "UA200,United Airlines,Chicago,180\n"
            "DL300,Delta,Miami,200\n"
        );
        
        Airport* airport = loadAirportFromFile(destination, dest_dir);
        
        REQUIRE(airport != nullptr);
        REQUIRE(airport->name == destination);
        REQUIRE(airport->flights.size() == 3);
        REQUIRE(airport->flights[0]->flight_number == "AA100");
        REQUIRE(airport->flights[0]->airline == "American Airlines");
        REQUIRE(airport->flights[0]->origin == "Los Angeles");
        REQUIRE(airport->flights[0]->destination == destination);
        REQUIRE(airport->flights[0]->seat_capacity == 150);
        
        cleanupAirport(airport);
        removeDestinationFile(destination, dest_dir);
    }
    
    SECTION("Valid destination with whitespace in fields") {
        std::string destination = "Tokyo";
        createDestinationFile(destination, dest_dir,
            "  JL001  ,  Japan Airlines  ,  Seoul  ,  250  \n"
            "NH002,All Nippon Airways,Osaka,180\n"
        );
        
        Airport* airport = loadAirportFromFile(destination, dest_dir);
        
        REQUIRE(airport != nullptr);
        REQUIRE(airport->flights.size() == 2);
        REQUIRE(airport->flights[0]->flight_number == "JL001");
        REQUIRE(airport->flights[0]->airline == "Japan Airlines");
        
        cleanupAirport(airport);
        removeDestinationFile(destination, dest_dir);
    }
    
    SECTION("Invalid destination - file does not exist") {
        REQUIRE(loadAirportFromFile("NonExistentCity123", dest_dir) == nullptr);
        REQUIRE(loadAirportFromFile("", dest_dir) == nullptr);
    }
    
    SECTION("Empty file - no flights loaded") {
        std::string destination = "EmptyAirport";
        createDestinationFile(destination, dest_dir, "");
        
        Airport* airport = loadAirportFromFile(destination, dest_dir);
        
        REQUIRE(airport != nullptr);
        REQUIRE(airport->name == destination);
        REQUIRE(airport->flights.size() == 0);
        
        cleanupAirport(airport);
        removeDestinationFile(destination, dest_dir);
    }
    cleanupDirectory(dest_dir);
}

TEST_CASE("deleteAirport() function", "[5 points]") {

    // Create a flight with given parameters
    auto createFlight = [](const std::string& flightNum, const std::string& airline,
                          const std::string& origin, const std::string& dest, int capacity) -> Flight* {
        Flight* f = new Flight;
        f->flight_number = flightNum;
        f->airline = airline;
        f->origin = origin;
        f->destination = dest;
        f->seat_capacity = capacity;
        return f;
    };
    
    // Create an airport with given name and flights
    auto createAirportWithFlights = [&](const std::string& name, 
                                        const std::vector<Flight*>& flights) -> Airport* {
        Airport* airport = new Airport;
        airport->name = name;
        airport->flights = flights;
        return airport;
    };
    
    SECTION("Standard case - airport with multiple flights") {
        std::vector<Flight*> flights = {
            createFlight("AA100", "American Airlines", "New York", "Boston", 150),
            createFlight("UA200", "United Airlines", "Los Angeles", "Boston", 180),
            createFlight("DL300", "Delta", "Chicago", "Boston", 200)
        };
        Airport* airport = createAirportWithFlights("Boston", flights);
        
        // Function should complete without throwing
        REQUIRE_NOTHROW(deleteAirport(airport));
    }

    SECTION("INFO: Invalid input - null pointer") {
        // Passing nullptr should not crash 
        REQUIRE_NOTHROW(deleteAirport(nullptr));
    }

    SECTION("Multiple deletes in sequence") {
        // Create multiple airports and delete them
        for (int i = 0; i < 10; i++) {
            Airport* airport = new Airport;
            airport->name = "Airport" + std::to_string(i);
            airport->flights = {
                createFlight("FL" + std::to_string(i), "Airline", "Origin", "Dest", 100)
            };
            
            REQUIRE_NOTHROW(deleteAirport(airport));
        }
    }
    
    SECTION("Verify no memory leak - using valgrind") {
        // This test is implemented on the autograder
    }

}

#include <algorithm>
TEST_CASE("printFlight() function", "[5 points]") {

    // Create test destinations directory
    auto createDestDirectory = []() {
        std::filesystem::create_directory("test_destinations");
    };

    // Create test dest file in a specified directory
    auto createDestinationFile = [](const std::string& destination,  const std::string& dest_dir, const std::string& content) {
        std::ofstream file(dest_dir + destination + ".csv");
        file << content;
        file.close();
    };
    
    // Remove test destination file
    auto removeDestinationFile = [](const std::string& destination,  const std::string& dest_dir) {
        std::filesystem::remove(dest_dir + destination + ".csv");
    };

    // Cleanup test directory
    auto cleanupDirectory = [](const std::string& dest_dir) {
        std::filesystem::remove_all(dest_dir);
    };

    // Create test directory before tests
    const std::string dest_dir = "test_destinations/";
    createDestDirectory();
    
    SECTION("Standard valid destination - returns SUCCESS") {
        std::string destination = "NewYork";
        createDestinationFile(destination, dest_dir,
            "AA100,American Airlines,Los Angeles,150\n"
            "UA200,United Airlines,Chicago,180\n"
        );
        
        std::stringstream output;
        Error result = printFlight(destination, dest_dir, output);
        
        REQUIRE(result == Error::SUCCESS);
        
        std::string outputStr = output.str();
        REQUIRE(outputStr.find("Flight") != std::string::npos);
        REQUIRE(outputStr.find("AA100") != std::string::npos);
        REQUIRE(outputStr.find("American Airlines") != std::string::npos);
        REQUIRE(outputStr.find("Los Angeles") != std::string::npos);
        REQUIRE(outputStr.find("150") != std::string::npos);
        
        // Check that output has multiple lines (header + flights)
        size_t newlineCount = std::count(outputStr.begin(), outputStr.end(), '\n');
        REQUIRE(newlineCount >= 3);  // Header + 2 flights 
        removeDestinationFile(destination, dest_dir);
    }
    
    SECTION("Non-existent destination - returns UNAVAILABLE") {
        std::stringstream output;
        Error result = printFlight("NonExistentCityXYZ", dest_dir, output);
        
        REQUIRE(result == Error::UNAVAILABLE);
        REQUIRE(output.str().empty());

        result = printFlight("", dest_dir, output);
        // File doesn't exist, should return UNAVAILABLE
        REQUIRE(result == Error::UNAVAILABLE);
    }
    cleanupDirectory(dest_dir);
}

TEST_CASE("validateUsername() function", "[10 points]") {

    SECTION("Valid usernames") {
        REQUIRE(validateUsername("john") == "");
        REQUIRE(validateUsername("john_doe") == "");
        REQUIRE(validateUsername("user-name") == "");
        REQUIRE(validateUsername("user123") == "");
        REQUIRE(validateUsername("User_Name123") == "");
        REQUIRE(validateUsername("abc") == "");  // Minimum length
        REQUIRE(validateUsername("abcdefghijklmnopqrst") == "");  // 20 chars (max 20)
        REQUIRE(validateUsername("validUser") == "");
        REQUIRE(validateUsername("test_user-name") == "");
    }
    
    SECTION("Invalid - empty or whitespace only") {
        REQUIRE(validateUsername("") != "");
        REQUIRE(validateUsername("   ") != "");
        REQUIRE(validateUsername("\t") != "");
        REQUIRE(validateUsername("\n") != "");
        REQUIRE(validateUsername("  \t  ") != "");
    }
    
    SECTION("Invalid - too short") {
        REQUIRE(validateUsername("ab") != "");  // Less than 3
        REQUIRE(validateUsername("a") != "");
        REQUIRE(validateUsername("") != "");
        REQUIRE(validateUsername("12") != "");  // 2 chars
    }
    
    SECTION("Invalid - too long") {
        REQUIRE(validateUsername("abcdefghijklmnopqrstu") != "");  // 21 chars
        REQUIRE(validateUsername("aaaaaaaaaaaaaaaaaaaaa") != "");
        REQUIRE(validateUsername("123456789012345678901") != "");
    }
    
    SECTION("Invalid - special characters") {
        REQUIRE(validateUsername("user!") != "");
        REQUIRE(validateUsername("user@name") != "");
        REQUIRE(validateUsername("user#123") != "");
        REQUIRE(validateUsername("user name") != "");
        REQUIRE(validateUsername("user.name") != "");
        REQUIRE(validateUsername("user/name") != "");
        REQUIRE(validateUsername("user\\name") != "");
        REQUIRE(validateUsername("user&name") != "");
        REQUIRE(validateUsername("user*name") != "");
        REQUIRE(validateUsername("user+name") != "");
        REQUIRE(validateUsername("user.name") != "");
    }
    
    SECTION("Invalid - starts with number") {
        REQUIRE(validateUsername("123user") != "");
        REQUIRE(validateUsername("1user") != "");
        REQUIRE(validateUsername("0") != "");
    }
    
    SECTION("Invalid - consecutive special characters") {
        REQUIRE(validateUsername("user__name") != "");
        REQUIRE(validateUsername("user--name") != "");
        REQUIRE(validateUsername("user_-name") != "");
        REQUIRE(validateUsername("user-_name") != "");
        REQUIRE(validateUsername("__test") != "");
        REQUIRE(validateUsername("--test") != "");
    }

    SECTION("Error message verification") {
        REQUIRE(validateUsername("") == "Username cannot be empty.");
        REQUIRE(validateUsername("ab") == "Username must be at least 3 characters.");
        REQUIRE(validateUsername("abcdefghijklmnopqrstu") == "Username must be at most 20 characters.");
        REQUIRE(validateUsername("123") == "Username must start with a letter.");
        REQUIRE(validateUsername("user!") == "Username can only contain letters, numbers, underscores, and hyphens.");
        REQUIRE(validateUsername("user__name") == "Username cannot have consecutive underscores or hyphens.");
    }
}

TEST_CASE("usernameExists() function", "[5 points]") {
    // Create test users directory
    auto createUsersDirectory = []() {
        std::filesystem::create_directory("test_users");
    };
    
    // Create a test user file in specified directory
    auto createUserFile = [](const std::string& username, const std::string& users_dir) {
        std::ofstream file(users_dir + username + ".csv");
        file << "user data";
        file.close();
    };
    
    // Remove test user file
    auto removeUserFile = [](const std::string& username, const std::string& users_dir) {
        std::filesystem::remove(users_dir + username + ".csv");
    };
    
    // Cleanup test directory
    auto cleanupDirectory = [](const std::string& users_dir) {
        std::filesystem::remove_all(users_dir);
    };
    
    // Create test directory before tests
    const std::string users_dir = "test_users/";
    createUsersDirectory();
    
    SECTION("Multiple valid usernames that exist") {
        std::vector<std::string> usernames = {"alice", "bob-4", "charlie_5"};
        
        for (const auto& user : usernames) {
            createUserFile(user, users_dir);
            INFO("Testing existing user: " << user);
            REQUIRE(usernameExists(user, users_dir) == true);
            removeUserFile(user, users_dir);
        }
    }
    
    SECTION("Standard usernames that do not exist") {
        REQUIRE(usernameExists("nonexistent_user", users_dir) == false);
        REQUIRE(usernameExists("fakeuser123", users_dir) == false);
        REQUIRE(usernameExists("notreal", users_dir) == false);
    }
    
    SECTION("Invalid input") {
        REQUIRE(usernameExists("", users_dir) == false);
        REQUIRE(usernameExists("user!", users_dir) == false);
        REQUIRE(usernameExists("user@name", users_dir) == false);
        REQUIRE(usernameExists("user#123", users_dir) == false);
        REQUIRE(usernameExists("user$file", users_dir) == false);
        REQUIRE(usernameExists("user%test", users_dir) == false);
        REQUIRE(usernameExists("user&name", users_dir) == false);
        REQUIRE(usernameExists("user*name", users_dir) == false);
        REQUIRE(usernameExists("user.name", users_dir) == false);  // dot not allowed
        REQUIRE(usernameExists("user/name", users_dir) == false);  // slash not allowed
        REQUIRE(usernameExists("user\\name", users_dir) == false); // backslash not allowed
        REQUIRE(usernameExists("user name", users_dir) == false);  // space not allowed
        REQUIRE(usernameExists("   ", users_dir) == false);
        REQUIRE(usernameExists("\t", users_dir) == false);
        REQUIRE(usernameExists("\n", users_dir) == false);
        REQUIRE(usernameExists(" user", users_dir) == false);
        REQUIRE(usernameExists("user ", users_dir) == false);
    }

    SECTION("INFO: Edge case - non-existent directory") {
        std::string nonexistentDir = "nonexistent_directory/";
        
        REQUIRE(usernameExists("anyuser", nonexistentDir) == false);
    }
    cleanupDirectory(users_dir);
}

TEST_CASE("getUsername() function", "[25 points]") {

    const std::string users_dir = "test_users/";
    
    // Create test users directory
    auto createUsersDirectory = [&users_dir]() {
        std::filesystem::create_directory(users_dir);
    };
    
    // Create a test user file
    auto createUserFile = [&users_dir](const std::string& username) {
        std::filesystem::create_directory(users_dir);
        std::ofstream file(users_dir + username + ".csv");
        file << "user data";
        file.close();
    };
    
    // Remove a test user file
    auto removeUserFile = [&users_dir](const std::string& username) {
        std::filesystem::remove(users_dir + username + ".csv");
    };
    
    // Setup
    createUsersDirectory();
    
    // Cleanup after all tests
    std::atexit([]() {
        std::filesystem::remove_all("test_users");
    });
    
    SECTION("User cancels at first prompt") {
        std::stringstream input("cancel");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "");
        REQUIRE(output.str().find("Do you have a username") != std::string::npos);
    }
    
    SECTION("User cancels during new username creation") {
        std::stringstream input("no\ncancel\ncancel");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "");
        REQUIRE(output.str().find("Pick a username") != std::string::npos);
    }
    
    SECTION("User cancels during existing username entry") {
        std::stringstream input("yes\ncancel\ncancel");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "");
    }
    
    SECTION("Create new valid username") {
        std::string newUsername = "newuser123";
        std::stringstream input("no\n" + newUsername);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == newUsername);
        REQUIRE(std::filesystem::exists(users_dir + newUsername + ".csv"));
        
        removeUserFile(newUsername);
    }
    
    SECTION("Username already taken - validation passes but exists") {
        std::string takenUser = "taken_user";
        createUserFile(takenUser);
        
        std::stringstream input("no\n" + takenUser + "\nvaliduser");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "validuser");
        REQUIRE(output.str().find("already taken") != std::string::npos);
        
        removeUserFile(takenUser);
        removeUserFile("validuser");
    }
    
    SECTION("Invalid username - too short") {
        std::stringstream input("no\nab\nvaliduser");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "validuser");
        REQUIRE(output.str().find("at least 3 characters") != std::string::npos);
        
        removeUserFile("validuser");
    }

    SECTION("Use existing valid username") {
        std::string existingUser = "existinguser";
        createUserFile(existingUser);
        
        std::stringstream input("yes\n" + existingUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == existingUser);
        
        removeUserFile(existingUser);
    }
    
    SECTION("Existing username not found") {
        std::stringstream input("yes\nnonexistent\ncancel\ncancel");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result != "nonexistent");
        REQUIRE(output.str().find("does not exist") != std::string::npos);
    }
    
    SECTION("Invalid answer at first prompt - retries") {
        std::stringstream input("maybe\nno\nnewuser");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "newuser");
        REQUIRE(output.str().find("Pick an option") != std::string::npos);
        
        removeUserFile("newuser");
    }
    
    SECTION("Various invalid answers") {
        std::vector<std::string> invalidInputs = {"y", "n", "yeah", "nope", "1", ""};
        
        for (size_t i = 0; i < invalidInputs.size(); i++) {
            std::string newUser = "testuser" + std::to_string(i);
            std::stringstream input(invalidInputs[i] + "\nno\n" + newUser);
            std::stringstream output;
            
            std::string result = getUsername(input, output, users_dir);
            
            REQUIRE(result == newUser);
            REQUIRE(output.str().find("Pick an option") != std::string::npos);
            
            removeUserFile(newUser);
        }
    }
    
    SECTION("Case insensitivity - YES accepted") {
        std::string existingUser = "caseuser";
        createUserFile(existingUser);
        
        std::stringstream input("YES\n" + existingUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == existingUser);
        
        removeUserFile(existingUser);
    }
    
    SECTION("Case insensitivity - NO accepted") {
        std::string newUser = "lowercaseuser";
        std::stringstream input("NO\n" + newUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == newUser);
        
        removeUserFile(newUser);
    }
    
    SECTION("Case insensitivity - CANCEL accepted") {
        std::stringstream input("CANCEL");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "");
    }
    
    SECTION("Username converted to lowercase on creation") {
        std::stringstream input("no\nNewUser");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        // Username should be converted to lowercase
        REQUIRE(result == "newuser");
        REQUIRE(std::filesystem::exists(users_dir + "newuser.csv"));
        
        removeUserFile("newuser");
    }
    
    SECTION("Username converted to lowercase on login") {
        std::string existingUser = "mixedcase";
        createUserFile(existingUser);
        
        std::stringstream input("yes\nMixedCase");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        // Should find the lowercase version
        REQUIRE(result == "mixedcase");
        
        removeUserFile(existingUser);
    }
    
    SECTION("Multiple failed login attempts before success") {
        std::string existingUser = "patientuser";
        createUserFile(existingUser);
        
        std::stringstream input("yes\nbaduser1\nyes\nbaduser2\nyes\nbaduser3\nyes\n" + existingUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == existingUser);
        
        // Count "does not exist" messages
        std::string outputStr = output.str();
        size_t count = 0;
        size_t pos = 0;
        while ((pos = outputStr.find("does not exist", pos)) != std::string::npos) {
            count++;
            pos += 14;
        }
        REQUIRE(count >= 3);
        
        removeUserFile(existingUser);
    }
    
    SECTION("Multiple failed creation attempts before success") {
        std::string takenUser1 = "taken1";
        std::string takenUser2 = "taken2";
        createUserFile(takenUser1);
        createUserFile(takenUser2);
        
        std::stringstream input("no\n" + takenUser1 + "\n" + takenUser2 + "\neventuallyvalid");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "eventuallyvalid");
        
        // Count "already taken" messages
        std::string outputStr = output.str();
        size_t count = 0;
        size_t pos = 0;
        while ((pos = outputStr.find("already taken", pos)) != std::string::npos) {
            count++;
            pos += 13;
        }
        REQUIRE(count >= 2);
        
        removeUserFile(takenUser1);
        removeUserFile(takenUser2);
        removeUserFile("eventuallyvalid");
    }
    
    SECTION("Output contains confirmation message on success") {
        std::string newUser = "confirmuser";
        std::stringstream input("no\n" + newUser);
        std::stringstream output;
        
        getUsername(input, output, users_dir);
        
        REQUIRE(output.str().find("Username " + newUser + " created") != std::string::npos);
        
        removeUserFile(newUser);
    }
    
    SECTION("Sequence - cancel then create new username") {
        std::string newUser = "aftercancel";
        std::stringstream input("yes\ncancel\nno\n" + newUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == newUser);
        
        removeUserFile(newUser);
    }
    
    SECTION("Sequence - no then yes - second attempt succeeds") {
        std::string existingUser = "switchuser";
        createUserFile(existingUser);
        
        std::stringstream input("no\ncancel\nyes\n" + existingUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == existingUser);
        
        removeUserFile(existingUser);
    }

        SECTION("Sequence - yes then invalid - second attempt succeeds") {
        std::string existingUser = "switchuser";
        createUserFile(existingUser);
        
        std::stringstream input("yes\ninvalid\n" + existingUser);
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == existingUser);
        
        removeUserFile(existingUser);
    }
    
    SECTION("Multiple validation errors in sequence") {
        // Too short, then special chars, then valid
        std::stringstream input("no\nab\nuser!@#\nvaliduser");
        std::stringstream output;
        
        std::string result = getUsername(input, output, users_dir);
        
        REQUIRE(result == "validuser");
        std::string outputStr = output.str();
        REQUIRE(outputStr.find("at least 3 characters") != std::string::npos);
        REQUIRE(outputStr.find("can only contain letters") != std::string::npos);
        
        removeUserFile("validuser");
    }
    
}
*/






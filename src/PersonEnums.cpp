#include "PersonEnums.h"
#include <algorithm>
#include <cctype>


// ------------------------------------------------------------
// Helper functions for string cleanup
// ------------------------------------------------------------

// Trim whitespace from both ends of a string.
static std::string trim(const std::string& input) {
    int start = 0;
    int end = input.size();

    while (start < end && std::isspace(static_cast<unsigned char>(input[start]))) {
        start++;
    }
    while (end > start && std::isspace(static_cast<unsigned char>(input[end - 1]))) {
        end--;
    }

    return input.substr(start, end - start);
}

// Convert a string to lowercase (used for matching).
static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

// ------------------------------------------------------------
// PrimaryOS
// ------------------------------------------------------------

PrimaryOS parse_primary_os(const std::string& s) {
    std::string val = lower(trim(s));

    if (val == "macos" || val == "mac")   return PrimaryOS::MacOS;
    if (val == "windows" || val == "win") return PrimaryOS::Windows;
    if (val == "linux")                   return PrimaryOS::Linux;

    return PrimaryOS::Unknown;
}

std::string to_string(PrimaryOS os) {
    switch (os) {
        case PrimaryOS::MacOS:   return "MacOS";
        case PrimaryOS::Windows: return "Windows";
        case PrimaryOS::Linux:   return "Linux";
        default:                 return "Unknown";
    }
}

std::vector<std::string> all_primary_os_strings() {
    return {"MacOS", "Windows", "Linux"};
}

// ------------------------------------------------------------
// StudyTime
// ------------------------------------------------------------

StudyTime parse_study_time(const std::string& s) {
    std::string val = lower(trim(s));

    if (val == "morning")   return StudyTime::Morning;
    if (val == "afternoon") return StudyTime::Afternoon;
    if (val == "night" || val == "evening") return StudyTime::Night;

    return StudyTime::Unknown;
}

std::string to_string(StudyTime st) {
    switch (st) {
        case StudyTime::Morning:   return "Morning";
        case StudyTime::Afternoon: return "Afternoon";
        case StudyTime::Night:     return "Night";
        default:                   return "Unknown";
    }
}

std::vector<std::string> all_study_time_strings() {
    return {"Morning", "Afternoon", "Night"};
}

// ------------------------------------------------------------
// Region
// ------------------------------------------------------------

struct RegionMapping {
    const char* name;
    Region region;
};

static const RegionMapping REGION_TABLE[] = {
    {"us-northeast", Region::US_Northeast},
    {"us-southeast", Region::US_Southeast},
    {"us-midwest", Region::US_Midwest},
    {"us-southwest", Region::US_Southwest},
    {"us-west", Region::US_West},
    {"canada-east", Region::Canada_East},
    {"canada-west", Region::Canada_West},
    {"mexico-central", Region::Mexico_Central},
    {"mexico-north", Region::Mexico_North},
    {"mexico-south", Region::Mexico_South},
    {"central-america", Region::Central_America},
    {"caribbean", Region::Caribbean},
    {"south-america-north", Region::South_America_North},
    {"south-america-south", Region::South_America_South},
    {"uk-ireland", Region::UK_Ireland},
    {"iberia", Region::Iberia},
    {"france-benelux", Region::France_Benelux},
    {"dach", Region::DACH},
    {"nordics", Region::Nordics},
    {"central-europe", Region::Central_Europe},
    {"eastern-europe", Region::Eastern_Europe},
    {"balkans", Region::Balkans},
    {"baltics", Region::Baltics},
    {"italy-malta", Region::Italy_Malta},
    {"greece-cyprus", Region::Greece_Cyprus},
    {"middle-east", Region::Middle_East},
    {"north-africa", Region::North_Africa},
    {"east-africa", Region::East_Africa},
    {"west-africa", Region::West_Africa},
    {"central-africa", Region::Central_Africa},
    {"southern-africa", Region::Southern_Africa},
    {"china", Region::China},
    {"japan", Region::Japan},
    {"korea", Region::Korea},
    {"southeast-asia-mainland", Region::Southeast_Asia_Mainland},
    {"southeast-asia-islands", Region::Southeast_Asia_Islands},
    {"south-asia-india", Region::South_Asia_India},
    {"south-asia-pakistan-bangladesh-srilanka", Region::South_Asia_Pakistan_Bangladesh_SriLanka},
    {"central-asia", Region::Central_Asia},
    {"australia", Region::Australia},
    {"new-zealand", Region::New_Zealand},
    {"pacific-islands", Region::Pacific_Islands},
    {"other", Region::Other},
    {"unknown", Region::Unknown}
};

Region parse_region(const std::string& s) {
    std::string val = lower(trim(s));

    for (const auto& entry : REGION_TABLE) {
        if (val == entry.name)
            return entry.region;
    }
    return Region::Unknown;
}

std::string to_string(Region r) {
    for (const auto& entry : REGION_TABLE) {
        if (entry.region == r)
            return entry.name;
    }
    return "unknown";
}

std::vector<std::string> all_region_strings() {
    std::vector<std::string> regions;
    regions.reserve(std::size(REGION_TABLE));
    for (const auto& entry : REGION_TABLE) {
        regions.push_back(entry.name);
    }
    return regions;
}

// ------------------------------------------------------------
// EngineeringFocus
// ------------------------------------------------------------

// Table-based mapping (easy to extend)
struct FocusMapping {
    const char* name;
    EngineeringFocus focus;
};

static const FocusMapping FOCUS_TABLE[] = {
    // Computer Engineering
    {"computer_systems", EngineeringFocus::Computer_Systems},
    {"embedded_systems", EngineeringFocus::Embedded_Systems},
    {"cybersecurity", EngineeringFocus::Cybersecurity},
    {"networking", EngineeringFocus::Networking},
    {"robotics_ce", EngineeringFocus::Robotics_CE},

    // Electrical Engineering
    {"power_systems", EngineeringFocus::Power_Systems},
    {"signal_processing", EngineeringFocus::Signal_Processing},
    {"electronics", EngineeringFocus::Electronics},
    {"telecommunications", EngineeringFocus::Telecommunications},
    {"control_systems", EngineeringFocus::Control_Systems},

    // Biomedical Engineering
    {"medical_devices", EngineeringFocus::Medical_Devices},
    {"bioinformatics", EngineeringFocus::Bioinformatics},
    {"biomechanics", EngineeringFocus::Biomechanics},
    {"neural_engineering", EngineeringFocus::Neural_Engineering},
    {"imaging_systems", EngineeringFocus::Imaging_Systems},

    // Mechanical Engineering
    {"thermodynamics", EngineeringFocus::Thermodynamics},
    {"fluid_mechanics", EngineeringFocus::Fluid_Mechanics},
    {"manufacturing", EngineeringFocus::Manufacturing},
    {"dynamics", EngineeringFocus::Dynamics},
    {"materials_mechanics", EngineeringFocus::Materials_Mechanics},

    // Civil & Environmental Engineering
    {"structural", EngineeringFocus::Structural},
    {"transportation", EngineeringFocus::Transportation},
    {"environmental", EngineeringFocus::Environmental},
    {"geotechnical", EngineeringFocus::Geotechnical},
    {"water_resources", EngineeringFocus::Water_Resources},

    // Chemical Engineering
    {"process_design", EngineeringFocus::Process_Design},
    {"materials_cheme", EngineeringFocus::Materials_ChemE},
    {"energy_systems", EngineeringFocus::Energy_Systems},
    {"biochemical", EngineeringFocus::Biochemical},
    {"nanotechnology", EngineeringFocus::Nanotechnology},

    // Other / Unknown
    {"other", EngineeringFocus::Other},
    {"unknown", EngineeringFocus::Unknown}
};

// ------------------------------------------------------------
// Parse a string into an EngineeringFocus enum
// ------------------------------------------------------------
EngineeringFocus parse_engineering_focus(const std::string& s) {
    std::string val = lower(trim(s));

    for (const auto& entry : FOCUS_TABLE) {
        if (val == entry.name)
            return entry.focus;
    }

    return EngineeringFocus::Unknown;
}

// ------------------------------------------------------------
// Convert an EngineeringFocus enum to a string
// ------------------------------------------------------------
std::string to_string(EngineeringFocus f) {
    for (const auto& entry : FOCUS_TABLE) {
        if (entry.focus == f)
            return entry.name;
    }
    return "unknown";
}

// ------------------------------------------------------------
// Return all available focus strings (for menus or validation)
// ------------------------------------------------------------
std::vector<std::string> all_engineering_focus_strings() {
    std::vector<std::string> list;
    list.reserve(std::size(FOCUS_TABLE));

    for (const auto& entry : FOCUS_TABLE) {
        list.push_back(entry.name);
    }
    return list;
}
#ifndef PERSON_ENUMS_H
#define PERSON_ENUMS_H

#include <string>
#include <vector>

enum class PrimaryOS {
    MacOS,
    Windows,
    Linux,
    Unknown
};

enum class StudyTime {
    Morning,
    Afternoon,
    Night,
    Unknown
};

enum class Region {
    // North America
    US_Northeast, US_Southeast, US_Midwest, US_Southwest, US_West,
    Canada_East, Canada_West,
    Mexico_Central, Mexico_North, Mexico_South,

    // Latin America & Caribbean
    Central_America, Caribbean, South_America_North, South_America_South,

    // Europe
    UK_Ireland, Iberia, France_Benelux, DACH, Nordics, Central_Europe,
    Eastern_Europe, Balkans, Baltics, Italy_Malta, Greece_Cyprus,

    // Middle East & North Africa
    Middle_East, North_Africa,

    // Sub-Saharan Africa
    East_Africa, West_Africa, Central_Africa, Southern_Africa,

    // Asia
    China, Japan, Korea, Southeast_Asia_Mainland, Southeast_Asia_Islands,
    South_Asia_India, South_Asia_Pakistan_Bangladesh_SriLanka, Central_Asia,

    // Oceania
    Australia, New_Zealand, Pacific_Islands,

    // Other / Unknown
    Other, Unknown
};

enum class EngineeringFocus {

    // ---- Computer Engineering ----
    Computer_Systems,
    Embedded_Systems,
    Cybersecurity,
    Networking,
    Robotics_CE,

    // ---- Electrical Engineering ----
    Power_Systems,
    Signal_Processing,
    Electronics,
    Telecommunications,
    Control_Systems,

    // ---- Biomedical Engineering ----
    Medical_Devices,
    Bioinformatics,
    Biomechanics,
    Neural_Engineering,
    Imaging_Systems,

    // ---- Mechanical Engineering ----
    Thermodynamics,
    Fluid_Mechanics,
    Manufacturing,
    Dynamics,
    Materials_Mechanics,

    // ---- Civil & Environmental Engineering ----
    Structural,
    Transportation,
    Environmental,
    Geotechnical,
    Water_Resources,

    // ---- Chemical Engineering ----
    Process_Design,
    Materials_ChemE,
    Energy_Systems,
    Biochemical,
    Nanotechnology,

    // ---- Other / Unknown ----
    Other,
    Unknown
};

PrimaryOS  parse_primary_os(const std::string& s);
std::string to_string(PrimaryOS os);

StudyTime  parse_study_time(const std::string& s);
std::string to_string(StudyTime st);

Region     parse_region(const std::string& s);
std::string to_string(Region r);

EngineeringFocus    parse_engineering_focus(const std::string& s);
std::string         to_string(EngineeringFocus f);

std::vector<std::string> all_primary_os_strings();
std::vector<std::string> all_study_time_strings();
std::vector<std::string> all_region_strings();
std::vector<std::string> all_engineering_focus_strings();

#endif // PERSON_ENUMS_H
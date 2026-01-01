# Insight Finder (C++)

## About 
Insight Finder is a C++17, CLI-first data exploration tool that generates natural-language insights from structured datasets. It analyzes correlations across multiple attributes (e.g., OS, study time, region, language, hobbies) and scores each insight based on confidence and coverage.

The project supports CSV and JSON data sources, custom and automatic discovery modes, and heat-map–based correlation analysis. A Qt-based GUI is included as an experimental prototype, while the CLI provides the complete, stable feature set.

## How to Build and Run

### CLI Version

**Compile:**
```bash
cd /Users/kw/EC327/decoderscpp
g++ -std=c++17 -I src src/*.cpp -lcurl -o cli_app
```

**Run:**
```bash
cd data
../cli_app
```

### Qt GUI Version

**Prerequisites:**
```bash
brew list qt6 || brew install qt6
brew list cmake || brew install cmake
```

**Build:**
```bash
cd .../decoderscpp/InsightFinderProject
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
make
```

**Run:**
```bash
./InsightFinderProject
```

### Quick Start (CLI):
```bash
# Inside the program:
> load class_data_set.csv    # Load from local CSV
> generate                   # Generate insights
> list-insights              # View results
> save 0 1 2                 # Save useful insights
```

## All Commands

### Data Loading

`load <file.csv>` | Load data from local CSV file 
`load-json` | Load from default JSON URL 
`load-json-custom <url>` | Load from any JSON URL you provide 

### Data Persistence
`save-dataset` | Save changes to current file 
`save-as <file.csv>` | Save data to a new file 

### Person Management
`list` | Show all loaded people 
`add` | Add a new person 
`edit <index>` | Edit a person 
`remove <index>` | Remove a person 

### Insight Generation
`generate` | Auto-generate insights (4 default topics) 
`generate-custom <a> <b>` | Generate insights for custom topic pair 
`discover-best` | 6x6 heat map (36 cells, 15 pairs) 
`discover-all` | 9x9 heat map (81 cells, 36 pairs) 

### Insight Management
`list-insights` | Show generated insights 
`save <indexes> [file]` | Save useful insights 
`discard <indexes> [file]` | Block unwanted insights 
`list-saved` | Show saved insights 

## Data Loading Details

### CSV Loading
```
> load class_data_set.csv
Loaded 46 people.
```

### JSON Loading (Default URL)
```
> load-json
Loaded 63 people from JSON.
```

### JSON Loading (Custom URL)
```
> load-json-custom http://example.com/mydata.json
Loaded 50 people from custom JSON URL.
```

## Insight-driven sentences

- Load data from CSV file or JSON URL into memory.
- Trigger generation of English sentences that describe trends in the data.
- Review insights sorted by their quality score (0–100).
- Will be able to mark insights as useful (saved for later) or discard them so they are not regenerated for the same dataset.
- The Insight Engine is implemented by the InsightGenerator class, which analyzes the Person dataset and produces Insight objects with English sentences and statistics. 

Example sentences produced by the `InsightGenerator`:

- `People whose primary OS is MacOS tend to study in the mornings.`
- `People whose favorite color is blue tend to have a hobby of reading.`

Useful insights persist in-memory for the current run and can be revisited from the main menu.

## Generation Modes

### Auto-Generate (`generate` or `generate-auto`)
Generates insights from all 4 default topic pairs:
- **Primary OS → Study Time**: Discovers when people with different operating systems prefer to study
- **Favorite Color → Hobby**: Correlations between color preferences and hobbies
- **Region → Language**: Identifies language patterns by geographic region
- **Engineering Focus → Course Load**: Reveals typical course loads for different engineering specializations

This mode scans your entire dataset and produces all insights that meet quality thresholds (minimum support and confidence levels).

### Custom-Generate (`generate-custom <attr1> <attr2>`)
Generates insights for **ANY** attribute pair chosen by the user. Supported attributes:
- `os` (or: primary_os, primaryos)
- `study` (or: studytime, study_time)
- `color` (or: favoritecolor, favoritecolors)
- `hobby` (or: hobbies)
- `region` (or: area)
- `language` (or: lang, languages)
- `focus` (or: major, engineering, engineeringfocus)
- `course` (or: courseload, load, courses)
- `graduation` (or: gradyear, year)

This mode allows targeted exploration of **any relationship** in your data. Examples:
- `generate-custom os hobby` → "Linux users tend to enjoy gym"
- `generate-custom color language` → "People who like blue tend to speak English"
- `generate-custom study course` → "Night studiers tend to take 5 courses"
- `generate-custom region hobby` → "People from China tend to enjoy gaming"

The command accepts flexible inputs, both order and synonym variations are accepted (e.g., "os study" and "study os" are equivalent).

### Dynamic Insight Discovery
The number of insights generated is not fixed. It depends on:
- Patterns discovered in the dataset
- Quality thresholds (minimum 2-3 people per group)
- Previously blocked insights (user can discard unwanted insights)

Each insight includes:
- **Description**: Natural language sentence explaining the pattern
- **Score**: Quality metric (0-100) based on confidence and coverage
- **Support**: Number of people matching the pattern
- **Key**: Unique identifier for saving/blocking

### Total Possible Combinations
There are 9 supported topics, the algo can generate insights for **36 unique pairs** (9 choose 2), including:
- Traditional pairs: os→study, color→hobby, region→language, focus→course
- Custom combinations: os→hobby, color→language, study→course, region→hobby, and many more

## 🌟 Creative Feature: Heat Map Discovery

### Two Heat Map Options

| Command | Matrix | Cells | Unique Pairs | Attributes |
|---------|--------|-------|--------------|------------|
| `discover-best` | 6×6 | 36 | 15 | os, study, color, hobby, region, language |
| `discover-all` | 9×9 | 81 | 36 | All 9 attributes |

### `discover-best` (6x6 Heat Map)
Analyzes 6 core attributes and shows strongest relationships.

```
> discover-best
6x6 Heat Map (36 cells, 15 unique pairs)

TOP 10 STRONGEST RELATIONSHIPS:
1. os ←→ language
   Avg Score: 72/100  ##############
   Insights Found: 3
```

### `discover-all` (9x9 Heat Map)
Full analysis including focus, course, and graduation attributes.

```
> discover-all
9x9 Heat Map (81 cells, 36 unique pairs)

TOP 10 STRONGEST RELATIONSHIPS:
1. os <-> language
   Avg Score: 72/100  ##############
   Insights Found: 3
```

### Color-Coded Matrix Output
Both commands display correlation matrices with color-coded scores:
- 🟡 Yellow: < 50 (weak)
- 🟠 Orange: 50-65 (moderate)
- 🔴 Red: > 65 (strong)

**Use case:** 
Instead of manually testing combinations, these commands reveal which attribute relationships are most meaningful in your dataset.

### Example Workflow
```
> load class_data_set.csv
Loaded 46 people.

> generate
Generated 17 insights.

> generate-custom os hobby
Generated 1 matching insights.

> list-insights
0) [Score 36] People whose primary OS is Linux tend to have hobby of gym.

> generate-custom color language
Generated 13 matching insights.

> save 0 1 2
> discard 5
```

## Data Persistence

Save and reload your data between sessions.

### Example:
```
> load class_data_set.csv     # Load and remember this file
Loaded 46 people.

> remove 5                     # Make changes
Person removed.

> save-dataset                 # Save back to same file
Dataset saved to: class_data_set.csv

> save-as backup.csv           # Or save to new file
Dataset saved to: backup.csv
```

### How It Works:
- `load` remembers which file you loaded
- `save-dataset` saves changes back to that file
- `save-as` saves to a new file
- Changes stay in memory until you save

## Tested Commands with Sample Output

All commands below have been tested and verified.

### Data Loading
```
> load class_data_set.csv
Loaded 46 people.

> load-json
Fetching JSON from default URL...
Loaded 63 people from JSON.

> load-json-custom http://example.com/data.json
Fetching JSON from custom URL...
Loaded 63 people from custom JSON URL.
```

### Person Management
```
> add
=== Add New Person ===
ID (nickname or identifier): new_person
Graduation Year (e.g., 2025): 2025
Region: us-west
Primary OS: Linux
Engineering Focus: cybersecurity
Study Time: Night
Course Load: 5
Favorite Colors: blue,red
Hobbies: gaming,coding
Languages: english
Person added! Total: 47 people.

> edit 0
=== Editing Person 0 ===
Press Enter to keep current value.
ID [ed]: 
...
Person updated!

> remove 0
Person removed.
```

### Insight Generation
```
> generate
Generating insights automatically...
Generated 14 insights.

> generate-custom os hobby
Generating insights matching 'os' and 'hobby'...
Generated 1 matching insights.

> list-insights
0) [Score 83] People from us-northeast tend to speak english.
1) [Score 71] People whose favorite color is sage tend to have a hobby of reading.
...
```

### Insight Management
```
> save 0 1 2 my_insights.csv
Saved insight: People from us-northeast tend to speak english.
Saved insight: People whose favorite color is sage tend to have a hobby of reading.
Saved insight: People whose primary OS is MacOS tend to study in the nights.
Saved to: my_insights.csv

> discard 0 1 blocked.txt
Discarded insight: ...
Blocked keys saved to: blocked.txt

> list-saved
0) People from us-northeast tend to speak english. (score=83)
...
```

### Data Persistence
```
> save-dataset
Dataset saved to: class_data_set.csv

> save-as backup.csv
Dataset saved to: backup.csv
```

### Creative Feature - Heat Maps
```
> discover-best
6x6 Heat Map (36 cells, 15 unique pairs)
TOP 10 STRONGEST RELATIONSHIPS:
1. os ←→ language
   Avg Score: 72/100  ##############
   Insights Found: 3

> discover-all
9x9 Heat Map (81 cells, 36 unique pairs)
TOP 10 STRONGEST RELATIONSHIPS:
1. os <-> language
   Avg Score: 72/100  ##############
   Insights Found: 3
```

## Project Architecture

| File | Purpose |
|------|---------|
| `main.cpp` | Entry point |
| `Cli.cpp/h` | Command-line interface |
| `Person.cpp/h` | Data model (immutable) |
| `PersonBuilder.cpp/h` | Builds Person objects |
| `PersonRepository.cpp/h` | Stores/manages persons |
| `PersonCsvReader.cpp/h` | Reads CSV files |
| `PersonJsonReader.cpp/h` | Fetches JSON from URLs |
| `InsightGenerator.cpp/h` | Generates insights |
| `InsightStore.cpp/h` | Manages saved insights |
| `PersonEnums.cpp/h` | Enums for Region, OS, etc. |
| `InsightFinderProject/` | Qt GUI application |

# Online Complaint and Grievance Tracking System

**Course:** CAP455 - Object-Oriented Programming using C++  
**Project:** 72 - Online Complaint and Grievance Tracking System  
**Assessment phase:** CA2 - Persist and Search

A menu-driven C++ console application for lodging, viewing, searching, and updating citizen grievances. Records are stored in the local binary file `grievances.dat` so they can be loaded again after the application closes.

## Features

- Lodge a grievance with an ID, citizen name, department, category, and description.
- Prevent duplicate active grievance IDs.
- Save fixed-size grievance records to `grievances.dat` in binary append mode.
- View all active records using sequential binary reads.
- Search by grievance ID and use `seekg` to read the matching record at its byte offset.
- Update the status and administrative remarks in place using `seekp`.
- Demonstrate default, parameterized, and copy constructors, a destructor, fixed-size character arrays, and file streams.

## Project files

Save the pasted C++ program as `main.cpp`. The program creates `grievances.dat` in its current working directory when the first grievance is saved.

## Build and run in VS Code

1. Open the project folder in VS Code and save the pasted code as `main.cpp`.
2. Add this header near the other includes because the program uses `cout` and `cin`:

   ```cpp
   #include <iostream>
   ```

3. Open the VS Code terminal in the folder containing `main.cpp` and compile with g++:

   ```powershell
   g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o grievance_tracker.exe
   ```

4. Run the program:

   ```powershell
   .\grievance_tracker.exe
   ```

## Menu

| Choice | Action |
| --- | --- |
| 1 | Lodge a new grievance |
| 2 | View all persisted grievances |
| 3 | Track a grievance by ID using `seekg` |
| 4 | Update status and administrative remarks using `seekp` |
| 5 | Exit |

## CA2 explanation points

- `findRecordIndex` scans the fixed-size binary records and returns the zero-based record index.
- The byte offset for a record is `record index * sizeof(Grievance)`.
- `seekg` moves the input position to read a record; `seekp` moves the output position before the updated record is written.
- The copy constructor is used to make a copy before saving a new grievance.
- `grievances.dat` must stay with the program's working directory for saved records to remain available.

## Testing evidence to add

Before submission, run the program and capture your own evidence for:

1. Adding a grievance and seeing the success message.
2. Viewing the saved record.
3. Searching for an existing ID and a missing ID.
4. Updating the status/remarks and confirming the changes after restarting.
5. Rejecting a duplicate ID.

Fill in actual test results and screenshots in your project report. Do not claim a test passed until you have run it.

## Technical note

This pasted implementation writes the `Grievance` object representation directly to a binary file. Raw object-byte persistence is compiler/platform dependent and should only be used for a same-build classroom demonstration. For portable, standards-safe storage, serialize each field explicitly or define a separate trivially copyable disk-record structure. Also check stream state after writes in a final submission version.

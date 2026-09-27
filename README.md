# Hotel Reservation & Rating Management System (C++)

A C++ application designed to manage hotel bookings, guest information, multi-criteria feedback/ratings, and automated asynchronous notifications for low customer satisfaction scores.

---

## Features

* **Generic Dynamic Collection (`Kolekcija<T1, T2>`)**: Custom template container handling pair elements with optional duplicate checking, dynamic resizing, and index insertion (`InsertAt`).
* **Passport Validation (`ValidirajBrojPasosa`)**: Regular expression validation ensuring passport numbers comply with standard format rules (1–2 uppercase letters, 3–4 digits, optional separator, 2–4 digits).
* **Multi-Criteria Guest Reviews (`Komentar` & `Kriteriji`)**: Rating system supporting evaluation across custom criteria (*CISTOCA*, *USLUGA*, *LOKACIJA*, *UDOBNOST*) with automatic average score calculation.
* **Asynchronous Email Trigger (`std::thread` & `std::mutex`)**: Multi-threaded mechanism that triggers a simulated customer service follow-up email when two or more criteria receive ratings below 5/10.
* **Memory Management**: Custom deep-copy constructors, assignment operators, destructors, and smart pointers (`std::unique_ptr`).

---

## Class Architecture

```
                                +-------------------+
                                |    Rezervacija    |
                                +-------------------+
                                  |        |      |
                    +-------------+        |      +--------------+
                    | (1..*)               | (1)                 | (2)
                    v                      v                     v
                +------+             +----------+            +-------+
                | Gost |             | Komentar |            | Datum |
                +------+             +----------+            +-------+
                                           |
                                           v (1)
                             +---------------------------+
                             | Kolekcija<Kriteriji, int> |
                             +---------------------------+

```

### 1. `Kolekcija<T1, T2>`

A generic double-array dynamic collection supporting:

* Element duplication toggle (`_omoguciDupliranje`).
* Inserting items at arbitrary indices via `InsertAt(index, el1, el2)`.
* Deep copying and memory cleanup.

### 2. `Datum`

Handles dates (`day`, `month`, `year`) using dynamic memory allocation and includes date summary comparison methods.

### 3. `Gost`

Encapsulates guest details:

* `_imePrezime`: Managed via `std::unique_ptr<char[]>`.
* `_emailAdresa`: Guest contact string.
* `_brojPasosa`: Validated against regex `^[A-Z]{1,2}\d{3,4}[- ]?\d{2,4}$`. Sets to `"NOT VALID"` if validation fails.

### 4. `Komentar`

Stores textual guest feedback alongside a map-like `Kolekcija<Kriteriji, int>` of ratings (1–10 scale). Calculates overall mean scores and prevents duplicate category entries.

### 5. `Rezervacija`

Coordinates start/end dates, guest lists (`std::vector<Gost>`), and reservation comments. If poor feedback is posted, it dispatches an asynchronous thread (`std::thread`) to log customer service alerts without blocking the main execution path.

---

## Getting Started

### Prerequisites

* **Compiler**: MSVC (Visual Studio 2019/2022 recommended) or GCC/Clang with C++11 or higher.
*Note: The codebase uses MSVC-specific C-string functions (`strcpy_s`). For GCC/Clang, standard replacements (`strncpy` or `strcpy`) may be required.*

### Building and Running

#### Microsoft Visual Studio (MSVC)

1. Open Visual Studio and create a new **C++ Console Application** project.
2. Replace the contents of `main.cpp` with the code.
3. Build (`Ctrl + Shift + B`) and Run (`Ctrl + F5`).

#### Command Line (MSVC `cl.exe`)

```bash
cl /EHsc /std:c++14 main.cpp /Fe:ReservationSystem.exe
./ReservationSystem.exe

```

---

## Usage Example Output

When running `main()`, the program executes the following flow:

```text
0 0
1 1
2 2
...
Dupliciranje nije dozvoljeno!
-------------------------------------------
Broj pasosa validan
Broj pasosa validan

-------------------------------------------
Rezervacija 19.6.2022 - 20.6.2022 za goste:
	1.Denis Music denis@fit.ba BH235-532
	2.Jasmin Azemovic jasmin@fit.ba B123321
-------------------------------------------
Komentar rezervacije: 
Nismo pretjerano zadovoljni uslugom, a ni lokacijom.
CISTOCA(7)
USLUGA(4)
LOKACIJA(3)
UDOBNOST(6)
Prosjecna ocjena -> 5
-------------------------------------------
To: denis@fit.ba;jasmin@fit.ba;
Subject: Informacija

Postovani,

Zaprimili smo Vase ocjene, a njihova prosjecna vrijednost je 5
Zao nam je zbog toga, te ce Vas u najkracem periodu kontaktirati nasa Sluzba za odnose sa gostima.
Ugodan boravak Vam zelimo
Puno pozdrava

```
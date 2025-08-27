# Vaccine Management System

A command-line C application for managing vaccine batches and their inoculations to users. The system tracks vaccine lots, their expiration dates, available doses, and manages the application of these doses to individuals.

## Overview

This project aims to implement a vaccine and inoculation management system in C. The program allows:

- Registering new vaccine batches
- Tracking doses and expiration dates
- Applying vaccine doses to users
- Listing and deleting inoculation records
- Simulating time progression

Interaction is performed through commands entered via standard input. Each line starts with a single-character command, followed by the required arguments.

## Features

- Track up to 1000 vaccine batches
- Unlimited inoculations (memory-efficient)
- Supports quoted user names (e.g., "John Doe")
- Handles date validation and expiration
- Simulated date control (starts at 01-01-2025)
- Fully localized error messages (EN or PT via CLI arg)
- Full dynamic memory management (no leaks)
- Compliant with restricted C libraries and coding standards

## Getting Started

### Build

Compile with a standard C compiler (GCC):

```bash
gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c
```

### Run

```bash
./proj           # for English messages
./proj pt        # for Portuguese messages
```

## Input Format

Each command is entered via standard input as:

```bash
<command> [arg1] [arg2] ...
```

Arguments are separated by at least one space. Strings with whitespace must be enclosed in quotes.

## Commands

| Command | Action |
|:-------:|--------|
| `q`     | Quit the program |
| `c`     | Create a new vaccine batch |
| `l`     | List available vaccines |
| `a`     | Apply a vaccine to a user |
| `r`     | Remove availability of a batch |
| `d`     | Delete user vaccination records |
| `u`     | List all or user-specific applications |
| `t`     | Advance simulated date |

## Command Details

### `q` – Quit
```
q
```
Ends the program. No output.

### `c` – Create new vaccine batch
```
c <batch> <dd-mm-yyyy> <doses> <vaccine-name>
```
Adds a new batch.

**Errors**:
- `too many vaccines`
- `duplicate batch number`
- `invalid batch`
- `invalid name`
- `invalid date`
- `invalid quantity`

### `l` – List vaccines
```
l [<vaccine-name> {<vaccine-name>}]
```
Prints batches in chronological order by expiry date. Can be filtered by vaccine name(s).

**Errors**:
- `<name>: no such vaccine`

### `a` – Apply a dose
```
a <user-name> <vaccine-name>
```
Applies the earliest valid batch to the user.

**Errors**:
- `no stock`
- `already vaccinated`

### `r` – Remove batch availability
```
r <batch>
```
Removes batch if no applications exist; otherwise, sets available doses to 0.

**Errors**:
- `<batch>: no such batch`

### `d` – Delete vaccination records
```
d <user-name> [<dd-mm-yyyy> [<batch>]]
```
Deletes:
- All user records (1 arg)
- All user records on given date (2 args)
- Specific inoculation (3 args)

**Errors**:
- `<user>: no such user`
- `invalid date`
- `<batch>: no such batch`

### `u` – List applications
```
u [<user-name>]
```
Lists all inoculations or those for a specific user.

**Errors**:
- `<user>: no such user`

### `t` – Advance system date
```
t [<dd-mm-yyyy>]
```
If no date is provided, prints current system date.

**Errors**:
- `invalid date` (e.g., before current date)

## Localization

If run with the `pt` argument:

```bash
./proj pt
```

All error messages are printed in Portuguese:

```
demasiadas vacinas, número de lote duplicado, lote inválido, nome inválido, data inválida, quantidade inválida, vacina inexistente, esgotado, já vacinado, lote inexistente, utente inexistente, sem memória.
```

## Example Commands

### Register a batch

```bash
c A0C0 31-7-2025 210 malaria
```

### List batches

```bash
l
l tetanus malaria
```

### Apply a dose

```bash
a "João Miguel" tetanus
```

### Remove batch

```bash
r A0C0
```

### Delete inoculations

```bash
d xico
d xico 1-1-2025
d xico 1-1-2025 FA54
```

### List applications

```bash
u
u "João Miguel"
```

### Advance date

```bash
t
t 2-2-2025
```

## Constraints

- No global variables
- Memory must be freed before termination
- Use only the following C libraries:
  - `stdio.h`
  - `stdlib.h`
  - `string.h`
  - `ctype.h`

## Simulated Time

- Starts at `01-01-2025`
- All expiry dates are compared to current simulated date
- Invalid to move system date backward

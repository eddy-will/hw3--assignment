# C List

**INSY 8212: Programming with C**. **Due Friday 04 December 2026, 23:59 (Africa/Kigali time).**

## Overview

In this assignment you turn the linked list we built in class into a small **library**: nine functions that create and change a singly linked list of ints. The full instructions, with examples and diagrams for every function, are in `README.md` in the template repository. Read it, and read `c_list.h`, before writing any code.

**Files.** `c_list.h` declares the two structs and the nine functions, with a comment describing exactly what each one must do. Do not edit it: the grader uses its own copy. You write the functions in `c_list.c`, which must begin with `#include "c_list.h"`. Test them from your own `main.c`. Build with `make` and run with `./c_list`. Submit `c_list.c` only.

**The functions and their points:**

- Level 1: `list_create` (5), `list_destroy` (8), `list_push_front` (8), `list_push_back` (9)
- Level 2: `list_get` (9), `list_pop_front` (11)
- Level 3: `list_insert_at` (16), `list_remove_at` (16), `list_reverse` (18)

**Rules.**

- Do not change any struct, function name, return type, or parameter.
- Inside `c_list.c`, the only library functions allowed are `malloc` and `free`.
- Mark your helper functions `static`.
- Any compiler warning counts as a compile failure. The `Makefile` uses `-Werror`, so if it builds for you, it builds for the grader.
- Free every node you remove, exactly once, and never use a node after freeing it.
- `list->size` must always equal the number of nodes.
- Functions returning `int` return `0` on success and `-1` on failure. On failure, nothing changes: not the list, and not `*out`.

**How it's graded.** For most tests the grader builds the starting list itself, so a bug in one function doesn't fail the others. It checks the whole list after every step, and it counts every `malloc` and `free`: a leak, a double free, or using freed memory fails the test it happens in. Every test is visible in your feedback.

**Getting help.** You may ask me, classmates, the internet, or an AI tool, as long as you can explain and reason about every line you submit. Expect to be asked to draw what a list looks like after one of these functions runs.

## Input

Your functions read no input. The grader calls them directly.

## Output

Your functions print nothing. Printing inside `c_list.c` counts as a forbidden library call. You may print freely in your own `main.c`.

## Invalid input and edge cases

Every edge case (empty list, one node, first and last positions, invalid indexes) is defined in `c_list.h` and in the README. Follow them exactly.

## What you may and may not use

**Allowed:**

- `malloc` and `free` (from `stdlib.h`)

**Not allowed:**

- Every other library function inside `c_list.c`, including `printf`, `calloc`, `realloc` and `memcpy`
- Editing `c_list.h`
- `goto`

## Getting started

1. Open the template repository (https://github.com/Kabera192/c_list_template.git) and click **Use this template > Create a new repository**. Make it **private** (a public repo lets anyone copy your work).
2. Clone your new repository to your computer and work there. Commit and push as often as you like; this is for your own version control and is **not** your submission.
3. Write your code in `c_list.c`.

## Compiling

Your code is graded with exactly this command (gcc on Linux):

```
gcc -std=c11 -Wall -Wextra -g -Werror driver.c -o c_list
```

Or just type `make`. **Warnings count as errors:** if gcc prints any warning, your program does not compile and every test scores 0. Fix every warning before you submit.

## Submitting

**Only the Google Form counts.** Pushing to GitHub is not a submission.

1. Make a zip file named `c_list_STUDENTID.zip`, replacing STUDENTID with your numeric student ID, containing `c_list.c`. The easiest way: `make zip ID=12345` or `python3 tests/make_zip.py 12345` (with your ID). This also checks that your code compiles.
2. To zip by hand instead: Windows: select the file(s), right-click > Send to > Compressed (zipped) folder. macOS: select the file(s), right-click > Compress. Linux/WSL: `zip c_list_STUDENTID.zip c_list.c`. Zip the files themselves, not a folder around them (a folder is tolerated, but not required).
3. Do not include compiled programs, and do not use .rar or .7z.
4. Open the submission form (https://forms.gle/Wkqw2vfQZwamEJBV6), sign in with your Google account, enter your full name and numeric student ID exactly, and upload the zip.
5. You may submit again as many times as you like (see the late policy below for how we choose which one to grade).

## How your grade is computed

| Category | Points | How it is graded |
|---|---|---|
| Compiles cleanly (no warnings) | 0 | Compiles with the command above with no warnings |
| list_create | 5 | 1 visible test(s); partial credit per test |
| list_destroy | 8 | 5 visible test(s); partial credit per test |
| list_push_front | 8 | 5 visible test(s); partial credit per test |
| list_push_back | 9 | 6 visible test(s); partial credit per test |
| list_get | 9 | 8 visible test(s); partial credit per test |
| list_pop_front | 11 | 5 visible test(s); partial credit per test |
| list_insert_at | 16 | 10 visible test(s); partial credit per test |
| list_remove_at | 16 | 9 visible test(s); partial credit per test |
| list_reverse | 18 | 7 visible test(s); partial credit per test |
| Total | 100 |  |

If your program does not compile, all test-based categories score 0. Within a category, tests can carry different weights. The final score is rounded to the nearest 0.5. You will receive a feedback report by email listing every test, what went wrong, and your points per category.

## Deadline and late submissions

Deadline: Friday 04 December 2026, 23:59 Africa/Kigali time (UTC+02:00; = 2026-12-04 21:59 UTC).

Grace period: submissions up to 15 minutes after the deadline count as on time. After that, lateness is measured from the deadline itself.

Late penalty: 10% of the score you earned per day late. Any part of a day counts as a full day.

Submissions more than 3 day(s) late receive 0.

You may submit as many times as you like. We grade your latest on-time submission AND your latest submission overall (with its late penalty), and you receive whichever final score is higher.

| Submitted after the deadline by | Penalty |
|---|---|
| 10 minutes | 0 (on time) |
| 3 hours | -10% |
| 1 day 1 hour | -20% |
| 2.5 days | -30% |
| 4 days | score = 0 |

If you need an extension, ask the instructor before the deadline.

## Academic integrity

This is an individual assignment. You may discuss ideas with classmates, but the code you submit must be written by you. All submissions are compared with each other by software, and unusually similar pairs are reviewed by the instructor. Similar code alone is never treated as proof of copying; you will always be asked to explain your work first.

# Virtual Memory Management Unit (MMU) — C

**Course:** CST 334 — Operating Systems  
**Project:** Programming Assignment 4, *Let's Build an MMU!*  
**Language:** C

## Overview

This course project implements key functions for a simulated memory management unit (MMU). My implementation in `student_code.c` handles page-table entry flags, physical-frame selection, virtual-to-physical page mapping, permission checks, and byte-level memory access.

The project uses instructor-provided interfaces and supporting infrastructure; the code described below is my implementation within that framework.

## What I implemented

- **Page-table flags:** Test the valid, read, write, and execute bits using bit masks.
- **Address translation helpers:** Extract a virtual page number from a virtual address and a physical frame number from a page-table entry.
- **Frame allocation:** Scan the frame-usage array for the first available physical frame.
- **Page mapping:** Mark a frame in use and construct a page-table entry with requested access permissions.
- **Access validation:** Reject unmapped pages or access requests that lack the required permissions.
- **Byte-level memory operations:** Read and write data at the offset within a mapped physical page.

## Implementation details

The page-table entry uses the high-order bits as validity and access flags. A mask extracts the physical frame number from the remaining lower bits. Address translation uses the virtual page number to retrieve an entry, checks its permissions, and computes the corresponding physical-memory page address.

Invalid or disallowed reads return `0`; invalid or disallowed writes return without changing memory, following the assignment implementation.

## Source code

The key student-authored implementation is `src/student_code.c`. The original course archive also contains instructor-provided headers, tests, and build files. If publishing a runnable repository, include those materials only when course rules permit and label their origin.

## Testing and limitations

The assignment archive contains unit tests and a Makefile, but this README does not claim a passing test result. The provided build configuration references the Criterion test library and course-specific paths, so reproducing tests outside the course environment may require setup changes.

This is an educational MMU simulation, not a production operating-system memory manager.

## Skills demonstrated

C · Bitwise operations · Virtual memory · Page tables · Address translation · Access permissions · Memory allocation concepts

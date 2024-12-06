# Aims

- Understand routing protocols and route propagation.
- Implement a basic link state routing algorithm.
- Extend the basic implementation to use more efficient data structures (postgraduate task).

# Overview

In this assignment, you will simulate a network of routers that use a Link State (LS) routing protocol, similar to OSPF. Your program will:

1. Construct and maintain a network topology.
2. Use link state updates to maintain a synchronized Link-State Database (LSDB).
3. Compute shortest paths and routing tables using Dijkstra’s algorithm.

You will implement two versions:

- **Part 1: Basic Form (150 marks)**  
  Implement the LS routing protocol using Dijkstra’s algorithm with O(N²) complexity data structures.

- **Part 2 (Postgraduate Only): Advanced Form (50 marks)**  
  Modify your implementation to achieve O(N log N) complexity using appropriate data structures.

Finally, you will produce a brief report (1 page maximum), named `Comparison.PDF`, containing a complexity comparison and test results.

**Reference**:  
Dijkstra’s algorithm details can be found in the course notes and Kurose & Ross, Computer Networking, 7th Edition, Section 5.2.1.

---

# Task Details

## Part 1: Basic Form

- Program name: `Dijkstra`
- Read the initial topology from standard input.
- Use link state updates and Dijkstra’s algorithm to:
  - Update the topology (neighbor tables, LSDB).
  - Compute the routing tables.
- After each change, print the required tables (Neighbour, LSDB, Routing) for the requested routers.
- Continue reading updates until `END` is encountered.

## Part 2 (Postgraduate Only): Advanced Form

- Program name: `DijkstraNlogN`
- Similar to Part 1, but use data structures that achieve O(log N) complexity for relevant operations.
- Collect and compare performance data against the basic version.

## Report & Stress Test

- Provide:
  1. **Report** (`Comparison.PDF`):  
     - Present a graph and brief textual explanations to justify the complexity of both implementations.
     - Maximum 1 page in length.
  2. **Test network configurations** and **script** to run them.
     - The script should automate running both versions and produce summary runtime tables.
     - Use at least 4 data points (network sizes) to show performance differences.

**Marks are allocated based on:**
- Quality and correctness of the complexity argument (graph and explanations).
- Scripts and instructions to reproduce results.
- Proper runtime summaries in table format.

---

# Implementation Guidelines

- No code templates are provided. You must design your own data structures and logic.
- Document your development in a logbook (per course instructions).

**For PG students:**
- Provide 4 data points comparing `Dijkstra` and `DijkstraNlogN`.

---

# Programming Requirements

- Use a language of your choice.
- If compiled (C/C++, Java), provide a Makefile.  
- If interpreted (Python), ensure a proper shebang line and executable permissions.
- Must run on the provided environment (e.g., `uss.cs.adelaide.edu.au`).
- Allowed to use only standard libraries available on the system.

**Execution commands:**
- C/C++:
  ```bash
  make
  ./Dijkstra
  ./DijkstraNlogN
  ```
- Java:
  ```bash
  make
  java Dijkstra
  java DijkstraNlogN
  ```
- Python:
  ```bash
  ./Dijkstra
  ./DijkstraNlogN
  ```

---

# Algorithmic Details

**Key Assumptions:**
- All routers have synchronized LSDBs as soon as a link state change is read.
- Neighbor tables, LSDB, and routing tables are updated immediately after each input line describing a link.
- When ties occur (multiple shortest paths), choose the path where:
  - The next node in alphabetical order is selected first.
  - If multiple routes have the same cost, choose the least-hop route.

**Dijkstra’s Algorithm (Reference):**
```
Initialization:
  N' = {u}
  For all v:
    If v is a neighbor of u:
      D(v) = c(u,v)
    Else
      D(v) = ∞

Loop:
  Find w not in N' with the smallest D(w)
  Add w to N'
  For each neighbor v of w not in N':
    D(v) = min(D(v), D(w) + c(w,v))
Until N' = N
```

---

# Input Format

1. List of routers (one per line).
2. `LINKSTATE` keyword.
3. Lines describing each link:  
   `RouterA-RouterB cost [list_of_chosen_routers]`
   - `cost = -1` means remove the link.
4. After initial links, `UPDATE` keyword appears.
5. Lines describing link state changes, same format as above.
   - New routers can appear here and should be added dynamically.
6. Ends with `END` keyword.

**Process:**
- After reading the initial topology and each link state line, update the LSDB, run Dijkstra for all routers, and print tables for the chosen routers.
- After `END`, terminate normally.

---

# Output Format

For each requested router, print in alphabetical order by router name:

1. **Neighbour Table**:
   ```
   X Neighbour Table:
   Y|2
   Z|7

   ```
   - If empty:
     ```
     X Neighbour Table:
     ```
   - Blank line after the table.

2. **LSDB**:
   ```
   X LSDB:
   X|Y|2
   X|Z|7
   Y|Z|1

   ```
   - Links in alphabetical order.
   - If empty:
     ```
     Y LSDB:
     ```
   - Blank line after the table.

3. **Routing Table**:
   ```
   X Routing Table:
   Y|Y|2
   Z|Y|3

   ```
   - Destination in alphabetical order.
   - If empty:
     ```
     Y Routing Table:
     ```
   - Blank line after the table.

---

# Recommended Approach

1. Understand Dijkstra’s algorithm (textbook, notes, Wikipedia).
2. Manually work through a sample topology.
3. Plan data structures and parsing logic.
4. Implement incrementally, test, and debug.
5. Verify on the university servers.
6. Create additional test scenarios.
7. Produce final report and scripts.

---

# Submission & Testing

- Commit all code, Makefile, scripts, and the `Comparison.PDF` report to SVN.
- Test using input redirection and compare outputs.
- Ensure correctness and compliance with specified input/output formats.
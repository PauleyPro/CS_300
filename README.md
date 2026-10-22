# CS 300: Data Structures and Algorithms

## Course Overview

CS 300 focused on understanding how data structures and algorithms influence software performance, efficiency, and maintainability. Throughout the course, I explored vectors, hash tables, and binary search trees, examining how each structure stores, retrieves, and organizes information.

The course concluded with two projects centered on developing an advising assistance program for a university's Computer Science department. Project One evaluated the runtime and memory requirements of different data structures, while Project Two applied those findings to develop a working C++ application.

## Portfolio Artifacts

### Project One: Data Structure Analysis and Design

**[View Complete Project One Document](./CS%20300%20Project%20%286%29.docx)**

**Runtime Analysis:** Begins on page 18 of the document (PDF page numbering may differ).

Project One documents the design and evaluation of three data structures: a vector, hash table, and binary search tree. Each structure was evaluated against the same academic advising requirements, including loading course data, validating prerequisites, retrieving individual courses, and printing courses in alphanumeric order.

The complete document includes pseudocode, runtime analysis, memory considerations, implementation tradeoffs, and a final recommendation. I selected the hash table because it offers the strongest overall average performance for the program's primary operations, particularly course retrieval and prerequisite validation.

### Project Two: ABCU Advising Assistance Program

**[View C++ Source Code](./ProjectTwo%282%29.cpp)**

Project Two translates the analysis and design work into a functional C++ application. The program uses a custom hash table with separate chaining to store course information, validate prerequisites, search for individual courses, and display the complete course list in alphanumeric order.

Beyond the required functionality, the application includes input validation, duplicate detection, error handling, and an incremental testing framework. These additions demonstrate my approach to building reliable software through modular development and systematic testing.

## Project Reflection

### What was the problem you were solving in the projects for this course?

The primary objective was to develop an advising assistance program that could organize and retrieve course information for a university's Computer Science department. The application needed to read course data from a file, locate individual courses and their prerequisites, and display the complete course catalog in alphanumeric order. Before implementing the application, I needed to evaluate different data structures to determine which approach would provide the best balance of performance, memory usage, and functionality.

### How did you approach the problem? Why are data structures important to understand?

I approached the problem by first examining how vectors, hash tables, and binary search trees handle storing, searching, and organizing data. Rather than selecting a structure simply because it was familiar, I compared their runtime complexity and memory requirements against the application's actual needs. I ultimately selected a hash table because efficient course lookups were central to the program's functionality. Although a hash table does not inherently maintain sorted order, retrieving the stored courses and sorting them before printing provided a practical solution. This reinforced an important lesson: selecting a data structure requires understanding the operations a program performs most frequently and recognizing the tradeoffs associated with each design.

### How did you overcome any roadblocks you encountered while going through the activities or project?

One of the more challenging aspects was ensuring that course data remained accurate and consistent throughout the application. Reading a file and storing its contents was relatively straightforward, but accounting for missing prerequisites, duplicate course numbers, invalid records, and relationships between courses required additional consideration. I addressed these challenges by breaking the application into smaller functions and developing an incremental testing approach. I built a separate debugging environment where I could test individual components before integrating them into the main program. This allowed me to isolate problems, verify expected behavior, and introduce additional validation without repeatedly troubleshooting the entire application.

### How has your work on this project expanded your approach to designing software and developing programs?

This course strengthened my understanding of how early design decisions influence the behavior and performance of a completed application. Previously, I might have focused primarily on whether a proposed solution could accomplish the task. Now, I place greater emphasis on how the solution will perform as data grows, how its components interact, and whether the selected architecture supports future requirements. Comparing the different data structures also demonstrated that there is rarely one universally correct solution. Effective software design requires evaluating alternatives, understanding their limitations, and selecting an approach that aligns with the problem being solved.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

The most significant improvement in my development process has been placing greater emphasis on modularity, clear responsibilities, and validation. Rather than concentrating functionality inside one large section of code, I organized the application into smaller functions responsible for specific operations, such as loading course data, validating prerequisites, searching for courses, and displaying results. I also learned the value of descriptive naming, consistent formatting, and testing edge cases instead of relying exclusively on successful execution. These practices make code easier to understand, troubleshoot, and modify. More importantly, they support the kind of software development I value: building solutions that another developer can confidently review, maintain, and expand without having to reconstruct the original developer's thought process.

## Key Takeaways

CS 300 reinforced that software engineering is about more than producing working code. It requires understanding the problem, evaluating potential solutions, recognizing technical tradeoffs, and implementing systems that remain reliable and understandable over time.

The experience also strengthened my appreciation for incremental development and testing. By treating each component as an individual responsibility and validating its behavior before integration, I developed a more structured and deliberate approach to software engineering.

# TASK-2-CPP-MULTITHREADED-FILE-COMPRESSION-TOOL

COMPANY: CODTECH IT SOLUTIONS

NAME: SOUMIK SAHA

INTERN ID: CTO4DN437

DOMAIN: C++ PROGRAMMING 

DURATION: 4 WEEKS

MENTOR: NEELA SANTOSH

DESCRIPTION OF THE TASK: 
As part of my internship, I developed a multithreaded file compression and decompression tool using Run-Length Encoding (RLE) in C++. The project focuses on reducing the size of text files by encoding repeated characters and restoring them back to the original form efficiently. The tool also handles special characters such as \ and | using escape sequences, ensuring safe and accurate data processing.
Key Features:
Compression: Reads the file line-by-line, compresses each line using RLE (e.g., "aaaa" becomes "4|a"), and escapes special characters.

Decompression: Reconstructs the original data from the compressed file, correctly handling escaped characters.

Multithreading: Each line of the file is compressed or decompressed in a separate thread, significantly improving performance on large files.

Thread Safety: Uses mutex to protect shared resources during concurrent operations and error handling.

File I/O: Supports reading from and writing to text files with proper validation and error reporting.

Technologies Used:
C++ STL (fstream, thread, mutex, vector)

String manipulation and file handling

Basic multithreading and synchronization

This tool demonstrates the power of parallel processing and low-level file manipulation in C++, contributing to better performance in real-world data compression applications.

OUTPUT:
![Image](https://github.com/user-attachments/assets/230efb25-596e-4b3b-85c3-73c499a3f83f)


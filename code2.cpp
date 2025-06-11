#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

mutex mtx;

// Escape special characters (like '\' and '|')
string escapeChar(char ch) {
    if (ch == '\\') return "\\\\";
    if (ch == '|') return "\\|";
    return string(1, ch);
}

// Unescape character from escaped string (used during decompression)
char unescapeChar(const string& str, size_t& i) {
    if (str[i] == '\\' && i + 1 < str.size()) {
        return str[++i]; // Advance to escaped character
    }
    return str[i];
}

// Compress a single line using RLE
string rlecompress(const string& line) {
    if (line.empty()) return "";

    string result;
    int count = 1;

    for (size_t i = 1; i < line.length(); ++i) {
        if (line[i] == line[i - 1]) {
            ++count;
        } else {
            result += to_string(count) + "|" + escapeChar(line[i - 1]);
            count = 1;
        }
    }
    result += to_string(count) + "|" + escapeChar(line.back());
    return result;
}

// Decompress a single line
string rledecompress(const string& line) {
    string result;
    string countStr;

    for (size_t i = 0; i < line.length(); ++i) {
        if (isdigit(line[i])) {
            countStr += line[i];
        } else if (line[i] == '|') {
            ++i;
            if (i >= line.length()) break;  // Prevent out-of-bounds
            char ch = unescapeChar(line, i);

            try {
                int count = stoi(countStr);
                result += string(count, ch);
                countStr.clear();
            } catch (...) {
                lock_guard<mutex> lock(mtx);
                cerr << "Error: Invalid count in compressed data.\n";
                return "";
            }
        }
    }

    return result;
}

// Compress a text file line-by-line in parallel
void compressFile(const string& inputPath, const string& outputPath) {
    ifstream inputFile(inputPath);
    if (!inputFile) {
        lock_guard<mutex> lock(mtx);
        cerr << "Failed to open input file: " << inputPath << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    vector<string> compressed(lines.size());
    vector<thread> threads;

    for (size_t i = 0; i < lines.size(); ++i) {
        threads.emplace_back([&compressed, &lines, i]() {
            compressed[i] = rlecompress(lines[i]);
        });
    }

    for (auto& t : threads) t.join();

    ofstream outFile(outputPath);
    if (!outFile) {
        lock_guard<mutex> lock(mtx);
        cerr << "Failed to open output file: " << outputPath << endl;
        return;
    }

    for (const auto& c : compressed) {
        outFile << c << '\n';
    }

    lock_guard<mutex> lock(mtx);
    cout << "Compression complete: " << outputPath << endl;
}

// Decompress a text file line-by-line in parallel
void decompressFile(const string& inputPath, const string& outputPath) {
    ifstream inputFile(inputPath);
    if (!inputFile) {
        lock_guard<mutex> lock(mtx);
        cerr << "Failed to open input file: " << inputPath << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    vector<string> decompressed(lines.size());
    vector<thread> threads;

    for (size_t i = 0; i < lines.size(); ++i) {
        threads.emplace_back([&decompressed, &lines, i]() {
            decompressed[i] = rledecompress(lines[i]);
        });
    }

    for (auto& t : threads) t.join();

    ofstream outFile(outputPath);
    if (!outFile) {
        lock_guard<mutex> lock(mtx);
        cerr << "Failed to open output file: " << outputPath << endl;
        return;
    }

    for (const auto& d : decompressed) {
        outFile << d << '\n';
    }

    lock_guard<mutex> lock(mtx);
    cout << "Decompression complete: " << outputPath << endl;
}

int main() {
    string inputFile;
    cout << "Enter the name of the file to compress: ";
    cin >> inputFile;

    string compressedFile = "compressed_output.txt";
    string decompressedFile = "decompressed_output.txt";

    compressFile(inputFile, compressedFile);
    decompressFile(compressedFile, decompressedFile);

    cout << "All tasks completed successfully!" << endl;
    return 0;
}

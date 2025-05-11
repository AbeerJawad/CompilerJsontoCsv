# CompilerJsontoCsv
A lightweight C program that parses JSON files and converts them to CSV format using Flex and Bison.
Overview

This project provides a command-line tool for converting JSON files into CSV format. It parses JSON files using a lexer/parser combo (Flex/Bison), builds an abstract syntax tree (AST) representation of the JSON structure, and then flattens it into a CSV file.
Features

    Parses standard JSON files including objects, arrays, strings, numbers, booleans, and null values
    Converts JSON structure to flat CSV representation
    Supports nested structures (marked as [complex] in the CSV output)
    Command-line options for customizing output
    AST visualization for debugging

Project Structure

    main.c - Entry point and command-line interface
    scanner.l - Flex lexer specification for tokenizing JSON
    parser.y - Bison parser specification for JSON grammar
    ast.h/ast.c - Abstract Syntax Tree implementation
    csv_writer.h/csv_writer.c - CSV output generation

Build Requirements

    GCC compiler
    Flex (lexical analyzer generator)
    Bison (parser generator)
    Make

Building the Project

bash

# Generate parser and scanner code
flex -o scanner.c scanner.l
bison -d -o parser.c parser.y

# Compile source files
gcc -c main.c ast.c csv_writer.c scanner.c parser.c

# Link object files
gcc main.o ast.o csv_writer.o scanner.o parser.o -o json2csv

Alternatively, you can use the included Makefile:

bash

make

Usage

bash

./json2csv [options] input_file

Options:
  --print-ast      Display the AST structure (for debugging)
  --generate-csv   Generate CSV output (default output file is output.csv)
  --output-dir DIR Specify output directory (default is current directory)

Examples

Convert a JSON file to CSV:

bash

./json2csv --generate-csv example.json

Display the AST structure without generating CSV:

bash

./json2csv --print-ast example.json

Specify an output directory:

bash

./json2csv --generate-csv --output-dir /path/to/output example.json

CSV Output Format

The CSV output follows these rules:

    The first row contains field names (headers)
    Each JSON object is represented as a row in the CSV
    Nested objects/arrays are marked as [complex]
    First column is a unique ID
    JSON types are preserved in the CSV (strings in quotes, numbers as-is, etc.)

Limitations

    Only processes the first level of nested structures
    Maximum 64 fields per row
    Does not handle special JSON escape sequences in strings

Example

Input JSON:

json

{
  "name": "John Doe",
  "age": 30,
  "isStudent": false,
  "address": {
    "street": "123 Main St",
    "city": "Anytown",
    "zipCode": 12345
  },
  "phoneNumbers": [
    "555-1234",
    "555-5678"
  ],
  "email": "john.doe@example.com",
  "active": true,
  "score": 85.5,
  "notes": null
}

Output CSV:

id,name,age,isStudent,address,street,city,zipCode,phoneNumbers,email,active,score,notes
1,"John Doe",30,false,[complex],"123 Main St","Anytown",12345,[complex],"john.doe@example.com",true,85.5,null

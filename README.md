# Compiler JsontoCsv
A C program that parses JSON files and converts them to CSV format using Flex and Bison.
Overview

This project provides a command-line tool for converting JSON files into CSV format. It parses JSON files using a lexer/parser combo (Flex/Bison), builds an abstract syntax tree (AST) representation of the JSON structure, and then flattens it into a CSV file.
Features

    -Parses standard JSON files including objects, arrays, strings, numbers, booleans, and null values
    -Converts JSON structure to flat CSV representation
    -Supports nested structures (marked as [complex] in the CSV output)
    -Command-line options for customizing output
    -AST visualization for debugging

Project Structure

    -main.c - Entry point and command-line interface
    -scanner.l - Flex lexer specification for tokenizing JSON
    -parser.y - Bison parser specification for JSON grammar
    -ast.h/ast.c - Abstract Syntax Tree implementation
    -csv_writer.h/csv_writer.c - CSV output generation

Build Requirements

    -GCC compiler
    -Flex (lexical analyzer generator)
    -Bison (parser generator)

Building the Project

    -bison -d parser.y
    -flex scanner.l
    -gcc -o test main.c ast.c csv_writer.c parser.tab.c lex.yy.c -lm
    -./test example.json --print-ast --generate-csv

CSV Output Format

The CSV output follows these rules:

    The first row contains field names (headers)
    Each JSON object is represented as a row in the CSV
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

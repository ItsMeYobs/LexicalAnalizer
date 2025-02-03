#include <ctype.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

// Function to check if a character is a delimiter
bool isDelimiter(char chr) {
    return (chr == ' ' || chr == '+' || chr == '-' ||
            chr == '*' || chr == '/' || chr == ',' ||
            chr == ';' || chr == '%' || chr == '>' ||
            chr == '<' || chr == '=' || chr == '(' ||
            chr == ')' || chr == '[' || chr == ']' ||
            chr == '{' || chr == '}');
}

// Function to check if a character is an operator
bool isOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '*' ||
            chr == '/' || chr == '>' || chr == '<' ||
            chr == '=');
}

// Function to check if a character is a parenthesis
bool isParenthesis(char chr) {
    return (chr == '{' || chr == '}' || chr == '[' || 
            chr == ']' || chr == '(' || chr == ')');
}

// Function to check if a string is a valid identifier
bool isValidIdentifier(char* str) {
    return (str[0] != '0' && str[0] != '1' && str[0] != '2' &&
            str[0] != '3' && str[0] != '4' && str[0] != '5' &&
            str[0] != '6' && str[0] != '7' && str[0] != '8' &&
            str[0] != '9' && !isDelimiter(str[0]));
}

// Function to check if a string is a keyword
bool isKeyword(char* str) {
    const char* keywords[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };
    int i;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

// Function to check if a string is a constant
bool isConstant(char* str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    while (isdigit(str[i])) {
        i++;
    }
    return str[i] == '\0';
}

// Function to extract a substring from the given string
char* getSubstring(char* str, int start, int end) {
    int subLength = end - start + 1;
    char* subStr = (char*)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}

// Function to analyze the lexical structure of the input
void lexicalAnalyzer(char* input, FILE* file) {
    int len = strlen(input);
    int left = 0;

    fprintf(file, "\n%-15s %-15s\n", "Lexeme", "Token");
    fprintf(file, "--------------------------------\n");

    // Traverse input character by character
    for (int right = 0; right <= len; right++) {
        // If a delimiter (whitespace or end of string) is encountered, process the lexeme
        if (input[right] == ' ' || input[right] == '\0') {
            if (right > left) { // Skip if there's no lexeme (empty space)
                char* subStr = getSubstring(input, left, right - 1);

                // Check the type of lexeme
                if (isKeyword(subStr)) {
                    fprintf(file, "%-15s %-15s\n", subStr, "Keyword");
                } else if (isConstant(subStr)) {
                    fprintf(file, "%-15s %-15s\n", subStr, "Constant");
                } else if (isValidIdentifier(subStr)) {
                    fprintf(file, "%-15s %-15s\n", subStr, "Identifier");
                } else {
                    fprintf(file, "%-15s %-15s\n", subStr, "Unidentified");
                }

                free(subStr);
            }

            // Move left to the next character after the delimiter
            left = right + 1;
        } else if (isDelimiter(input[right])) {
            // Handle single-character tokens (operators, parentheses)
            char* subStr = getSubstring(input, right, right);
            if (isOperator(input[right])) {
                fprintf(file, "%-15s %-15s\n", subStr, "Operator");
            } else if (isParenthesis(input[right])) {
                fprintf(file, "%-15s %-15s\n", subStr, "Parenthesis");
            }
            free(subStr);
        }
    }
}

// Main function to read input and run the lexical analyzer
int main() {
    char code_input[MAX_LENGTH];
    FILE* file = fopen("output.txt", "w");

    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("Enter a line of code: ");
    fgets(code_input, MAX_LENGTH, stdin);
    code_input[strcspn(code_input, "\n")] = '\0'; // Remove newline character

    lexicalAnalyzer(code_input, file); 

    fclose(file);
    system("notepad output.txt");

    return 0;
}

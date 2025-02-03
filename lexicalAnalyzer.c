#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

bool isDelimiter(char chr) {
    return (chr == ' ' || chr == '+' || chr == '-' ||
            chr == '*' || chr == '/' || chr == ',' ||
            chr == ';' || chr == '%' || chr == '>' ||
            chr == '<' || chr == '=' || chr == '(' ||
            chr == ')' || chr == '[' || chr == ']' ||
            chr == '{' || chr == '}');
}

bool isOperator(char chr) {
    return (chr == '+' || chr == '-' || chr == '*' ||
            chr == '/' || chr == '>' || chr == '<' ||
            chr == '=');
}

bool isParenthesis(char chr) {
    return (chr == '{' || chr == '}' || chr == '[' || 
            chr == ']' || chr == '(' || chr == ')');
}

bool isValidIdentifier(char* str) {
    return (str[0] != '0' && str[0] != '1' && str[0] != '2' &&
            str[0] != '3' && str[0] != '4' && str[0] != '5' &&
            str[0] != '6' && str[0] != '7' && str[0] != '8' &&
            str[0] != '9' && !isDelimiter(str[0]));
}

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

char* getSubstring(char* str, int start, int end) {
    int subLength = end - start + 1;
    char* subStr = (char*)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}

void lexicalAnalyzer(char* input, FILE* file) {
    int left = 0, right = 0;
    int len = strlen	);

    fprintf(file, "\n%-15s %-15s\n", "Lexeme", "Token");
    fprintf(file, "--------------------------------\n");
    
    while (right <= len && left <= right) {
        if (!isDelimiter(input[right]))
            right++;

        if (isDelimiter(input[right]) && left == right) {
            if (isOperator(input[right])) {
                fprintf(file, "%-15c %-15s\n", input[right], "Operator");
            } else if (isParenthesis(input[right])) {
                fprintf(file, "%-15c %-15s\n", input[right], "Parenthesis");
            }
            right++;
            left = right;
        } else if (isDelimiter(input[right]) && left != right ||
                   (right == len && left != right)) {
            char* subStr = getSubstring(input, left, right - 1);

            if (isKeyword(subStr)) {
                fprintf(file, "%-15s %-15s\n", subStr, "Keyword");
            } else if (isConstant(subStr)) {
                fprintf(file, "%-15s %-15s\n", subStr, "Constant");
            } else if (isValidIdentifier(subStr) &&
                       !isDelimiter(input[right - 1])) {
                fprintf(file, "%-15s %-15s\n", subStr, "Identifier");
            } else if (!isValidIdentifier(subStr) &&
                       !isDelimiter(input[right - 1])) {
                fprintf(file, "%-15s %-15s\n", subStr, "Unidentified"); 
            }
            free(subStr);
            left = right;
        }
    }
}

int main() {
    char code_input[MAX_LENGTH];
    FILE* file = fopen("output.txt", "a");
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


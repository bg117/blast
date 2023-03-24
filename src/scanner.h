#ifndef SCANNER_H
#define SCANNER_H

/**
 * @brief Struct representing a scanner.
 *
 * A scanner is a data structure that reads a string and returns tokens.
 */
struct scanner
{
    char *src;
    int   i;
};

struct token;

/**
 * @brief Scans all tokens from the given source code.
 *
 * @param scanner The scanner to use.
 * @param num_tokens The number of tokens scanned.
 * @return The tokens scanned.
 */
struct token *scanner_scan(struct scanner *scanner, int *num_tokens);

#endif

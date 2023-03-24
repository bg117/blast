#ifndef SCANNER_H
#define SCANNER_H

/**
 * @brief Struct representing a scanner.
 *
 * A scanner is a data structure that reads a string and returns tokens.
 */
struct scanner {
    char *src;
    int i;
};

/**
 * @brief Scans all tokens from the given source code.
 *
 * @param scanner The scanner to use.
 * @param tokens The array of tokens to store the scanned tokens in.
 * @return The number of tokens scanned.
 *
 * @remark The tokens array must be large enough to store all tokens.
 */
void scanner_scan(struct scanner *scanner, struct token *tokens);

#endif

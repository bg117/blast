#ifndef KEY_VALUE_PAIR_H
#define KEY_VALUE_PAIR_H

/**
 * @brief Struct representing a key-value pair.
 *
 * A key-value pair is a data structure that associates a key with a value. For example,
 * the key-value pair (1, "one") associates the key 1 with the value "one".
 */
struct key_value_pair {
    int key;
    char *value;
};

#endif

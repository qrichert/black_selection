/*
 * /!\ Don't use this in production.
 * Seems to work just fine but maybe not.
 * Probably has memory leaks.
 *
 * To compile, run:
 * gcc -o black_selection -O3 [-Wall] black_selection.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Char Char;
struct Char {
    char value;
    Char *next_character;
    Char *previous_character;
};

Char *Char_new() {
    Char *new_char = malloc(sizeof(Char));
    new_char->value = 0;
    new_char->next_character = NULL;
    new_char->previous_character = NULL;
    return new_char;
}

void Char_delete(Char *character) {
    free(character);
}


typedef struct String String;
struct String {
    Char *first_character;
    Char *last_character;
};

String *String_new() {
    String *new_string = malloc(sizeof(String));
    new_string->first_character = NULL;
    new_string->last_character = NULL;
    return new_string;
}

void String_delete(String *string) {
    Char *current_character = string->first_character;
    free(string);

    while (current_character != NULL) {
        Char *tmp_character = current_character->next_character;
        free(current_character);
        current_character = tmp_character;
    }
}

int String_is_empty(String *string) {
    return string->first_character == NULL;
}

int String_length(String *string) {
    int length = 0;

    Char *current_character = string->first_character;
    while (current_character != NULL) {
        length++;
        current_character = current_character->next_character;
    }

    return length;
}

void String_print(String *string) {
    Char *character = string->first_character;
    while (character != NULL) {
        printf("%c", character->value);
        character = character->next_character;
    }
}

int String_contains_char(String *string, Char *character) {
    Char *current_character = string->first_character;

    while (current_character != NULL) {
        if (current_character == character) {
            return 1;
        }
        current_character = current_character->next_character;
    }

    return 0;
}

void String_append_char(String *string, const char character) {
    Char *new_character = Char_new();
    new_character->value = character;

    Char *last_character = string->last_character;
    if (last_character != NULL) {
        last_character->next_character = new_character;
        new_character->previous_character = last_character;
        string->last_character = new_character;
    } else {
        string->first_character = new_character;
        string->last_character = new_character;
    }
}

void String_append_chars(String *string, const char characters[]) {
    int nb_characters = strlen(characters);
    for (int i = 0; i < nb_characters; i++) {
        String_append_char(string, characters[i]);
    }
}

void String_merge_append_string(String *string, String *string_to_append) {
    if (String_is_empty(string_to_append)) {
        free(string_to_append);
        return;
    }
    if (!String_is_empty(string)) {
        string->last_character->next_character = string_to_append->first_character;
        string_to_append->first_character->previous_character = string->last_character;
        string->last_character = string_to_append->last_character;
    } else {
        string->first_character = string_to_append->first_character;
        string->last_character = string_to_append->last_character;
    }
    free(string_to_append);
}

void String_prepend_char(String *string, const char character) {
    Char *new_character = Char_new();
    new_character->value = character;

    Char *first_character = string->first_character;
    if (first_character != NULL) {
        first_character->previous_character = new_character;
        new_character->next_character = first_character;
        string->first_character = new_character;
    } else {
        string->first_character = new_character;
        string->last_character = new_character;
    }
}

void String_prepend_chars(String *string, const char characters[]) {
    int nb_characters = strlen(characters);
    for (int i = nb_characters - 1; i >= 0; i--) {
        String_prepend_char(string, characters[i]);
    }
}

void String_merge_prepend_string(String *string, String *string_to_prepend) {
    if (String_is_empty(string_to_prepend)) {
        free(string_to_prepend);
        return;
    }
    if (!String_is_empty(string)) {
        string->first_character->previous_character = string_to_prepend->last_character;
        string_to_prepend->last_character->next_character = string->first_character;
        string->first_character = string_to_prepend->first_character;
    } else {
        string->first_character = string_to_prepend->first_character;
        string->last_character = string_to_prepend->last_character;
    }
    free(string_to_prepend);
}

void String_remove_char(String *string, Char *character) {
    if (!String_contains_char(string, character)) {
        return;
    }

    if (character->previous_character != NULL) {
        (character->previous_character)->next_character = character->next_character;
    }
    if (character->next_character != NULL) {
        (character->next_character)->previous_character = character->previous_character;
    }

    if (character == string->first_character) {
        string->first_character = character->next_character;
    }
    if (character == string->last_character) {
        string->last_character = character->previous_character;
    }

    Char_delete(character);
}

void String_remove_index(String *string, int index) {
    int string_length = String_length(string);
    if (index < 0 || index >= string_length) {
        return;
    }

    if (index == 0) {
        String_remove_char(string, string->first_character);
    } else if (index == string_length - 1) {
        String_remove_char(string, string->last_character);
    } else {
        Char *current_character = string->first_character;
        int current_index = 0;
        while (current_character != NULL) {
            if (index == current_index) {
                String_remove_char(string, current_character);
                break;
            }
            current_character = current_character->next_character;
            current_index++;
        }
    }
}

String *String_ltrim(String *string, const char character) {
    String *trimmed_characters = String_new();
    Char *current_character = string->first_character;

    if (current_character == NULL) {
        return trimmed_characters;
    }

    while (current_character != NULL && current_character->value == character) {
        String_append_char(trimmed_characters, current_character->value);
        Char *char_to_remove = current_character;
        current_character = current_character->next_character;
        String_remove_char(string, char_to_remove);
    }

    return trimmed_characters;
}

String *String_rtrim(String *string, const char character) {
    String *trimmed_characters = String_new();
    Char *current_character = string->last_character;

    if (current_character == NULL) {
        return trimmed_characters;
    }

    while (current_character != NULL && current_character->value == character) {
        String_append_char(trimmed_characters, current_character->value);
        Char *char_to_remove = current_character;
        current_character = current_character->previous_character;
        String_remove_char(string, char_to_remove);
    }

    return trimmed_characters;
}

int String_lcount(String *string, const char character) {
    int count = 0;

    Char *current_character = string->first_character;
    while (current_character != NULL && current_character->value == character) {
        count++;
        current_character = current_character->next_character;
    }

    return count;
}

int String_rcount(String *string, const char character) {
    int count = 0;

    Char *current_character = string->last_character;
    while (current_character != NULL && current_character->value == character) {
        count++;
        current_character = current_character->previous_character;
    }

    return count;
}

char *String_to_char(String *string) {
    int string_length = String_length(string);
    char *string_as_char = malloc(sizeof(char) * string_length + 1);

    Char *current_character = string->first_character;
    int i = 0;
    while (current_character != NULL) {
        string_as_char[i] = current_character->value;
        current_character = current_character->next_character;
        i++;
    }

    string_as_char[string_length] = '\0';

    return string_as_char;
}


int main(int argc, char **argv) {
    // Get args.
    if (argc < 5) {
        printf("Error: Missing arguments.\n\n    ./black_selection "
        "/path/to/black input_file.py start_line end_line [line_length]\n");
        return EXIT_FAILURE;
    }

    char *black = malloc(sizeof(char) * strlen(argv[1]));
    strcpy(black, argv[1]);

    char *input_file = malloc(sizeof(char) * strlen(argv[2]));
    strcpy(input_file, argv[2]);

    const int line_start = (int) strtol(argv[3], NULL, 10);
    const int line_end = (int) strtol(argv[4], NULL, 10);

    int line_length = -1;
    if (argc >= 6)
        line_length = (int) strtol(argv[5], NULL, 10);

    // Get selected code (line range).
    String *begin_block = String_new();
    String *selection = String_new();
    String *end_block = String_new();

    FILE *file = NULL;
    file = fopen(input_file, "r");
    if (file != NULL) {
        char character = 0;
        char last_character = 0;
        int current_line = 0;
        int current_step = 0;  // 0 = begin block, 1 = selection, 2 = end block

        while ((character = fgetc(file)) != EOF) {
            if (last_character == '\n' || last_character == 0) {
                current_line += 1;
                if (current_line == line_start) {
                    current_step = 1;
                }
                if (current_line == line_end + 1) {
                    current_step = 2;
                }
            }
            last_character = character;
            if (current_step == 0) {
                String_append_char(begin_block, character);
            } else if (current_step == 1) {
                String_append_char(selection, character);
            } else if (current_step == 2) {
                String_append_char(end_block, character);
            }
        }
        fclose(file);
    } else {
        return EXIT_FAILURE;
    }

    if (String_is_empty(selection)) {
        return EXIT_SUCCESS;
    }

    // Save leading newlines, and remove them from selection (start with code).
    String *leading_newlines = String_ltrim(selection, '\n');

    // Save trailing newlines, and remove them from selection (end with code).
    String *trailing_newlines = String_rtrim(selection, '\n');

    // If selection is indented, make it valid Python by putting it into blocks.
    int leading_whitespace_length_spaces = String_lcount(selection, ' ');
    int leading_whitespace_length_tabs = String_lcount(selection, '\t');
    int indentation_level = 0;  // Code indentation level, determines number of blocks.
    char *indent = NULL;  // Indent to add (four spaces of tab)
    int indent_blocks_total_length = 0;  // How many chars have been added, total.

    if (leading_whitespace_length_spaces) {
        indentation_level = leading_whitespace_length_spaces /= 4;
        indent = "    ";
    } else if (leading_whitespace_length_tabs) {
        indentation_level = leading_whitespace_length_tabs;
        indent = "\t";
    }

    String *indent_blocks = String_new();
    int indent_length = (int) strlen(indent);
    for (int i = 0; i < indentation_level; i++) {  // indent blocks
        for (int j = 0; j < i; j++) {  // indents for line
            String_append_chars(indent_blocks, indent);
            indent_blocks_total_length += indent_length;
        }
        String_append_chars(indent_blocks, "if True:\n");
        indent_blocks_total_length += 9;
    }
    String_merge_prepend_string(selection, indent_blocks);

    // Write selection to tmp file.
    char tmp_file[] = "/tmp/._black_selection.txt";
    file = fopen(tmp_file, "w");
    if (file != NULL) {
        fputs(String_to_char(selection), file);
        fclose(file);
    } else {
        return EXIT_FAILURE;
    }
    String_delete(selection);

    // Run black on tmp file.
    char *black_command = malloc(sizeof(char) * 1000);
    black_command = strcpy(black_command, black);
    black_command = strcat(black_command, " ");
    black_command = strcat(black_command, tmp_file);

    if (line_length > 0) {
        // First, fake-write to NULL to get final string size.
        int size = snprintf(NULL, 0, " --line-length %d", line_length);
        char *line_length_str = malloc(size + 1);
        sprintf(line_length_str, " --line-length %d", line_length);
        black_command = strcat(black_command, line_length_str);
        free(line_length_str);
    }

    if (system(black_command) != 0) {
        return EXIT_FAILURE;
    }
    free(black_command);

    selection = String_new();

    file = fopen(tmp_file, "r");
    if (file != NULL) {
        char character = 0;
        while ((character = fgetc(file)) != EOF) {
            String_append_char(selection, character);
        }
        fclose(file);
    } else {
        return EXIT_FAILURE;
    }

    // Replace selected lines with reformatted lines.

    // Trim blackened code (and delete trimmed bits).
    String_delete(String_ltrim(selection, '\n'));
    String_delete(String_rtrim(selection, '\n'));

    // Remove placeholder indent blocks.
    while (indent_blocks_total_length > 0) {
        String_remove_index(selection, 0);
        indent_blocks_total_length--;
    }

    // Trim blackened code again (for lines added after indent block).
    String_delete(String_ltrim(selection, '\n'));

    // Restore saved leading newlines.
    String_merge_prepend_string(selection, leading_newlines);
    // Restore saved trailing newlines.
    String_merge_append_string(selection, trailing_newlines);

    // Update input file.
    file = fopen(input_file, "w");
    if (file != NULL) {
        fputs(String_to_char(begin_block), file);
        fputs(String_to_char(selection), file);
        fputs(String_to_char(end_block), file);
        fclose(file);
    } else {
        return EXIT_FAILURE;
    }

    String_delete(selection);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int failed() {
    printf("Failed\n");
    return 1;
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

char *remove_extension(const char *filename) {
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) {
        return strdup(filename);
    }
    size_t len = dot - filename;
    char *new_filename = malloc(len + 1);
    if (!new_filename) {
        return NULL;
    }
    strncpy(new_filename, filename, len);
    new_filename[len] = '\0';
    return new_filename;
}

int extract(const char *filename) {
    printf("Extracting appImage: '%s'\n", filename);
    char *base_filename = remove_extension(filename);
    if (!base_filename) {
        printf("Failed to remove extension\n");
        failed();
        return 1;
    }

    char extract[100];
    sprintf(extract, "chmod +x %s && mv %s %s", filename, filename, base_filename);
    system(extract);
    printf("Extracted binary: %s\n", base_filename);
    free(base_filename);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    if (file_exists(filename)) {
        size_t len = strlen(filename);
        if (len > 8 && strcmp(filename + len - 9, ".appImage") == 0) {
            extract(filename);
        } else {
            printf("Not a valid file extension\n");
            failed();
            return 1;
        }
    } else {
        printf("The file '%s' does not exist.\n", filename);
        failed();
        return 1;
    }

    return 0;
}
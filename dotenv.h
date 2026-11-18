#ifndef DOTENV_H
    #define DOTENV_H
    #include <stddef.h>

    size_t initenv(const char* file_path);

#endif // DOTENV_H

#ifdef DOTENV_IMPLEMENTATION
    #undef DOTENV_IMPLEMENTATION

    #ifndef DOTENV_FILE_BUFFER_SIZE
        #define DOTENV_FILE_BUFFER_SIZE 1 << 15
    #endif

    #ifndef DOTENV_BUFFER_SIZE
        #define DOTENV_BUFFER_SIZE 1 << 11
    #endif

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>


    size_t initenv(const char* file_path) {
        
        FILE* env_file = fopen(file_path, "r");
        if (!env_file) {
            perror("fopen: Unable to open .env");
            return 0;
        }

        unsigned char env_contents[DOTENV_FILE_BUFFER_SIZE] = {0};
        size_t read = fread(&env_contents, sizeof(unsigned char), DOTENV_FILE_BUFFER_SIZE, env_file);
        fclose(env_file);
        if (!read) {
            return 0;
        }
        
        unsigned char key[DOTENV_BUFFER_SIZE] = {0};
        unsigned char value[DOTENV_BUFFER_SIZE] = {0};

        int edit_key = 1;
        size_t key_num = 0;
        size_t val_num = 0;
        
        for (size_t i = 0; i <= strlen((const char*)&env_contents); i++) {
            const unsigned char c = env_contents[i];
            if (key_num >= DOTENV_BUFFER_SIZE || val_num >= DOTENV_BUFFER_SIZE) {
                perror("dotenv: buffer overflow\n");
                return 0;
            }
            switch (c) {
                case '=':
                    edit_key = 0;
                    break;
                case '\0':
                case '\n':
                    setenv((const char*)&key, (const char*)&value, 1);
                    memset(&key, 0, key_num);
                    memset(&value, 0, val_num);
                    key_num = 0;
                    val_num = 0;
                    edit_key = 1;
                    break;
                default:
                    if (edit_key) {
                        key[key_num++] = c;
                    } else {
                        value[val_num++] = c;
                    }
                    break;
            }
        }
        return 1;
    }

#endif // DOTENV_IMPLEMENTATION
#ifndef DOTENV_H
    #define DOTENV_H

    void initenv(const char* file_path);

#endif // DOTENV_H

#ifdef DOTENV_IMPLEMENTATION
    #undef DOTENV_IMPLEMENTATION

    #ifndef DOTENV_BUFFER_SIZE
        #define DOTENV_BUFFER_SIZE 2048
    #endif

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>


    void initenv(const char* file_path) {
        
        FILE* env_file = fopen(file_path, "r");
        if (!env_file) {
            perror("fopen: Unable to open .env");
            return;
        }

        unsigned char env_contents[DOTENV_BUFFER_SIZE] = {0};
        size_t read = fread(&env_contents, sizeof(unsigned char), sizeof(env_contents), env_file);
        fclose(env_file);
        if (!read) {
            return;
        }
        
        unsigned char key[256] = {0};
        unsigned char value[256] = {0};

        int edit_key = 1;
        size_t j = 0;
        
        for (size_t i = 0; i <= strlen((const char*)&env_contents); i++) {
            const unsigned char c = env_contents[i];
            switch (c) {
                case '=':
                    edit_key = 0;
                    j = 0;
                    break;
                case '\0':
                case '\n':
                    setenv((const char*)&key, (const char*)&value, 1);
                    memset(&key, 0, 256);
                    memset(&value, 0, 256);
                    j = 0;
                    edit_key = 1;
                    break;
                default:
                    if (edit_key) {
                        key[j++] = c;
                    } else {
                        value[j++] = c;
                    }
                    break;
            }
        }

        return;
    }

#endif // DOTENV_IMPLEMENTATION
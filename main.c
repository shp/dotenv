#define DOTENV_IMPLEMENTATION
#include "dotenv.h"

int main(void) {
    if(initenv(".env")) {
        const char *token = getenv("token");
        const char *idk = getenv("idk");
        printf("token = %s\nidk = %s\n", token, idk);
    };
    
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <curl/curl.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>

#define TARGET_PREFIX "0000000"

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}

// Fast integer-to-string (no sprintf)
static inline char *ulltoa_fast(uint64_t value, char *buf) {
    char *p = buf;
    char *p1, tmp;
    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return buf;
    }
    p1 = p;
    while (value > 0) {
        *p++ = (char)('0' + (value % 10));
        value /= 10;
    }
    *p = '\0';
    // reverse
    for (--p; p1 < p; ++p1, --p) {
        tmp = *p1; *p1 = *p; *p = tmp;
    }
    return buf;
}

// SHA256 string -> hex output
void sha256_string(const char *input, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)input, strlen(input), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

// libcurl write callback
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    fwrite(contents, size, nmemb, stdout);
    return total_size;
}

// Very fast random generator (xorshift64)
static uint64_t xorshift64(uint64_t *state) {
    uint64_t x = *state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return *state = x;
}

void mine(const char *address) {
    printf("Mining has begun. To view menu, press Control + C\n");
    unsigned long long count = 0;
    char proof[32];
    char hash_hex[65];
    size_t prefix_len = strlen(TARGET_PREFIX);

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }

    uint64_t rng_state = (uint64_t)time(NULL) ^ (uint64_t)clock(); // seed RNG

    while (!stop) {
        // Generate 40-bit random number
        uint64_t rand_val = xorshift64(&rng_state) & ((1ULL << 40) - 1);

        ulltoa_fast(rand_val, proof);
        sha256_string(proof, hash_hex);

        if (strncmp(hash_hex, TARGET_PREFIX, prefix_len) == 0) {
            printf("\nNew Hash Found!\n");
            printf("%llu hashes attempted so far.\n", count);

            char url[512];
            snprintf(url, sizeof(url),
                     "https://eshells.pythonanywhere.com/submit_hash/%s-%s-%s",
                     address, hash_hex, proof);

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "Error submitting: %s\n", curl_easy_strerror(res));
            }

            printf("\nTechnical data: %s was made from %s.\n", hash_hex, proof);
        }

        count++;
    }

    printf("\n%llu hashes attempted so far.\n", count);
    curl_easy_cleanup(curl);
}

int main() {
    char address[128];
    printf("Username? ");
    if (!fgets(address, sizeof(address), stdin)) {
        return 1;
    }
    address[strcspn(address, "\n")] = 0;

    signal(SIGINT, handle_sigint);

    while (1) {
        char choice[8];
        printf("Would you like to keep mining (Y/n, or just press enter for yes)? ");
        if (!fgets(choice, sizeof(choice), stdin)) break;
        choice[strcspn(choice, "\n")] = 0;

        if (strlen(choice) == 0 || choice[0] == 'y' || choice[0] == 'Y') {
            stop = 0;
            mine(address);
        } else {
            printf("Exiting...\n");
            break;
        }
    }
    return 0;
}

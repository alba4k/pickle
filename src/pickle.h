typedef unsigned long size_t;

// encodes byte into dest,
// dest needs to have a buffer of at least 7B
void byteenc(const char byte, char *dest);

// encodes str into dest
// dest has to be 6x+1 strlen(str)
void strenc(const char *str, char *dest);


// decodes str into dest
// str needs to contain 2 pickle words, dest needs 1B allocated
void bytedec(const char *str, char *dest);

// decodes str, return the outcome
// str needs to contain 2 pickle words
char chrdec(const char *str);

// decode str into dest
// str can only contain pickle words
// dest needs at least(1/6)x-1 strlen(str) bytes allocated
void strdec(const char *str, char *dest);


// encodes n bytes of str into dest
// dest needs a 6n+1 buffer
void strnenc(const char *str, char *dest, size_t n);

// decodes 6n bytes of str into dest
// dest needs a buffer of n+1 bytes
void strndec(const char *str, char *dest, size_t n);

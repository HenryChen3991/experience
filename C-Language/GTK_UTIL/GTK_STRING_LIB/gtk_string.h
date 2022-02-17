/**
 * Delete blank characters at the beginning and end of the line
 */
char * strtrim(char *s);

/**
 * Delete string blank characters, including the beginning and end of a line
 */
char * strtrimc( char * s );

void cmsUtl_strToUpper(char *string);
void cmsUtl_strToLower(char *string);
int cmsUtl_strcmp(const char *s1, const char *s2);
void print_short_type_bin(int n);
void print_int_type_bin(int n);

char *cmsUtl_strncpy(char *dest, const char *src, int dlen);
int hex2int(char *hex);


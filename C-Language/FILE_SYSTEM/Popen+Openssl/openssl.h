void openssl_aes_128_cbc_decrypt(char *file_path);
void openssl_aes_128_cbc_encrypt(char *file_path);
typedef enum
{
	not_supported,
	encrypt,
	decrypt
} openssl_action_e;
extern char *openssl_action_name[32];
openssl_action_e analyze_openssl_action(char *action);

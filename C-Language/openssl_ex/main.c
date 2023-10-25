#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <time.h>

#define AES_BLOCK_SIZE 16
#define MAX_SEND_BUFF_SIZE 4096
#define MAX_PKT_BUFF 4096

/**********************************************************
函数名：PKCS5Padding          
参数：unsigned char *str      --字符串地址
返回值：int                   --正向测试填充后的字符串长度
说明：对初始数据进行PKCS5Padding填充
***********************************************************/
int PKCS5Padding(unsigned char *str, int len)
{
    int remain, i;
    
    remain = 16 - len % 16;
    //printf("remain = %d\n",remain);
    for (i = 0; i < remain; i++)
    {
        str[len + i] = remain;
        //printf("str[len+i]= %d\n",str[len+i]);
    }
       str[len + i] = '\0';
    return len + remain;
}

/**********************************************************
函数名：DePKCS5Padding         
参数：unsigned char *p    --字符串地址
返回值：int               --反填充个数
说明：对明文进行PKCS5Padding填充反填充(去除后面的填充乱码)
***********************************************************/
int DePKCS5Padding(unsigned char *str, int str_len)
{
     int remain,i;

     remain = *(str + str_len - 1);//读取填充的个数
     //printf("remain = %d\n",remain);
    
     for(i = 0; i < remain; i++){str--;}
     str++;
     *str = '\0';//截断
    
     return remain;
}

int aes_cbc_encrypt(char* in, int in_len, char* key, char *iv, char* out)//, int olen)可能会设置buf长度
{
    int i = 0;
    AES_KEY aes;
    int len;
    char aes_encode_temp[MAX_PKT_BUFF] = {0};

    if(!in || !key || !out) return 0;

    //抽取数据
    memcpy(aes_encode_temp, in, in_len);

    //进行PCK5填充 获取填充后长度
    len = PKCS5Padding((unsigned char*)aes_encode_temp, in_len);
    
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0) {
        printf("fail to set encrypt key\n");
        return 0;
    }

    AES_cbc_encrypt((unsigned char*)aes_encode_temp, (unsigned char*)out, len, &aes, (unsigned char*)iv, AES_ENCRYPT);

    return 1;
}

int aes_cbc_decrypt(char* in, int in_len, char* key, char *iv, char* out)
{
    int i;
    AES_KEY aes;

    if(!in || !key || !out) return 0;

    if (AES_set_decrypt_key((unsigned char*)key, strlen(key) * 8, &aes) < 0) {
        printf("fail to set encrypt key\n");
        return 0;
    }

    AES_cbc_encrypt((unsigned char*)in, (unsigned char*)out, in_len, &aes, iv, AES_DECRYPT);
    
    return (in_len - DePKCS5Padding((unsigned char*)out, in_len));
}

/**********************************************************
函数名：base64_encode
参数：char* in_str --输入字符串地址
参数：char* out_str --输出字符串地址
返回值:int --0失败 成功返回编号的字节数
说明：对in_str进行base64编码 输出到out_str
***********************************************************/
int base64_encode(char *in_str, int in_len, char *out_str)
{
    BIO *b64 = NULL, *bio = NULL;
    BUF_MEM *bptr = NULL;
    size_t size = 0;
    
    if (in_str == NULL || out_str == NULL)
        return 0;
    
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, in_str, in_len);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bptr);
    memcpy(out_str, bptr->data, bptr->length);
    out_str[bptr->length] = '\0';
    size = bptr->length;
    BIO_free_all(bio);
    
    return size;
}

/**********************************************************
函数名：base64_decode
参数：char* in_str --输入字符串地址
参数：char* out_str --输出字符串地址
返回值:int --0
说明：对str_in进行base64编码 输出到out_str
***********************************************************/
int base64_decode(char *in_str, char *out_str)
{
    int in_len = strlen(in_str);
    BIO *b64 = NULL;
    BIO *bmem = NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new_mem_buf(in_str, in_len);
    bmem = BIO_push(b64, bmem);
    BIO_read(bmem, out_str, in_len);
    BIO_free_all(bmem);

    return 0;
}

int get_base64_decode_len(char *base64_encode_data)
{
    int i = 0;
    int j = 0;
    char tmp[MAX_PKT_BUFF] = {0};
    int len_b64 = 0;

    /* base64加密过程,每76个字符会添加\r\n,计算解码后长度时需去除,但解码数据不需要去除 */
    for (i = 0; i < strlen(base64_encode_data); i++) {
        if (base64_encode_data[i] != '\n' && base64_encode_data[i] != '\r') {
            tmp[j] = base64_encode_data[i]; //将字符串中与要删除字符不同的字符保留，相同字符则跳过
            j++;
        }
    }

    tmp[j] = '\0'; //在字符串末尾加上'\0'
    
    //printf("base64_encode_data:\n%s\n", tmp);
    
    // 计算解码后的数据长度
    if(strstr(tmp, "==")) {
        len_b64 = strlen(tmp) / 4 * 3 - 2;  
    } else if(strstr(tmp, "=")) {
        len_b64 = strlen(tmp) / 4 * 3 - 1;  
    } else {  
        len_b64 = strlen(tmp) / 4 * 3;
    }

    return len_b64;
}

int aes_cbc_base64_enc_data(char *src, int src_len, char *iv, char *dst, char *key)
{
    int i = 0;
    int len_b64 = 0;
    int enc_len = 0;
    unsigned char enc_key[17] = {0};
    unsigned char enc_iv[17] = {0};
    char enc_data[MAX_PKT_BUFF] = {0};
    char base64_encode_out[MAX_PKT_BUFF] = {0};
    char dec[MAX_PKT_BUFF] = {0};
    
    enc_len = ((src_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

    strcpy(enc_key, key);
    strcpy(enc_iv, iv);


#if 0    
    printf("key:%s ,enc key:%s, iv:%s\n", key, enc_key, enc_iv);

    printf("len:%d, src:\n", src_len);
    for (i = 0; i < src_len; i++) {
        printf("%02x ", (unsigned int)src[i]);
    }
    printf("\n");
#endif

    if (!aes_cbc_encrypt(src, src_len, enc_key, enc_iv, enc_data)) {
        printf("encrypt error\n");
        return 0;
    }
    
#if 0
    printf("len:%d, enc:\n", enc_len);
    for (i = 0; i < enc_len; i++) {
        printf("%02x ", (unsigned int)enc_data[i]);
    }
    printf("\n");
#endif

    base64_encode(enc_data, enc_len, base64_encode_out);

    len_b64 = strlen(base64_encode_out);
    /* base64 加密后会存在回车换行符*/
    if (base64_encode_out[len_b64 - 1] == '\n') {
        base64_encode_out[len_b64 - 1] = '\0';
        len_b64--;
    }
    
    memcpy(dst, base64_encode_out, len_b64);

#if 0    /* 加密后直接解密(测试) */
    printf("base64_encode_out:%s\n", base64_encode_out);

    printf("\n=============================\n");
    /* base64解码 */
    char base64_decode_out[MAX_PKT_BUFF];
    memset(base64_decode_out, 0, sizeof(base64_decode_out));
    base64_decode(base64_encode_out, base64_decode_out);

    printf("base64_dec:\n");
    for (i = 0; i < enc_len; i++) {
        printf("%02x ", (unsigned int)base64_decode_out[i]);
    }

    printf("\n");
    
    /* aes解密 */
    memset(enc_iv, 0, sizeof(enc_iv));
    memcpy(enc_iv, iv, strlen(iv) + 1);
    
    aes_cbc_decrypt(base64_decode_out, enc_len, enc_key, enc_iv, dec);

    printf("dec:\n");
    for (i = 0; i < enc_len; i++) {
        printf("%02x ", (unsigned int)dec[i]);
    }
    printf("\n");
    
    printf("\n=============================\n");
#endif

    return strlen(dst);
}

int aes_cbc_base64_dec_data(char *src, char *iv, char *dst, char *key)
{
    int i = 0;
    int j = 0;
    int dec_len = 0;
    int len_b64 = 0;
    unsigned char dec[MAX_PKT_BUFF] = {0};
    unsigned char dec_key[17] = {0};
    unsigned char enc_iv[17] = {0};
    char base64_decode_out[MAX_PKT_BUFF] = {0};
    char base64_encode_data[MAX_PKT_BUFF] = {0};
    char tmp[MAX_PKT_BUFF] = {0};

    strcpy(dec_key, key);
    strcpy(enc_iv, iv);
    
    strcpy(base64_encode_data, src);

    len_b64 = get_base64_decode_len(base64_encode_data);
    
    base64_encode_data[strlen(base64_encode_data)] = '\n'; /* 单独解码需要增加\n */
    base64_encode_data[strlen(base64_encode_data) + 1] = '\0';
    
    //printf("len:%d, dec: base64_encode_data:\n", strlen(base64_encode_data) - 1);
    //printf("%s\n", base64_encode_data);
    //printf("\n=============================\n");
    
    base64_decode(base64_encode_data, base64_decode_out);

#if 0
    printf("len:%d, base64_dec:\n", len_b64);
    for (i = 0; i < len_b64; i++) {
        printf("%02x ", (unsigned int)base64_decode_out[i]);
    }
    
    printf("\n=============================\n");
    printf("key:%s, dec key:%s, iv:%s\n", key, dec_key, enc_iv);
#endif

    dec_len = aes_cbc_decrypt(base64_decode_out, len_b64, (unsigned char *)dec_key, enc_iv, dec);

#if 0
    printf("dec, dec_len:%d:\n", dec_len);
    for (i = 0; i < dec_len; i++) {
        printf("%c", (unsigned int)dec[i]);
    }

    printf("\n*****************************\n");
#endif

    memcpy(dst, dec, dec_len);
    return dec_len;
}

int main() {
    unsigned char plaintext[] = "00112233445566778899AABBCCDDEEFF";
	unsigned char ciphertext[512] = {0};
    unsigned char out[512] ={0};
    unsigned char key[EVP_MAX_KEY_LENGTH] = "0123456789abcdef";
    unsigned char iv[EVP_MAX_IV_LENGTH] = "1234567890abcdef";
    int plaintext_len = strlen((const char *)plaintext);
    aes_cbc_base64_enc_data(plaintext, plaintext_len, iv, ciphertext, key);
    printf("ciphertext : %s\n",ciphertext);
    aes_cbc_base64_dec_data(ciphertext, iv, out, key);
    printf("plaintext : %s\n",out);

    return 0;
}


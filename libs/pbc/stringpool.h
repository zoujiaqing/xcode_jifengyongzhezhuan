#ifndef PROTOBUF_C_STRINGPOOL_H
#define PROTOBUF_C_STRINGPOOL_H
#ifdef __cplusplus 
extern "C" { 
#endif 
struct _stringpool;

struct _stringpool * _pbcS_new(void);
void _pbcS_delete(struct _stringpool *pool);
const char * _pbcS_build(struct _stringpool *pool, const char * str , int sz);
#ifdef __cplusplus 
} 
#endif 
#endif

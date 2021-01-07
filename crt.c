// #include <stdarg.h>
// #include <stdio.h> 
// #include <ntddk.h> 


#ifdef __cplusplus
extern "C" {
#endif


// //////////////////////////////////////////////////
// //                   Wine/CRT

// #define MSVCRT_EINVAL    22
// #define MSVCRT_ERANGE    34
// #define MSVCRT_STRUNCATE 80
// #define MSVCRT__TRUNCATE ((size_t)-1)

// #define MSVCRT_INVALID_PMT(x,err)   ()
// #define MSVCRT_CHECK_PMT_ERR(x,err) (x)
// #define MSVCRT_CHECK_PMT(x)         MSVCRT_CHECK_PMT_ERR((x), MSVCRT_EINVAL) 

// #define _CALL_INVALID_PARAMETER(expr) _invalid_parameter_noinfo()
// #define _INVALID_PARAMETER(expr) _CALL_INVALID_PARAMETER(expr)

// #define _VALIDATE_RETURN_VOID( expr, errorcode )                               \
//     {                                                                          \
//         int _Expr_val=!!(expr);                                                \
//         if ( !( _Expr_val ) )                                                  \
//         {                                                                      \
//             return;                                                            \
//         }                                                                      \
//     }

// #define _VALIDATE_RETURN( expr, errorcode, retexpr )                           \
//     {                                                                          \
//         int _Expr_val=!!(expr);                                                \
//         if ( !( _Expr_val ) )                                                  \
//         {                                                                      \
//             return ( retexpr );                                                \
//         }                                                                      \
//     }

// #define _SECURECRT__FILL_STRING(_String, _Size, _Offset)

// typedef int (*OUTPUTFN)(FILE * _File, const char *, _locale_t, va_list);


// #define INT_MAX       2147483647    /* maximum (signed) int value */
// #define MAXSTR INT_MAX

// #define _T(x) x

// errno_t __cdecl
// wcsncpy_s_k8 (                               
//     wchar_t* wcDest,
//     size_t numElement,
//     const wchar_t *wcSrc,
//     size_t count )
// {
//     WCHAR *p = wcDest;
//     BOOLEAN truncate = (count == MSVCRT__TRUNCATE);

//     if(!wcDest && !numElement && !count)
//         return 0;

//     if (!wcDest || !numElement)
//         return MSVCRT_EINVAL;

//     if (!wcSrc)
//     {
//         *wcDest = 0;
//         return count ? MSVCRT_EINVAL : 0;
//     }

//     while (numElement && count && *wcSrc)
//     {
//         *p++ = *wcSrc++;
//         numElement--;
//         count--;
//     }
//     if (!numElement && truncate)
//     {
//         *(p-1) = 0;
//         return MSVCRT_STRUNCATE;
//     }
//     else if (!numElement)
//     {
//         *wcDest = 0;
//         return MSVCRT_ERANGE;
//     }

//     *p = 0;
//     return 0; 
// }


// size_t __cdecl
// wcsnlen_k8 (
//     const wchar_t *s,
//     size_t maxlen )
// {
//     size_t i;

//     for (i = 0; i < maxlen; i++)
//         if (!s[i]) break;
//     return i;
// }


// errno_t  __cdecl
// memcpy_s_k8 (
//     void * dest,
//     size_t numberOfElements,
//     const void * src,
//     size_t count )
// {
//     if(!count)
//         return 0;

//     if (!MSVCRT_CHECK_PMT(dest != NULL)) return MSVCRT_EINVAL;
//     if (!MSVCRT_CHECK_PMT(src != NULL))
//     {
//         memset(dest, 0, numberOfElements);
//         return MSVCRT_EINVAL;
//     }
//     if (!MSVCRT_CHECK_PMT_ERR( count <= numberOfElements, MSVCRT_ERANGE ))
//     {
//         memset(dest, 0, numberOfElements);
//         return MSVCRT_ERANGE;
//     }

//     memmove(dest, src, count);
//     return 0;
// } 


// errno_t __cdecl
// strncpy_s_k8 (
//     char *dest,
//     size_t numberOfElements,
//     const char *src,
//     size_t count)
// {
//     size_t i, end;

//     if(!count) {
//         if(dest && numberOfElements)
//             *dest = 0;
//         return 0;
//     }

//     if (!MSVCRT_CHECK_PMT(dest != NULL)) return MSVCRT_EINVAL;
//     if (!MSVCRT_CHECK_PMT(src != NULL)) return MSVCRT_EINVAL;
//     if (!MSVCRT_CHECK_PMT(numberOfElements != 0)) return MSVCRT_EINVAL;

//     if(count!=MSVCRT__TRUNCATE && count<numberOfElements)
//         end = count;
//     else
//         end = numberOfElements-1;

//     for(i=0; i<end && src[i]; i++)
//         dest[i] = src[i];

//     if(!src[i] || end==count || count==MSVCRT__TRUNCATE) {
//         dest[i] = '\0';
//         return 0;
//     }

//     dest[0] = '\0';
//     return MSVCRT_EINVAL;
// }

// errno_t __cdecl
// strcpy_s_k8 (
//     char* dst,
//     size_t elem,
//     const char* src )
// {
//     size_t i;
//     if(!elem) return MSVCRT_EINVAL;
//     if(!dst) return MSVCRT_EINVAL;
//     if(!src)
//     {
//         dst[0] = '\0';
//         return MSVCRT_EINVAL;
//     }

//     for(i = 0; i < elem; i++)
//     {
//         if((dst[i] = src[i]) == '\0') return 0;
//     }
//     dst[0] = '\0';
//     return MSVCRT_ERANGE;
// } 


// int __cdecl
// sprintf_s_k8 (
//    TCHAR *buffer,
//    size_t sizeOfBuffer,
//    const TCHAR *format,
//    ... )
// {
//     int res;

//     va_list va;
//     va_start(va, format);
//     res = vsprintf(buffer, format, va);
//     va_end(va);

//     return res;
// }

// int __cdecl
// vsprintf_s_k8 (
//     char * _DstBuf,
//     size_t _Size,
//     const char * _Format,
//     va_list _ArgList)
// {
//     return vsprintf(_DstBuf, _Format, _ArgList);
// }


// char * __cdecl
// strtok_s_k8 (
//     char *str,
//     const char *delim,
//     char **ctx)
// {
//     if (!MSVCRT_CHECK_PMT(delim != NULL)) return NULL;
//     if (!MSVCRT_CHECK_PMT(ctx != NULL)) return NULL;
//     if (!MSVCRT_CHECK_PMT(str != NULL || *ctx != NULL)) return NULL;

//     if(!str)
//         str = *ctx;

//     while(*str && strchr(delim, *str))
//         str++;
//     if(!*str)
//     {
//         *ctx = str;
//         return NULL;
//     }

//     *ctx = str+1;
//     while(**ctx && !strchr(delim, **ctx))
//         (*ctx)++;
//     if(**ctx)
//         *(*ctx)++ = 0;

//     return str;
// }

// int __cdecl
// _snprintf_s_k8 (
//     char    *s,
//     size_t  DstSize,
//     size_t  MaxCount,
//     const char *format,
//     ...)
// {
//     int res;

//     va_list va;
//     va_start(va, format);
//     res = _vsnprintf( s, DstSize, format, va );
//     va_end(va);

//     return res;
//  }    
 
#ifdef __cplusplus
}
#endif
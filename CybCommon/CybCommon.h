#ifndef CYBCOMMON_H
#define CYBCOMMON_H

/** @file
 * @brief Cybermals Engine - Common Definitions
 */

//Library Exports
#ifdef _MSC_VER
    #ifdef DLL_EXPORT
        #define CYBAPI __declspec(dllexport)
    #else
        #define CYBAPI __declspec(dllimport)
    #endif
#else
    #define CYBAPI extern
#endif

//TRUE and FALSE (some compilers don't define these)
#ifndef TRUE
    #define TRUE  1
    #define FALSE 0
#endif

//Generic Error Codes
enum
{
    CYB_NO_ERROR,
    CYB_ERROR
};

#endif
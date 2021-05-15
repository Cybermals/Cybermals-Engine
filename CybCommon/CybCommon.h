#ifndef CYBCOMMON_H
#define CYBCOMMON_H

/** @file
 * @brief Cybermals Engine - Common Definitions
 */
 
/** @addtogroup CybCommon
 * @brief Cybermals Engine - Common Definitions
 * @{
 */
 
//Version Info
/** @brief Cybermals Engine version float.
 */
#define CYB_VERSION        0.3f

/** @brief Cybermals Engine version string.
 */
#define CYB_VERSION_STRING "0.3.0a"

//Library Exports
#ifdef _MSC_VER
    #ifdef DLL_EXPORTS
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
/** @brief Error codes used by the Cybermals Engine.
 */
enum Cyb_ErrorCodes
{
    CYB_NO_ERROR, /**< No error occurred. */
    CYB_ERROR     /**< A generic error occurred. */
};

/**
 * @}
 */

#endif
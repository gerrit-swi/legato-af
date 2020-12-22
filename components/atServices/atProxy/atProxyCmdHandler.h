/**
 * @file atProxyCmdHandler.h
 *
 * AT Proxy Command Handling interface.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_AT_PROXY_CMD_HANDLER_H_INCLUDE_GUARD
#define LE_AT_PROXY_CMD_HANDLER_H_INCLUDE_GUARD

#include "atProxy.h"
#include "pa_port.h"

//--------------------------------------------------------------------------------------------------
/**
 * AT parser tokens
 */
//--------------------------------------------------------------------------------------------------
#define AT_TOKEN_EQUAL  '='
#define AT_TOKEN_CR  0x0D
#define AT_TOKEN_BACKSPACE  0x08
#define AT_TOKEN_QUESTIONMARK  '?'
#define AT_TOKEN_SEMICOLON  ';'
#define AT_TOKEN_COMMA  ','
#define AT_TOKEN_QUOTE  0x22
#define AT_TOKEN_BACKSLASH  0x5C
#define AT_TOKEN_SPACE  0x20

// Maximum number of parameters supported per AT Cmd
#define AT_PROXY_PARAMETER_LIST_MAX   5

// Some component such as PA doesn't know le_atServer_ServerCmdRef_t
#ifndef le_atServer_ServerCmdRef_t
    typedef struct le_atServer_ServerCmd* le_atServer_ServerCmdRef_t;
#endif

typedef enum
{
    PARSER_SEARCH_A,
    PARSER_SEARCH_T,
    PARSER_SEARCH_CR

} le_atProxy_RxParserState_t;

// AT Proxy Command Session structure
typedef struct le_atProxy_AtCommandSession
{
    le_atProxy_PortRef_t port;  ///< The connected AT port
    le_atServer_CmdRef_t ref; ///< Command session reference
    char command[LE_ATDEFS_COMMAND_MAX_BYTES];  ///< cmd found in input string
    le_atProxy_RxParserState_t rxState;       ///< input string parser state
    uint16_t index;           ///< Parse Buffer index
    uint16_t operatorIndex;   ///< Index of operator ("=" or "?")
    le_atServer_Type_t type;  ///< AT Command type (i.e. Action, parameter, read, or test)
    bool local;               ///< Indicates if this is a "local" or "remote" AT Command
    uint32_t registryIndex;   ///< For "local" commands, index of AT Cmd in Registry
    char atCmdParameterList[AT_PROXY_PARAMETER_LIST_MAX][LE_ATDEFS_PARAMETER_MAX_BYTES];
                              ///< Parameter list
    uint32_t parameterIndex;  ///< Parameter index (count)
    bool active;              ///< Indicates if this session is active (i.e., in processing)
    bool dataMode;            ///< Indicates if current session is in data mode
    le_dls_List_t unsolicitedList;           ///< unsolicited list to be sent
} le_atProxy_AtCommandSession_t;


// Initialize the AT Port Command Handler
void atProxyCmdHandler_Init(void);

//--------------------------------------------------------------------------------------------------
/**
 * Complete the current AT command session
 *
 * @return none
 */
//--------------------------------------------------------------------------------------------------
void atProxyCmdHandler_Complete
(
    struct le_atProxy_AtCommandSession* atCmdPtr   ///< [IN] AT Command Session Pointer
);

//--------------------------------------------------------------------------------------------------
/**
 * Start AT command data mode
 *
 * @return none
 */
//--------------------------------------------------------------------------------------------------
void atProxyCmdHandler_StartDataMode
(
    struct le_atProxy_AtCommandSession* atCmdPtr   ///< [IN] AT Command Session Pointer
);

//--------------------------------------------------------------------------------------------------
/**
 * Check if the current session is local and active
 *
 * @return true if current session is local and active, false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool atProxyCmdHandler_IsLocalSessionActive
(
    struct le_atProxy_AtCommandSession* atCmdPtr   ///< [IN] AT Command Session Pointer
);

//--------------------------------------------------------------------------------------------------
/**
 * Check if the current session is active
 *
 * @return true if current session is active, false otherwise
 */
//--------------------------------------------------------------------------------------------------
bool atProxyCmdHandler_IsActive
(
    struct le_atProxy_AtCommandSession* atCmdPtr   ///< [IN] AT Command Session Pointer
);

//--------------------------------------------------------------------------------------------------
/**
 * Parse incoming characters
 *
 * @return none
 */
//--------------------------------------------------------------------------------------------------
void atProxyCmdHandler_ParseBuffer
(
    struct le_atProxy_AtCommandSession* atCmdPtr,  ///< [IN] AT Command Session Pointer
    uint32_t count                                 ///< [IN] Number of new characters
);

//--------------------------------------------------------------------------------------------------
/**
 * Send or queue local unsolicited responses
 *
 * @return none
 */
//--------------------------------------------------------------------------------------------------
void atProxyCmdHandler_SendUnsolicitedResponse
(
    le_atServer_ServerCmdRef_t cmdRef,             ///< [IN] Asynchronous Server Command Reference
    const char* responseStr,                       ///< [IN] Unsolicited Response String
    struct le_atProxy_AtCommandSession* atCmdPtr   ///< [IN] AT Command Session Pointer
);

//--------------------------------------------------------------------------------------------------
/**
 * Open an AT command session
 *
 * @return Reference to the requested AT command session if successful, NULL otherwise
 */
//--------------------------------------------------------------------------------------------------
le_atProxy_AtCommandSession_t* atProxyCmdHandler_OpenSession
(
    le_atProxy_PortRef_t port   ///< [IN] AT port for AT session to be opened
);


#endif /* LE_AT_PROXY_CMD_HANDLER_H_INCLUDE_GUARD */
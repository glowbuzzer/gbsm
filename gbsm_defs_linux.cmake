
#this is the process name we try and find to send signals to
SET(GBC_PROCESS_NAME "GBC")


#defines for which signal to send to GBC */
SET(SIGNAL_TO_SEND SIGIO)
#SET(SIGNAL_TO_SEND SIGPWR)
#RT signals require some jiggerpokery in GBC
#SET(SIGNAL_TO_SEND SIGRTMIN)

#N.B. SIGRTMIN is higher priority than SIGRTMAX if you are trying to use the RT signals */


SET(GBC_SHARED_MEMORY_NAME "gbc_shared_memory")



#Enables debug logging overall
add_compile_definitions(ENABLE_LOGGING=1)

#Sets the debug log level
#LOG_LVL_FATAL = 0
#LOG_LVL_ERROR = 1
#LOG_LVL_WARNING =  2
#LOG_LVL_INFO = 3
#LOG_LVL_DEBUG = 4
#LOG_LVL_TRACE = 5

add_compile_definitions(LOG_LEVEL=5)

#Enables user messages (standard messages that describe what is going on in GBEM output to console, log, syslog etc.)
add_compile_definitions(GBSM_UM_EN=1)

#Enables State Machine (SM) logging for GBSM (this produces voluminous output)
add_compile_definitions(GBSM_SM_LOG_EN=0)

#Enables general GBSM debug logging (general logging statements in key places)
add_compile_definitions(GBSM_GEN_LOG_EN=1)

#Enables drive error debug logging
add_compile_definitions(GBSM_DRIVE_STATUS_ERROR_LOG_EN=1)


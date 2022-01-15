
# system variables



#d1 commands for mega must be defined in mega_transfer.ino
TO_SS1 = 0xC1
TO_SS2 = 0xC2
TO_SS3 = 0xC3
SEND_DATA_TRUE = 0xD1
SEND_DATA_FALSE = 0xD0

# d2 commands for nano, must be defined in nano_firmware.ino
SET_POS_REF = 0x01
GET_MOTOR_POS = 0xEE
ENABLE_MOTOR_F = 0xAF
ENABLE_MOTOR_E = 0xAE
DISABLE_MOTOR_F = 0xBF
DISABLE_MOTOR_E = 0xBE

LED_DEBUG = 0xCC

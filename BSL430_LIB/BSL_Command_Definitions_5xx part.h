// BSL_Command_Definitions_5xx.h  ( part ) 2021 Dec 13
/*
 * MSP430 USB Firmware Upgrade Example
 *
 * An example software to field firmware upgrade a MSP430 USB based device
 *
 * Copyright (C) {2010} Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#define ROM_ACK            0x90
#define RX_PASSWORD        0x10
#define RX_DATA_BLOCK      0x12
#define TX_DATA_BLOCK      0x14
#define ERASE_SEGMENT      0x16
#define ERASE_MAIN_OR_INFO 0x16
#define MASS_ERASE         0x18
#define ERASE_CHECK        0x1C
#define SET_MEM_OFFSET     0x21

//Received commands
#define RX_DATA_BLOCK  0x10
#define RX_PASSWORD    0x11
#define ERASE_SEGMENT  0x12
#define TOGGLE_INFO    0x13
#define ERASE_BLOCK    0x14
#define MASS_ERASE     0x15
#define CRC_CHECK      0x16
#define LOAD_PC        0x17
#define TX_DATA_BLOCK  0x18
#define TX_BSL_VERSION 0x19
#define TX_BUFFER_SIZE 0x1A
#define RX_DATA_BLOCK_FAST 0x1B

// PI
#define CHANGE_BAUD_RATE 0x52
#define BAUD430_4800   0x01
#define BAUD430_9600   0x02
#define BAUD430_19200  0x03
#define BAUD430_38400  0x04
#define BAUD430_57600  0x05
#define BAUD430_115200 0x06

//Responses
#define BSL_DATA_REPLY    0x3A
#define BSL_MESSAGE_REPLY 0x3B
#define FLASH_ACK         SUCCESSFUL_OPERATION

// Error Codes
// - From the API
#define SUCCESSFUL_OPERATION 0x00
#define NO_EXCEPTIONS 0x00
#define FLASH_WRITE_CHECK_FAILED 0x01
#define FLASH_FAIL_BIT_SET 0x02
#define VOLTAGE_CHANGE_DURING_PROGRAM 0x03
#define BSL_LOCKED 0x04
#define BSL_PASSWORD_ERROR 0x05
#define BYTE_WRITE_FORBIDDEN 0x06

// - From the Command Interpreter
#define UNKNOWN_COMMAND 0x07
#define LENGTH_TOO_BIG_FOR_BUFFER 0x08

#define HEADER_INCORRECT 0x51
#define CHECKSUM_INCORRECT 0x52
#define PACKET_SIZE_ZERO 0x53
#define PACKET_SIZE_TOO_BIG 0x54
#define UNKNOWN_ERROR 0x55

// errors for PI commands
#define UNKNOWN_BAUD_RATE 0x56

#define OPERATION_SUCCESSFUL 0
#define TXT_EOF -1
#define ERROR_OPENING_FILE -2
#define NO_DATA_READ -3

static DataBlock defaultPass = {
   {
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF
   },
   32,
   0xFFE0
};

static DataBlock defaultPass5438 = {
   {
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,
   (unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF,(unsigned char)0xFF
   },
   16,
   0xFFF0
};
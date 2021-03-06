/*
 * MSP430 USB Firmware Upgrade Example
 *
 * An example software to field firmware upgrade a MSP430 USB based device
 *
 * Copyright (C) {2010} Texas Instruments Incorporated - http://www.ti.com/
 *
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
#include "BSL_Commands_5xx.h"
#include "BSL_Command_Definitions_5xx.h"
#include "BSL_IO_UART.h"
#include <stdio.h>

unsigned char getBSL_Response( unsigned char PI_response );

dataBuffer TXBuffer;
dataBuffer RXBuffer;
unsigned int answer;

unsigned char (*BSL_RX_Packet)( dataBuffer *db );
unsigned char (*BSL_TX_Packet)( dataBuffer  db );

/*******************************************************************************
 *******************************************************************************/
dataBuffer Flash_get_RX_Buffer()
{
    return RXBuffer;
}
/*******************************************************************************
 *******************************************************************************/
void Flash_set_TX_Packet( unsigned char (*txFunc)( dataBuffer  db ) )
{
    BSL_TX_Packet = txFunc;
}

/*******************************************************************************
 *******************************************************************************/
void Flash_set_RX_Packet( unsigned char (*rxFunc)( dataBuffer  *db ) )
{
    BSL_RX_Packet = rxFunc;
}

/*******************************************************************************
 *Function:    Flash_TX_BufferSize
 *Description: Calculates the difference between startTime and the actual windows time
 *Parameters:
 *             none
 *             char* comPort         a string for the COM port ie "COM1"
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is filled correctly
 *DataBuffer:
 *             [1]                   The low byte of the BSL's buffer size
 *             [2]                   The high byte of the BSL's buffer size
 *******************************************************************************/
unsigned char Flash_TX_BufferSize()
{
    TXBuffer.data[0] = TX_BUFFER_SIZE;
    TXBuffer.size = 1;
    answer = BSL_TX_Packet(TXBuffer);
    if( answer != FLASH_ACK )
    {
        return answer;
    }
    else                                      // FLASH_ACK means command was received
    {
        answer = BSL_RX_Packet(&RXBuffer);              // Now get BSL Reply data
        if( answer != FLASH_ACK )
        {
            return answer;
        }
        else if (RXBuffer.data[0] != BSL_DATA_REPLY)
        {
            return RXBuffer.data[1];               // Received an error reply
        }
        else
        {
            return FLASH_ACK;                         // Data buffer filled correctly,
        }
    }
}

/*******************************************************************************
 *Function:    Flash_TX_BSL_Version
 *Description: Transmits the BSL version command, and fills data buffer with reply
 *Parameters:
 *             DWORD startTime       the Start time
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is filled correctly
 *DataBuffer:
 *             [1]                   Vendor Field
 *             [2]                   Command Interpreter Version Field
 *             [3]                   430API Version Field
 *             [4]                   Peripheral Interface Version Field
 *******************************************************************************/
unsigned char Flash_TX_BSL_Version()
{
    TXBuffer.data[0] = TX_BSL_VERSION;
    TXBuffer.size = 1;
    answer = BSL_TX_Packet(TXBuffer);
    if( answer != FLASH_ACK )
    {
        return answer;
    }
    else
    {
        answer = BSL_RX_Packet(&RXBuffer);
        if( answer != FLASH_ACK )
        {
            return answer;
        }
        else if( RXBuffer.data[0] != BSL_DATA_REPLY )
        {
            return RXBuffer.data[1];
        }
        else
        {
            return FLASH_ACK;
        }
    }
}

/*******************************************************************************
 *Function:    Flash_TX_DataBlock
 *Description: Transmits a block of data from the BSL using the dataBuffer
 *Parameters:
 *             int addr              The Address where the data should begin
 *             int length            The length of data in the data block
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is filled correctly
 *******************************************************************************/
unsigned char Flash_TX_DataBlock( unsigned long int addr, unsigned long int length, unsigned char *buffer )
{
    int answer;
#ifdef DEBUG_VERBOSE
    printf( "\n-------------------------------------\n");
    printf( "Requesting Data Block...\n");
#endif
    TXBuffer.data[0] = TX_DATA_BLOCK;
    TXBuffer.data[1] = (unsigned char)(addr &0xFF);
    TXBuffer.data[2] = (unsigned char)(addr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(addr>>16 &0xFF);
    TXBuffer.data[4] = (unsigned char)(length &0xFF);
    TXBuffer.data[5] = (unsigned char)(length>>8 &0xFF);
    TXBuffer.size = 6;
    //printf( "\nhere 1\n");
    answer = BSL_TX_Packet(TXBuffer);
    if( answer != FLASH_ACK )
    {
        return answer;
    }
    else
    {
        unsigned int buffer_pointer = 0;
        //printf( "\nhere 2\n");
        for( buffer_pointer = 0; buffer_pointer < length;)
        {
            answer = BSL_RX_Packet(&RXBuffer);
            if( answer != FLASH_ACK )
            {
                //printf( "\nhere 3 - error\n");
                return answer;
            }
            else if( RXBuffer.data[0] == BSL_DATA_REPLY )
            {
                unsigned int i = 0;
                //printf( "\nGot one buffer, size: %u\n", RXBuffer.size);
                for( i = 1; i< RXBuffer.size; i++, buffer_pointer++)
                {
                    buffer[buffer_pointer] = RXBuffer.data[i];
                    //printf( "|%2.2x|",buffer[buffer_pointer]);
                }
            }
            else
            {
                return 1;  // to do, return NAK or something
            }
        } // streaming RX loop

        return 0;
    }
}

/*******************************************************************************
 *Function:    Flash_LoadPC
 *Description: Sends a command to set the MSP430's Program counter to a location
 *Parameters:
 *             int addr              The address to which the PC should be set
 *Returns:
 *             FLASH_ACK                   the 430 received the command
 *                                   the device can not be counted on being responsive after
 *******************************************************************************/
unsigned char Flash_LoadPC( unsigned long int addr )
{
    TXBuffer.data[0] = LOAD_PC;
    TXBuffer.data[1] = (unsigned char)(addr &0xFF);
    TXBuffer.data[2] = (unsigned char)(addr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(addr>>16 &0xFF);
    TXBuffer.size = 4;
    return BSL_TX_Packet(TXBuffer);
}

/*******************************************************************************
 *Function:    Flash_CRC_Check
 *Description: Calculates a CRC check on the device
 *Parameters:
 *             int addr              The address for the first byte included in the CRC
 *             int length            The number of bytes in the CRC
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is filled correctly
 *DataBuffer:
 *             [1]                   low bits of 16 bit crc
 *             [2]                   high bits of 16 bit crc
 *******************************************************************************/
unsigned char Flash_CRC_Check( unsigned long int addr, unsigned long int length )
{
    TXBuffer.data[0] = CRC_CHECK;
    TXBuffer.data[1] = (unsigned char)(addr &0xFF);
    TXBuffer.data[2] = (unsigned char)(addr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(addr>>16 &0xFF);
    TXBuffer.data[4] = (unsigned char)(length &0xFF);
    TXBuffer.data[5] = (unsigned char)(length>>8 &0xFF);
    TXBuffer.size = 6;
    answer = BSL_TX_Packet(TXBuffer);
    if( answer != FLASH_ACK )
    {
        return answer;
    }
    else
    {
        answer = BSL_RX_Packet(&RXBuffer);
        if( answer != FLASH_ACK )
        {
            return answer;
        }
        else if( RXBuffer.data[0] != BSL_DATA_REPLY )
        {
            return RXBuffer.data[1];
        }
        else
        {
            return FLASH_ACK;
        }
    }
}

/*******************************************************************************
 *Function:    Flash_massErase
 *Description: sends the mass erase command to the device
 *Parameters:
 *             none
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is sent correctly
 *******************************************************************************/
//to do: wait for reply for FLASH_ACK?
unsigned char Flash_massErase()
{
    TXBuffer.data[0] = MASS_ERASE;
    TXBuffer.size = 1;
    return getBSL_Response( BSL_TX_Packet(TXBuffer) );
}

/*******************************************************************************
 *Function:    Flash_toggleInfo
 *Description: Toggles the 430 info lock
 *Parameters:
 *             none
 *Returns:
 *             char                  An FLASH_ACK if the dataBuffer is sent correctly
 *******************************************************************************/
//to do: wait for reply for FLASH_ACK?
unsigned char Flash_toggleInfo()
{
    TXBuffer.data[0] = TOGGLE_INFO;
    TXBuffer.size = 1;
    return getBSL_Response( BSL_TX_Packet(TXBuffer));
}

/*******************************************************************************
 *Function:    Flash_eraseSegment
 *Description: Erases a segment of device flash
 *Parameters:
 *             int addr              The address within a flash segment to erase
 *Returns:
 *             char                  An FLASH_ACK if the flash was correctly erased
 *******************************************************************************/
unsigned char Flash_eraseSegment( unsigned long int addr )
{
    TXBuffer.data[0] = ERASE_SEGMENT;
    TXBuffer.data[1] = (unsigned char)(addr &0xFF);
    TXBuffer.data[2] = (unsigned char)(addr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(addr>>16 &0xFF);
    TXBuffer.size = 4;
    return getBSL_Response(BSL_TX_Packet(TXBuffer));
}

/*******************************************************************************
 *Function:    Flash_RX_Password_5438
 *Description: Sends a 16 byte password, only for the 5438 device
 *Parameters:
 *             char* block           A pointer to a 16 byte 'password' array
 *Returns:
 *             char                  An FLASH_ACK if the device was correctly unlocked
 *
 *Note:        An incorrect password will cause flash+RAM erase, so this function
 *             could return a timeout as the MSP430 goes unresponsive
 *******************************************************************************/
unsigned char Flash_RX_Password_5438( DataBlock data )
{
    int i;
    TXBuffer.data[0] = RX_PASSWORD;
    for( i = 0; i < 16; i++ )
    {
        TXBuffer.data[i+1] = data.data[i];
    }
    TXBuffer.size = 17;
    return getBSL_Response(BSL_TX_Packet(TXBuffer));
}

/*******************************************************************************
 *Function:    Flash_RX_Password
 *Description: Sends a 32 byte password, only for the 5438 device
 *Parameters:
 *             char* block           A pointer to a 32 byte 'password' array
 *Returns:
 *             char                  An FLASH_ACK if the device was correctly unlocked
 *
 *Note:        An incorrect password will cause flash+RAM erase, so this function
 *             could return a timeout as the MSP430 goes unresponsive
 *******************************************************************************/
unsigned char Flash_RX_Password( DataBlock data )
{
    int i;
    TXBuffer.data[0] = RX_PASSWORD;
    for( i = 0; i < 32; i++ )
    {
        TXBuffer.data[i+1] = data.data[i];
    }
    TXBuffer.size = 33;
    return getBSL_Response(BSL_TX_Packet(TXBuffer));
}

/*******************************************************************************
 *Function:    Flash_RX_DataBlock
 *Description: Programs a block of data to the MSP430 memory
 *Parameters:
 *             int addr              The address to begin the data block in the 430
 *             int blockSize         The number of bytes
 *             char* block           a pointer to an array of bytes
 *Returns:
 *             DWORD                 The difference between start Time and current time
 *******************************************************************************/
unsigned char Flash_RX_DataBlock(DataBlock data)
{
    unsigned int i;

#ifdef DEBUG_VERBOSE
    printf( "\n-------------------------------------\n");
    printf( "Writing Data Block...\n");
#endif
    TXBuffer.data[0] = RX_DATA_BLOCK;
    TXBuffer.data[1] = (unsigned char)(data.startAddr &0xFF);
    TXBuffer.data[2] = (unsigned char)(data.startAddr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(data.startAddr>>16 &0xFF);
    for(i = 0; i < data.numberOfBytes; i++ )
    {
        TXBuffer.data[i+4] = data.data[i];
    }
    TXBuffer.size = data.numberOfBytes+4;
    return getBSL_Response(BSL_TX_Packet(TXBuffer));
}
/*******************************************************************************
 *Function:    Flash_RX_DataBlock_Fast
 *Description: Programs a block of data to the MSP430 memory
 *Parameters:
 *             int addr              The address to begin the data block in the 430
 *             int blockSize         The number of bytes
 *             char* block           a pointer to an array of bytes
 *Returns:
 *             DWORD                 The difference between start Time and current time
 *******************************************************************************/
unsigned char Flash_RX_DataBlock_Fast (DataBlock data)
{
    unsigned int i;

#ifdef DEBUG_VERBOSE
    printf( "\n-------------------------------------\n");
    printf( "Writing Data Block...\n");
#endif

    TXBuffer.data[0] = RX_DATA_BLOCK_FAST;
    TXBuffer.data[1] = (unsigned char)(data.startAddr &0xFF);
    TXBuffer.data[2] = (unsigned char)(data.startAddr>>8 &0xFF);
    TXBuffer.data[3] = (unsigned char)(data.startAddr>>16 &0xFF);

    int somethingToSend = 0;
    for(i = 0; i < data.numberOfBytes; i++ )
    {
        TXBuffer.data[i+4] = data.data[i];
        if (data.data[i] != 0xFF)
        {
            somethingToSend = 1;
        }
    }
    TXBuffer.size = data.numberOfBytes+4;

    // do not program the FLASH is the data block is all 0xFF's
    if (somethingToSend)
    {
        //printf( "Sending\n");
        BSL_TX_Packet(TXBuffer);
        //printf( "Done\n");
    }
    return BSL_ACK;
}
/*******************************************************************************
 *Function:    Flash_changeBaudRate
 *Description: Changes 430 and PC baud rate for communication
 *Parameters:
 *             int rate              an in describing the rate to use (see header for details)
 *Returns:
 *             FLASH_ACK                   The baud rate was changed correctly
 *******************************************************************************/

unsigned char Flash_changeBaudRate( unsigned int rate )
{
    unsigned char answer;
    unsigned int rateDef;
#ifdef DEBUG_VERBOSE
    printf( "\n-------------------------------------\n");
    printf( "Changing Baud Rate...\n");
#endif
    switch( rate )
    {
        case 4800:
        rateDef = BAUD430_4800;
        break;
        case 9600:
        rateDef = BAUD430_9600;
        break;
        case 19200:
        rateDef = BAUD430_19200;
        break;
        case 38400:
        rateDef = BAUD430_38400;
        break;
        case 57600:
        rateDef = BAUD430_57600;
        break;
        case 115200:
        rateDef = BAUD430_115200;
        break;
        default:
        rateDef = 0xFF;
        break;
    }
    TXBuffer.data[0] = CHANGE_BAUD_RATE;
    TXBuffer.data[1] = rateDef;
    TXBuffer.size = 2;
    answer = BSL_TX_Packet(TXBuffer);
    if( answer == FLASH_ACK )
    {
        changeCommBaudRate( rate );
    }
    return answer;
}

/*******************************************************************************
 *Function:    getBSL_Response
 *Description: Checks for correct response of both the PI and Core, returns any errors
 *Parameters:
 *             char PI_response   The response from the PI, so a function can be embedded
 *Returns:
 *             an FLASH_ACK or error
 *******************************************************************************/
unsigned char getBSL_Response( unsigned char PI_response )
{
    if( PI_response != FLASH_ACK )
    {
        return PI_response;
    }
    else
    {
        PI_response = BSL_RX_Packet(&RXBuffer);
        if( PI_response != FLASH_ACK )
        {
            printf("FAIL(%2.2x)\n",PI_response);
            return PI_response;
        }
        else if ( RXBuffer.data[1] != FLASH_ACK )
        {
            return RXBuffer.data[1];
        }
        else
        {
            return FLASH_ACK;
        }
    }
}

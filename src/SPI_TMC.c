
/**
 ******************************************************************************
 * @file           :  SPI_TMC.c
 * @brief          :  functions to SPI read/write to the tmc4361a
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright 2022 Glowbuzzer.
 * All rights reserved.</center></h2>
 *
 ******************************************************************************
 */


#include "pigpio.h"
#include "SPI_TMC.h"
#include "TMC4361A_Register.h"
#include "gbsm_config.h"
#include "gpio.h"
#include "log.h"
unsigned int hspi1;


#define SPI_ASSERT gpioWrite(PIN_TMC4361A_SPI_CS, 0);
#define SPI_DEASSERT  gpioWrite(PIN_TMC4361A_SPI_CS, 1);

//Function declarations for static fns used in this CU
static int32_t tmc40bit_read_int32(uint8_t address, const unsigned int *hspi);
static uint32_t tmc40bit_read_uint32(uint8_t address, const unsigned int *hspi);
static uint8_t tmc40bit_write_int32(uint8_t address, int32_t value, const unsigned int *hspi);
static uint8_t tmc40bit_write_uint32(uint8_t address, uint32_t value, const unsigned int *hspi);

void tmc4361A_write_int32_to_all_same_val(const uint8_t address, const int32_t value, const uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status){
    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        spi_status[total_num_in_chain-i] = tmc40bit_write_int32(address, value, hspi);
    }
    SPI_DEASSERT
}

void tmc4361A_write_int32_to_all_different_vals(const uint8_t address, const int32_t *values, const uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status){
    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        spi_status[total_num_in_chain-i] = tmc40bit_write_int32(address, values[i], hspi);

    }
    SPI_DEASSERT
}

void tmc4361A_write_uint32_to_all_same_val(const uint8_t address, const uint32_t value, const uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status){

    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        spi_status[total_num_in_chain-i] = tmc40bit_write_uint32(address, value, hspi);
    }
    SPI_DEASSERT
}


void tmc4361A_write_uint32_to_all_different_vals(const uint8_t address, const uint32_t *values, const uint8_t total_num_in_chain, const unsigned int *hspi, uint8_t *spi_status){

    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        spi_status[total_num_in_chain-i] = tmc40bit_write_uint32(address, values[i], hspi);

    }
    SPI_DEASSERT
}


void tmc4361A_write_int32_to_individual(const uint8_t address, const int32_t value, const uint8_t pos_in_chain, const unsigned int *hspi, uint8_t *spi_status){
    int8_t i = 1;
    SPI_ASSERT

         tmc40bit_write_int32(address, value, hspi);

    //shift to correct drive chip in chain
    while(i < pos_in_chain) {
        tmc40bit_write_int32(0, 0, hspi);
        i++;
    }
    SPI_DEASSERT

}


void tmc4361A_write_uint32_to_individual(const uint8_t address, const uint32_t value, const uint8_t pos_in_chain, const unsigned int *hspi, uint8_t *spi_status){
    int8_t i = 1;
    SPI_ASSERT

    tmc40bit_write_uint32(address, value, hspi);

    //shift to correct drive chip in chain
    while(i < pos_in_chain) {
        tmc40bit_write_uint32(0, 0, hspi);
        i++;
    }
    SPI_DEASSERT

}

void tmc4361A_read_int32_all(const uint8_t address, int32_t *values, const uint8_t total_num_in_chain, const unsigned int *hspi){
    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        tmc40bit_read_int32(address, hspi);
    }
    SPI_DEASSERT

    SPI_ASSERT
values = values + (total_num_in_chain-1);
    for (int i = 0; i < total_num_in_chain; i++) {
        *values = tmc40bit_read_int32(address, hspi);
        values--;
    }
    SPI_DEASSERT
}

void tmc4361A_read_uint32_all(const uint8_t address, uint32_t *values, const uint8_t total_num_in_chain, const unsigned int *hspi){
    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        tmc40bit_read_uint32(address, hspi);
    }
    SPI_DEASSERT

    values = values + (total_num_in_chain-1);

    SPI_ASSERT
    for (int i = 0; i < total_num_in_chain; i++) {
        *values = tmc40bit_read_uint32(address, hspi);
        values--;
    }
    SPI_DEASSERT
}






int32_t tmc4361A_read_int32_individual(const uint8_t address, const uint8_t pos_in_chain, const uint8_t total_num_in_chain, const unsigned int *hspi){
    int8_t  i = 1;
    SPI_ASSERT
    tmc40bit_read_int32(address, hspi);

    // Shift the written data to the correct driver in chain
    while(i < pos_in_chain) {
        tmc40bit_write_int32(0, 0, hspi);
        i++;
    }

    SPI_DEASSERT

    SPI_ASSERT

    // Shift data from target into the last one...
    while(i < total_num_in_chain) {
        tmc40bit_write_int32(0, 0, hspi);
        i++;
    }

        return tmc40bit_read_int32(address, hspi);

    SPI_DEASSERT
}

uint32_t tmc4361A_read_uint32_individual(const uint8_t address, const uint8_t pos_in_chain, const uint8_t total_num_in_chain, const unsigned int *hspi){
    int8_t  i = 1;
    SPI_ASSERT
    tmc40bit_read_int32(address, hspi);

    // Shift the written data to the correct driver in chain
    while(i < pos_in_chain) {
        tmc40bit_write_uint32(0, 0, hspi);
        i++;
    }

    SPI_DEASSERT

    SPI_ASSERT

    // Shift data from target into the last one...
    while(i < total_num_in_chain) {
        tmc40bit_write_uint32(0, 0, hspi);
        i++;
    }

    return tmc40bit_read_uint32(address, hspi);

    SPI_DEASSERT
}





//unsigned char tmc4361A_cover(unsigned char data, unsigned char lastTransfer, uint8_t total_num_in_chain, unsigned int hspi)
//{
//    static uint64 coverIn = 0;     // read from squirrel
//    static uint64 coverOut = 0;    // write to squirrel
//    static uint8 coverLength = 0;  // data to be written
//
//    uint8 out = 0; // return value of this function
//
//    // buffer outgoing data
//    coverOut <<= 8;    // shift left by one byte to make room for the next byte
//    coverOut |= data;  // add new byte to be written
//    coverLength++;     // count outgoing bytes
//
//    // return read and buffered byte to be returned
//    out = coverIn >> 56;  // output last received byte
//    coverIn <<= 8;        // shift by one byte to read this next time
//
//    if(lastTransfer)
//    {
//        /* Write data to cover register(s). The lower 4 bytes go into the cover low register,
//		 * the higher 4 bytes, if present, go into the cover high register.
//		 * The datagram needs to be sent twice, otherwise the read buffer will be delayed by
//		 * one read/write datagram.
//		 */
//
//        // Send the buffered datagram & wait a bit before continuing so the 4361 can complete the datagram to the driver
//        // measured delay between COVER_LOW transmission and COVER_DONE flag: ~90�s -> 1 ms more than enough
//        // todo CHECK 3: Delay measurement only done on TMC4361, not 4361A - make sure the required delay didnt change (LH) #1
//
//        uint64 coverOutArrayA[SM_NUM_DRIVES];
//        for (int i = 0; i < total_num_in_chain; i++) {
//            coverOutArrayA[i] = coverOut >> 32;
//        }
//
//        uint64 coverOutArrayB[SM_NUM_DRIVES];
//        for (int i = 0; i < total_num_in_chain; i++) {
//            coverOutArrayB[i] = coverOut & coverOut & 0xFFFFFFFF;
//        }
//
//
//        if(coverLength > 4) {
//            tmc4361A_write_int32_all(TMC4361A_COVER_HIGH_WR, (uint32_t)coverOutArrayA, total_num_in_chain, hspi);
//        }
//        tmc4361A_write_int32_all(TMC4361A_COVER_LOW_WR, coverOutArrayB, total_num_in_chain, hspi);
//        gpioDelay(1);
//
//        // Trigger a re-send by writing the low register again
//        tmc4361A_write_int32_all(TMC4361A_COVER_LOW_WR, coverOutArrayB, total_num_in_chain, hspi);
//
//        // Read the reply
//        coverIn = 0;
//        if(coverLength > 4)
//            coverIn |= (uint64) tmc4361A_readInt(TMC4361A_COVER_DRV_HIGH_RD) << 32;
//        coverIn |= tmc4361A_readInt(TMC4361A_COVER_DRV_LOW_RD);
//        coverIn <<= (8-coverLength) * 8; // Shift the highest byte of the reply to the highest byte of the buffer uint64
//
//        // Clear write buffer
//        coverOut = 0;
//        coverLength=0;
//    }
//
//    return out; // return buffered read byte
//}



static void spi_log_error(int rc){

        if (rc==PI_BAD_HANDLE){
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI transfer failed with a [bad handle]");
        } else if (rc ==PI_BAD_SPI_COUNT){
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI transfer failed with a [bad SPI count]");
        } else if (rc ==PI_SPI_XFER_FAILED){
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI transfer failed with a [transfer failed]");
        }else{
            LL_ERROR(GBSM_GEN_LOG_EN, "GBSM: SPI transfer failed with an [unknown error]");
        }

}

// General SPI decription
static uint8_t tmc40bit_write_int32(const uint8_t address, const int32_t value, const unsigned int *hspi)
{
    char tbuf[5];
    char rbuf[5];
    int rc = 0;
    tbuf[0] = (char) (address | 0x80);
    tbuf[1] = (char) ((value>>24) & 0xFF);
    tbuf[2] = (char) ((value>>16) & 0xFF);
    tbuf[3] = (char) ((value>>8) & 0xFF);
    tbuf[4] = (char) (value & 0xFF);

    rc = spiXfer(*hspi, tbuf, rbuf, 5);
    if (rc!=5){
        spi_log_error(rc);
    }

    return (uint8_t)rbuf[0];
}

static uint8_t tmc40bit_write_uint32(const uint8_t address, const uint32_t value, const unsigned int *hspi)
{
    char tbuf[5];
    char rbuf[5];
    int rc = 0;
    tbuf[0] = (char) (address | 0x80);
    tbuf[1] = (char) ((value>>24) & 0xFF);
    tbuf[2] = (char) ((value>>16) & 0xFF);
    tbuf[3] = (char) ((value>>8) & 0xFF);
    tbuf[4] = (char) (value & 0xFF);

    rc = spiXfer(*hspi, tbuf, rbuf, 5);

    if (rc!=5){
        spi_log_error(rc);
    }

    return (uint8_t)rbuf[0];
}



static int32_t tmc40bit_read_int32(const uint8_t address, const unsigned int *hspi)
{
    char tbuf[5], rbuf[5];
    int32_t value;
    int rc = 0;
    // clear write bit
    tbuf[0] = (char) (address & 0x7F);

    rc = spiXfer(*hspi, tbuf, rbuf, 5);

    if (rc!=5){
        spi_log_error(rc);
    }

    value = rbuf[1];
    value <<= 8;
    value |= rbuf[2];
    value <<= 8;
    value |= rbuf[3];
    value <<= 8;
    value |= rbuf[4];

    return value;
}


static uint32_t tmc40bit_read_uint32(const uint8_t address, const unsigned int *hspi)
{
    char tbuf[5], rbuf[5];
    uint32_t value;
    int rc = 0;
    // clear write bit
    tbuf[0] = (char) (address & 0x7F);

    rc = spiXfer(*hspi, tbuf, rbuf, 5);

    if (rc!=5){
        spi_log_error(rc);
    }

    value = (uint32_t) rbuf[1];
    value <<= 8;
    value |= (uint32_t) rbuf[2];
    value <<= 8;
    value |= (uint32_t) rbuf[3];
    value <<= 8;
    value |= (uint32_t) rbuf[4];

    return value;
}

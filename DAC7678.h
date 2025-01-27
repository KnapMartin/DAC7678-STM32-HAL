/*
 * DAC7678.h
 *
 *  Created on: Jan 24, 2025
 *      Author: knap-linux
 */

#ifndef DAC7678_H_
#define DAC7678_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define DAC7678_TIMEOUT 	100 // ms
#define DAC7678_MAX_VALUE 	4095

typedef enum
{
	DAC7678_OK,
	DAC7678_ERROR,
	DAC7678_ERROR_TX,
	DAC7678_ERROR_RX,
} DAC7678_State;

typedef struct
{
	I2C_HandleTypeDef *m_hi2c;
	uint8_t m_address;
} DAC7678;

typedef enum
{
	DAC7678_CH_NONE = -1,
	DAC7678_CH_A 	= 0b0000,
	DAC7678_CH_B 	= 0b0001,
	DAC7678_CH_C 	= 0b0010,
	DAC7678_CH_D 	= 0b0011,
	DAC7678_CH_E 	= 0b0100,
	DAC7678_CH_F 	= 0b0101,
	DAC7678_CH_G 	= 0b0110,
	DAC7678_CH_H 	= 0b0111,
	DAC7678_CH_ALL  = 0b1111
} DAC7678_Channel;

typedef enum
{
	DAC7678_CMD_NONE               = -1,
	DAC7678_CMD_WRITE_IN_REG       = 0b0000,
	DAC7678_CMD_READ_IN_REG        = 0b0000,
	DAC7678_CMD_UPDATE_DAC_REG     = 0b0001,
	DAC7678_CMD_READ_DAC_REG       = 0b0001,
	DAC7678_CMD_WRITE_UPDATE       = 0b0011,
	DAC7678_CMD_WRITE_UPDATE_ALL   = 0b0010,
	DAC7678_CMD_WRITE_OFF          = 0b0100,
	DAC7678_CMD_READ_OFF           = 0b0100,
	DAC7678_CMD_WRITE_CLR_CODE     = 0b0101,
	DAC7678_CMD_READ_CLR_CODE      = 0b0101,
	DAC7678_CMD_WRITE_LDAC         = 0b0110,
	DAC7678_CMD_READ_LDAC          = 0b0110,
	DAC7678_CMD_RESET              = 0b0111,
	DAC7678_CMD_WRITE_REF_STATIC   = 0b1000,
	DAC7678_CMD_READ_REF_STATIC    = 0b1000,
	DAC7678_CMD_WRITE_REF_FLEX     = 0b1001,
	DAC7678_CMD_READ_REF_FLEX      = 0b1001
} DAC7678_Command;

typedef enum
{
	DAC7678_WRITE_NONE,
	DAC7678_UPDATE_ON,
	DAC7678_UPDATE_OFF,
	DAC7678_UPDATE_ALL
} DAC7678_WriteOptions;

typedef enum
{
	DAC7678_REF_S_NONE,
	DAC7678_REF_ON,
	DAC7678_REF_OFF
} DAC7678_ReferenceStaticOptions;

typedef enum
{
	DAC7678_REF_F_NONE,
	DAC7678_REF_SYNCH_DAC,
	DAC7678_REF_ALWAYS_ON,
	DAC7678_REF_ALWAYS_OFF,
	DAC7678_REF_AS_STATIC,
} DAC7678_ReferenceFlexiOptions;

DAC7678_State DAC7678_init(DAC7678 *device, I2C_HandleTypeDef *hi2c);

DAC7678_State DAC7678_set_value(DAC7678 *device, const DAC7678_Channel channel, const DAC7678_WriteOptions options, const uint16_t value);
DAC7678_State DAC7678_update(DAC7678 *device, const DAC7678_Channel channel);
DAC7678_State DAC7678_set_internal_reference_static(DAC7678 *device, const DAC7678_ReferenceStaticOptions options);
DAC7678_State DAC7678_set_internal_reference_flexi(DAC7678 *device, const DAC7678_ReferenceFlexiOptions options);

#ifdef __cplusplus
}
#endif

#endif /* DAC7678_H_ */

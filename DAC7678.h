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

#define DAC7678_TIMEOUT 		100 // ms
#define DAC7678_MAX_VALUE 		4095
#define DAC7678_MAX_CHANNEL		8

#define DAC7678_TEST		// toggle tests

#ifdef DAC7678_TEST
typedef enum
{
	DAC7678_TST_PASS,
	DAC7678_TST_FAIL,
} DAC7678_Test;
#endif

typedef enum
{
	DAC7678_OK,
	DAC7678_ERROR,
	DAC7678_ERROR_TX,
	DAC7678_ERROR_RX,
	DAC7678_ERROR_IN_VALUE,
	DAC7678_ERROR_CH_VALUE,
} DAC7678_State;

typedef enum
{
	DAC7678_CH_NONE = -1,
	DAC7678_CH_A 	= 0x00,
	DAC7678_CH_B 	= 0x01,
	DAC7678_CH_C 	= 0x02,
	DAC7678_CH_D 	= 0x03,
	DAC7678_CH_E 	= 0x04,
	DAC7678_CH_F 	= 0x05,
	DAC7678_CH_G 	= 0x06,
	DAC7678_CH_H 	= 0x07,
	DAC7678_CH_ALL  = 0xFF
} DAC7678_Channel;

typedef enum
{
	DAC7678_CMD_NONE               = -1,
	DAC7678_CMD_WRITE_IN_REG       = 0x00,
	DAC7678_CMD_READ_IN_REG        = 0x00,
	DAC7678_CMD_UPDATE_DAC_REG     = 0x10,
	DAC7678_CMD_READ_DAC_REG       = 0x10,
	DAC7678_CMD_WRITE_UPDATE       = 0x30,
	DAC7678_CMD_WRITE_UPDATE_ALL   = 0x20,
	DAC7678_CMD_WRITE_PWR          = 0x40,
	DAC7678_CMD_READ_PWR           = 0x40,
	DAC7678_CMD_WRITE_CLR_CODE     = 0x50,
	DAC7678_CMD_READ_CLR_CODE      = 0x50,
	DAC7678_CMD_WRITE_LDAC         = 0x60,
	DAC7678_CMD_READ_LDAC          = 0x60,
	DAC7678_CMD_RESET              = 0x70,
	DAC7678_CMD_WRITE_REF_STATIC   = 0x80,
	DAC7678_CMD_READ_REF_STATIC    = 0x80,
	DAC7678_CMD_WRITE_REF_FLEX     = 0x90,
	DAC7678_CMD_READ_REF_FLEX      = 0x90
} DAC7678_Command;

typedef enum
{
	DAC7678_WRT_NONE		= -1,
	DAC7678_WRT_UPDATE_OFF 	= 0x00,
	DAC7678_WRT_UPDATE_ALL 	= 0x20,
	DAC7678_WRT_UPDATE_ON	= 0x30
} DAC7678_WriteOptions;

typedef enum
{
	DAC7678_REF_S_NONE	= -1,
	DAC7678_REF_S_OFF 	= 0x00,
	DAC7678_REF_S_ON 	= 0x10
} DAC7678_ReferenceStaticOptions;

typedef enum
{
	DAC7678_REF_F_NONE			= -1,
	DAC7678_REF_F_SYNCH_DAC 	= 0b01000000,
	DAC7678_REF_F_ALWAYS_ON 	= 0b01010000,
	DAC7678_REF_F_ALWAYS_OFF 	= 0b01100000,
	DAC7678_REF_F_AS_STATIC		= 0b00000000,
} DAC7678_ReferenceFlexiOptions;

typedef enum
{
	DAC7678_PWR_NONE 			= -1,
	DAC7678_PWR_ON 				= 0,
	DAC7678_PWR_PLDOWN_1K 		= 0b00100000,
	DAC7678_PWR_PLDOWN_100K		= 0b01000000,
	DAC7678_PWR_HIGH_Z			= 0b01100000,
} DAC7678_PowerOptions;

typedef enum
{
	DAC7678_PWR_CH_NONE = 0x00,
	DAC7678_PWR_CH_A 	= 0x01,
	DAC7678_PWR_CH_B 	= 0x02,
	DAC7678_PWR_CH_C 	= 0x04,
	DAC7678_PWR_CH_D 	= 0x08,
	DAC7678_PWR_CH_E 	= 0x10,
	DAC7678_PWR_CH_F 	= 0x20,
	DAC7678_PWR_CH_G 	= 0x40,
	DAC7678_PWR_CH_H 	= 0x80,
	DAC7678_PWR_CH_ALL 	= 0xFF,
} DAC7678_PowerChannels;

typedef enum
{
	DAC7678_CLR_NONE 	= -1,
	DAC7678_CLR_ZERO 	= 0b00000000,
	DAC7678_CLR_MID 	= 0b00010000,
	DAC7678_CLR_FULL	= 0b00100000,
	DAC7678_CLR_DISABLE	= 0b00110000,
} DAC7678_ClearOptions;

typedef enum
{
	DAC7678_LDAC_CH_NONE	= 0x00,
	DAC7678_LDAC_CH_A 		= 0x01,
	DAC7678_LDAC_CH_B 		= 0x02,
	DAC7678_LDAC_CH_C 		= 0x04,
	DAC7678_LDAC_CH_D 		= 0x08,
	DAC7678_LDAC_CH_E 		= 0x10,
	DAC7678_LDAC_CH_F 		= 0x20,
	DAC7678_LDAC_CH_G 		= 0x40,
	DAC7678_LDAC_CH_H 		= 0x80,
	DAC7678_LDAC_CH_ALL		= 0xFF,
} DAC7678_LdacChannel;

typedef enum
{
	DAC7678_RST					= 0x00,
	DAC7678_RST_SET_HS_MODE		= 0x40,
	DAC7678_RST_KEEP_HS_MODE	= 0x80,
} DAC7678_ResetOptions;

typedef struct
{
	I2C_HandleTypeDef		*m_hi2c;
	uint8_t					m_address;
	DAC7678_WriteOptions	m_write_options;
} DAC7678;

DAC7678_State DAC7678_init(DAC7678 *device, I2C_HandleTypeDef *hi2c, const uint8_t address);
DAC7678_State DAC7678_deinit(DAC7678 *device);
DAC7678_State DAC7678_set_write_options(DAC7678 *device, const DAC7678_WriteOptions options);
DAC7678_State DAC7678_set_value(DAC7678 *device, const DAC7678_Channel channel, const uint16_t value);
DAC7678_State DAC7678_update_dac_reg(DAC7678 *device, const DAC7678_Channel channel);
DAC7678_State DAC7678_set_power_reg(DAC7678 *device, const DAC7678_PowerOptions options, const DAC7678_PowerChannels channel_mask);
DAC7678_State DAC7678_set_clear_reg(DAC7678 *device, const DAC7678_ClearOptions options);
DAC7678_State DAC7678_set_ldac_reg(DAC7678 *device, const DAC7678_LdacChannel channel_mask);
DAC7678_State DAC7678_set_int_ref_static_reg(DAC7678 *device, const DAC7678_ReferenceStaticOptions options);
DAC7678_State DAC7678_set_int_ref_flexi_reg(DAC7678 *device, const DAC7678_ReferenceFlexiOptions options);
DAC7678_State DAC7678_reset(DAC7678 *device, const DAC7678_ResetOptions options);

DAC7678_State DAC7678_get_value(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value);
DAC7678_State DAC7678_get_dac_reg(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value);
DAC7678_State DAC7678_get_power_reg(DAC7678 *device, DAC7678_PowerOptions *options, DAC7678_PowerChannels *channel_mask);
DAC7678_State DAC7678_get_clear_reg(DAC7678 *device, DAC7678_ClearOptions *options);
DAC7678_State DAC7678_get_ldac_reg(DAC7678 *device, DAC7678_LdacChannel *channel_mask);
DAC7678_State DAC7678_get_int_ref_static_reg(DAC7678 *device, DAC7678_ReferenceStaticOptions *options);
DAC7678_State DAC7678_get_int_ref_flexi_reg(DAC7678 *device, DAC7678_ReferenceFlexiOptions *options);

// TODO:
DAC7678_State DAC7678_set_values(DAC7678 *device); // dodaj array[8] v device struct

// NOTE: run from main loop or timer isr
void test_saw(DAC7678 *dac, uint16_t amplitude, uint16_t diff);
void test_sine(DAC7678 *dac, uint16_t amplitude, uint16_t num_samples);

#ifdef DAC7678_TEST
DAC7678_Test test_wr_input_register(DAC7678 *device);
DAC7678_Test test_wr_dac_register(DAC7678 *device);
DAC7678_Test test_wr_reference_register_static(DAC7678 *device);
DAC7678_Test test_wr_reference_register_flexi(DAC7678 *device);
DAC7678_Test test_wr_power_register(DAC7678 *device);
DAC7678_Test test_wr_clear_register(DAC7678 *device);
DAC7678_Test test_wr_ldac_register(DAC7678 *device);
void test_run_all(DAC7678 *device);
#endif

#ifdef __cplusplus
}
#endif

#endif /* DAC7678_H_ */

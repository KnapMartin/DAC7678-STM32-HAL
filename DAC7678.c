/*
 * DAC7678.c
 *
 *  Created on: Jan 24, 2025
 *      Author: knap-linux
 */

#include "DAC7678.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

static uint8_t s_init = 0;

DAC7678_State DAC7678_init(DAC7678 *device, I2C_HandleTypeDef *hi2c, const uint8_t address)
{
	device->m_hi2c = hi2c;
	device->m_address = address;
	device->m_write_options = DAC7678_WRT_NONE;
	s_init = 1;

	return DAC7678_OK;
}

DAC7678_State DAC7678_deinit(DAC7678 *device)
{
	device->m_hi2c = NULL;
	device->m_address = 0x00;
	device->m_write_options = DAC7678_WRT_NONE;
	s_init = 0;

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_write_options(DAC7678 *device, const DAC7678_WriteOptions options)
{
	if (!s_init) return DAC7678_ERROR;

	device->m_write_options = options;

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_value(DAC7678 *device, const DAC7678_Channel channel, const uint16_t value)
{
	if (!s_init) return DAC7678_ERROR;
	if (value > DAC7678_MAX_VALUE) return DAC7678_ERROR_IN_VALUE;
	if (channel > DAC7678_MAX_CHANNEL) return DAC7678_ERROR_CH_VALUE;

	uint8_t data[3];
	data[0] = (uint8_t)(device->m_write_options | channel);
	data[1] = (uint8_t)(value >> 4);
	data[2] = (uint8_t)(value << 4);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_update_dac_reg(DAC7678 *device, const DAC7678_Channel channel)
{
	if (!s_init) return DAC7678_ERROR;
	if (channel > DAC7678_MAX_CHANNEL) return DAC7678_ERROR_CH_VALUE;

	uint8_t data[3];
	data[0] = DAC7678_CMD_UPDATE_DAC_REG;
	data[1] = 0x00;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_power_reg(DAC7678 *device, const DAC7678_PowerOptions options, const DAC7678_PowerChannels channel_mask)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_PWR;
	uint16_t channels = channel_mask << 5;
	data[1] = (uint8_t)((channels >> 8) | options);
	data[2] = (uint8_t)(channels & 0xFF);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_clear_reg(DAC7678 *device, const DAC7678_ClearOptions options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_CLR_CODE;
	data[1] = 0x00;
	data[2] = (uint8_t)options;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_ldac_reg(DAC7678 *device, const DAC7678_LdacChannel channel_mask)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_LDAC;
	data[1] = (uint8_t)channel_mask;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_int_ref_static_reg(DAC7678 *device, const DAC7678_ReferenceStaticOptions options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_REF_STATIC;
	data[1] = 0x00;
	data[2] = (uint8_t)options;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_int_ref_flexi_reg(DAC7678 *device, const DAC7678_ReferenceFlexiOptions options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_REF_FLEX;
	data[1] = (uint8_t)options;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_reset(DAC7678 *device, const DAC7678_ResetOptions options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_RESET;
	data[1] = (uint8_t)options;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_value(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value)
{
	if (!s_init) return DAC7678_ERROR;
	if (channel > DAC7678_MAX_CHANNEL) return DAC7678_ERROR_CH_VALUE;

	uint8_t data[1];
	data[0] = (uint8_t)(DAC7678_CMD_READ_IN_REG | channel);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	uint16_t result = 0;
	result |= (uint16_t)(read_data[0] << 4);
	result |= (uint16_t)(read_data[1] >> 4);
	*value = result;

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_dac_reg(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value)
{
	if (!s_init) return DAC7678_ERROR;
	if (channel > DAC7678_MAX_CHANNEL) return DAC7678_ERROR_CH_VALUE;

	uint8_t data[1];
	data[0] = (uint8_t)(DAC7678_CMD_READ_DAC_REG | channel);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	uint16_t result = 0;
	result |= (uint16_t)(read_data[0] << 4);
	result |= (uint16_t)(read_data[1] >> 4);
	*value = result;

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_power_reg(DAC7678 *device, DAC7678_PowerOptions *options, DAC7678_PowerChannels *channel_mask)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_PWR;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_PowerOptions)(read_data[0] << 5);
	*channel_mask = (DAC7678_PowerChannels)(read_data[1]);

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_clear_reg(DAC7678 *device, DAC7678_ClearOptions *options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_CLR_CODE;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ClearOptions)(read_data[1] << 4);

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_ldac_reg(DAC7678 *device, DAC7678_LdacChannel *channel_mask)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_LDAC;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*channel_mask = (DAC7678_LdacChannel)(read_data[1]);

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_int_ref_static_reg(DAC7678 *device, DAC7678_ReferenceStaticOptions *options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_REF_STATIC;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ReferenceStaticOptions)(read_data[1] << 4);

	return DAC7678_OK;
}

DAC7678_State DAC7678_get_int_ref_flexi_reg(DAC7678 *device, DAC7678_ReferenceFlexiOptions *options)
{
	if (!s_init) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_REF_FLEX;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t read_data[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, read_data, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ReferenceFlexiOptions)(read_data[0] >> 1);

	return DAC7678_OK;
}


// ---------------------------------------------------------------------------------------------------------------

void test_saw(DAC7678 *dac, uint16_t amplitude, uint16_t diff)
{
	if (amplitude > DAC7678_MAX_VALUE) return;

	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_ALL);
	static uint16_t value = 0;
	DAC7678_set_value(dac, DAC7678_CH_ALL, value);
	value += diff;
	if (value > amplitude) value = 0;
}

void test_sine(DAC7678 *dac, uint16_t amplitude, uint16_t num_samples)
{
	if (amplitude > DAC7678_MAX_VALUE) return;

	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_ALL);
	static uint16_t index = 0;
	const float scale = (float)amplitude / 2.0f;
	const float offset = (float)amplitude / 2.0f;

	float angle = 2.0f * 3.14159265358979323846f * (float)index / (float)num_samples;
	uint16_t value = (uint16_t)(sinf(angle) * scale + offset);

	DAC7678_set_value(dac, DAC7678_CH_ALL, value);

	index++;
	if (index >= num_samples) index = 0;
}

#ifdef DAC7678_TEST
DAC7678_Test test_wr_input_register(DAC7678 *device)
{
	uint16_t dataW = 666;
	uint16_t dataR = 0;

	// channel A
	if (DAC7678_set_value(device, DAC7678_CH_A, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_A, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel B
	if (DAC7678_set_value(device, DAC7678_CH_B, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_B, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel C
	if (DAC7678_set_value(device, DAC7678_CH_C, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_C, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel D
	if (DAC7678_set_value(device, DAC7678_CH_D, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_D, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel E
	if (DAC7678_set_value(device, DAC7678_CH_E, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_E, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel F
	if (DAC7678_set_value(device, DAC7678_CH_F, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_F, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel G
	if (DAC7678_set_value(device, DAC7678_CH_G, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_G, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel H
	if (DAC7678_set_value(device, DAC7678_CH_H, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_H, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_dac_register(DAC7678 *device)
{
	DAC7678_set_write_options(device, DAC7678_WRT_UPDATE_OFF);

	uint16_t dataW = 666;
	uint16_t dataR = 0;

	// channel A
	if (DAC7678_set_value(device, DAC7678_CH_A, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_A) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_A, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel B
	if (DAC7678_set_value(device, DAC7678_CH_B, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_B) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_B, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel C
	if (DAC7678_set_value(device, DAC7678_CH_C, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_C) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_C, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel D
	if (DAC7678_set_value(device, DAC7678_CH_D, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_D) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_D, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel E
	if (DAC7678_set_value(device, DAC7678_CH_E, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_E) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_E, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel F
	if (DAC7678_set_value(device, DAC7678_CH_F, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_F) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_F, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel G
	if (DAC7678_set_value(device, DAC7678_CH_G, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_G) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_G, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;
	dataR = 0;

	// channel H
	if (DAC7678_set_value(device, DAC7678_CH_H, dataW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_update_dac_reg(device, DAC7678_CH_H) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_value(device, DAC7678_CH_H, &dataR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (dataW != dataR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_reference_register_static(DAC7678 *device)
{
	DAC7678_ReferenceStaticOptions optionW;
	DAC7678_ReferenceStaticOptions optionR;

	optionW = DAC7678_REF_S_ON;
	optionR = DAC7678_REF_S_NONE;
	if (DAC7678_set_int_ref_static_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_static_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_S_OFF;
	optionR = DAC7678_REF_S_NONE;
	if (DAC7678_set_int_ref_static_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_static_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_reference_register_flexi(DAC7678 *device)
{
	DAC7678_ReferenceFlexiOptions optionW;
	DAC7678_ReferenceFlexiOptions optionR;

	optionW = DAC7678_REF_F_SYNCH_DAC;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_int_ref_flexi_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_flexi_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR)
	{
//		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		return DAC7678_TST_FAIL;
	}

	optionW = DAC7678_REF_F_ALWAYS_ON;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_int_ref_flexi_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_flexi_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_F_ALWAYS_OFF;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_int_ref_flexi_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_flexi_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_F_AS_STATIC;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_int_ref_flexi_reg(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_int_ref_flexi_reg(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_power_register(DAC7678 *device)
{
	DAC7678_PowerOptions optionsW;
	DAC7678_PowerOptions optionsR;
	DAC7678_PowerChannels channelsW;
	DAC7678_PowerChannels channelsR;

	optionsW = DAC7678_PWR_ON;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_ALL;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power_reg(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_power_reg(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_PWR_PLDOWN_1K;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_D;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power_reg(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_power_reg(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_PWR_PLDOWN_100K;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_A | DAC7678_PWR_CH_C | DAC7678_PWR_CH_H;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power_reg(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_power_reg(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;

	optionsW = DAC7678_PWR_HIGH_Z;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_F | DAC7678_PWR_CH_D | DAC7678_PWR_CH_H;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power_reg(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_power_reg(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_clear_register(DAC7678 *device)
{
	DAC7678_ClearOptions optionsW;
	DAC7678_ClearOptions optionsR;

	optionsW = DAC7678_CLR_ZERO;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_reg(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_clear_reg(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_MID;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_reg(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_clear_reg(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_FULL;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_reg(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_clear_reg(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_DISABLE;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_reg(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_clear_reg(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

DAC7678_Test test_wr_ldac_register(DAC7678 *device)
{
	DAC7678_LdacChannel channelW;
	DAC7678_LdacChannel channelR;

	channelW = DAC7678_LDAC_CH_A;
	channelR = DAC7678_LDAC_CH_NONE;
	if (DAC7678_set_ldac_reg(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_ldac_reg(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	channelW = DAC7678_LDAC_CH_ALL;
	channelR = DAC7678_LDAC_CH_NONE;
	if (DAC7678_set_ldac_reg(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_ldac_reg(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	channelW = DAC7678_LDAC_CH_A | DAC7678_LDAC_CH_H | DAC7678_LDAC_CH_G | DAC7678_LDAC_CH_C;
	channelR = DAC7678_LDAC_CH_NONE;
	if (DAC7678_set_ldac_reg(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_get_ldac_reg(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

void test_run_all(DAC7678 *dac)
{
	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_OFF);
	if (test_wr_input_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 0 failed\r\n");
	}

	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_ON);
	if (test_wr_input_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 1 failed\r\n");
	}

	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_ALL);
	if (test_wr_input_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 2 failed\r\n");
	}

	if (test_wr_dac_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 3 failed\r\n");
	}

	if (test_wr_reference_register_static(dac) != DAC7678_TST_PASS)
	{
		printf("test 4 failed\r\n");
	}

	if (test_wr_reference_register_flexi(dac) != DAC7678_TST_PASS)
	{
		printf("test 5 failed\r\n");
	}

	if (test_wr_power_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 6 failed\r\n");
	}

	if (test_wr_clear_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 7 failed\r\n");
	}

	if (test_wr_ldac_register(dac) != DAC7678_TST_PASS)
	{
		printf("test 8 failed\r\n");
	}
}

#endif



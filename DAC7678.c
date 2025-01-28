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

static uint8_t sInit = 0;

DAC7678_State DAC7678_init(DAC7678 *device, I2C_HandleTypeDef *hi2c, const uint8_t address)
{
	device->m_hi2c = hi2c;
	device->m_address = address;
	device->m_writeOptions = DAC7678_WRT_NONE;
	sInit = 1;

	return DAC7678_OK;
}

DAC7678_State DAC7678_deinit(DAC7678 *device)
{
	device->m_hi2c = NULL;
	device->m_address = 0x00;
	device->m_writeOptions = DAC7678_WRT_NONE;
	sInit = 0;

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_write_options(DAC7678 *device, const DAC7678_WriteOptions options)
{
	if (!sInit) return DAC7678_ERROR;

	device->m_writeOptions = options;

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_value(DAC7678 *device, const DAC7678_Channel channel, const uint16_t value)
{
	if (!sInit) return DAC7678_ERROR;
	if (value > DAC7678_MAX_VALUE) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = (device->m_writeOptions << 4) | channel;
	data[1] = (uint8_t)(value >> 4);
	data[2] = (uint8_t)(value << 4);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_update(DAC7678 *device, const DAC7678_Channel channel)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_UPDATE_DAC_REG << 4;
	data[1] = 0x00;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_power(DAC7678 *device, const DAC7678_PowerOptions options, const DAC7678_PowerChannels channelMask)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_PWR << 4;
	uint16_t channels = channelMask << 5;
	data[1] = (uint8_t)(channels >> 8);
	data[1] = data[1] | options;
	data[2] = (uint8_t)(channels & 0xFF);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_clear_code(DAC7678 *device, const DAC7678_ClearOptions options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_CLR_CODE << 4;
	data[1] = 0x00;
	data[2] = (uint8_t)options;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_ldac(DAC7678 *device, const DAC7678_LdacChannel channelMask)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_LDAC << 4;
	data[1] = (uint8_t)channelMask;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_internal_reference_static(DAC7678 *device, const DAC7678_ReferenceStaticOptions options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_REF_STATIC << 4;
	data[1] = 0x00;
	data[2] = (uint8_t)options;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_internal_reference_flexi(DAC7678 *device, const DAC7678_ReferenceFlexiOptions options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_REF_FLEX << 4;
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
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_RESET << 4;
	data[1] = (uint8_t)options;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_value(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_IN_REG << 4 | channel;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	uint16_t result = 0;
	result |= (uint16_t)(readData[0] << 4);
	result |= (uint16_t)(readData[1] >> 4);
	*value = result;

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_dac_value(DAC7678 *device, const DAC7678_Channel channel, uint16_t *value)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_DAC_REG << 4 | channel;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	uint16_t result = 0;
	result |= (uint16_t)(readData[0] << 4);
	result |= (uint16_t)(readData[1] >> 4);
	*value = result;

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_power_value(DAC7678 *device, DAC7678_PowerOptions *options, DAC7678_PowerChannels *channelMask)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_PWR << 4;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_PowerOptions)(readData[0] << 5);
	*channelMask = (DAC7678_PowerChannels)(readData[1]);

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_clear_value(DAC7678 *device, DAC7678_ClearOptions *options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_CLR_CODE << 4;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ClearOptions)(readData[1] << 4);

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_ldac_value(DAC7678 *device, DAC7678_LdacChannel *channelMask)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_LDAC << 4;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*channelMask = (DAC7678_LdacChannel)(readData[1]);

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_internal_reference_static(DAC7678 *device, DAC7678_ReferenceStaticOptions *options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_REF_STATIC << 4;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ReferenceStaticOptions)(readData[1] << 4);

	return DAC7678_OK;
}

DAC7678_State DAC7678_read_internal_reference_flexi(DAC7678 *device, DAC7678_ReferenceFlexiOptions *options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[1];
	data[0] = DAC7678_CMD_READ_REF_FLEX << 4;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 1, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	uint8_t readData[2];
	if (HAL_I2C_Master_Receive(device->m_hi2c, device->m_address << 1, readData, 2, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_RX;
	}

	*options = (DAC7678_ReferenceFlexiOptions)(readData[0] >> 1);

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

void test_sine(DAC7678 *dac, uint16_t amplitude, uint16_t numSamples)
{
	if (amplitude > DAC7678_MAX_VALUE) return;

	DAC7678_set_write_options(dac, DAC7678_WRT_UPDATE_ALL);
	static uint16_t index = 0;
	const float scale = (float)amplitude / 2.0f;
	const float offset = (float)amplitude / 2.0f;

	float angle = 2.0f * 3.14159265358979323846f * (float)index / (float)numSamples;
	uint16_t value = (uint16_t)(sinf(angle) * scale + offset);

	DAC7678_set_value(dac, DAC7678_CH_ALL, value);

	index++;
	if (index >= numSamples) index = 0;
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
	if (DAC7678_read_value(device, DAC7678_CH_A, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_B, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_C, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_D, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_E, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_F, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_G, &dataR) != DAC7678_OK)
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
	if (DAC7678_read_value(device, DAC7678_CH_H, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_A) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_A, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_B) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_B, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_C) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_C, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_D) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_D, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_E) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_E, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_F) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_F, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_G) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_G, &dataR) != DAC7678_OK)
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
	if (DAC7678_update(device, DAC7678_CH_H) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_value(device, DAC7678_CH_H, &dataR) != DAC7678_OK)
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
	if (DAC7678_set_internal_reference_static(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_static(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_S_OFF;
	optionR = DAC7678_REF_S_NONE;
	if (DAC7678_set_internal_reference_static(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_static(device, &optionR) != DAC7678_OK)
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
	if (DAC7678_set_internal_reference_flexi(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_flexi(device, &optionR) != DAC7678_OK)
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
	if (DAC7678_set_internal_reference_flexi(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_flexi(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_F_ALWAYS_OFF;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_internal_reference_flexi(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_flexi(device, &optionR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionW != optionR) return DAC7678_TST_FAIL;

	optionW = DAC7678_REF_F_AS_STATIC;
	optionR = DAC7678_REF_F_NONE;
	if (DAC7678_set_internal_reference_flexi(device, optionW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_internal_reference_flexi(device, &optionR) != DAC7678_OK)
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
	if (DAC7678_set_power(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_power_value(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_PWR_PLDOWN_1K;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_D;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_power_value(device, &optionsR, &channelsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;
	if (channelsW != channelsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_PWR_PLDOWN_100K;
	optionsR = DAC7678_PWR_NONE;
	channelsW = DAC7678_PWR_CH_A | DAC7678_PWR_CH_C | DAC7678_PWR_CH_H;
	channelsR = DAC7678_PWR_CH_NONE;
	if (DAC7678_set_power(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_power_value(device, &optionsR, &channelsR) != DAC7678_OK)
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
	if (DAC7678_set_power(device, optionsW, channelsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_power_value(device, &optionsR, &channelsR) != DAC7678_OK)
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
	if (DAC7678_set_clear_code(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_clear_value(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_MID;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_code(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_clear_value(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_FULL;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_code(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_clear_value(device, &optionsR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (optionsW != optionsR) return DAC7678_TST_FAIL;

	optionsW = DAC7678_CLR_DISABLE;
	optionsR = DAC7678_CLR_NONE;
	if (DAC7678_set_clear_code(device, optionsW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_clear_value(device, &optionsR) != DAC7678_OK)
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
	if (DAC7678_set_ldac(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_ldac_value(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	channelW = DAC7678_LDAC_CH_ALL;
	channelR = DAC7678_LDAC_CH_NONE;
	if (DAC7678_set_ldac(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_ldac_value(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	channelW = DAC7678_LDAC_CH_A | DAC7678_LDAC_CH_H | DAC7678_LDAC_CH_G | DAC7678_LDAC_CH_C;
	channelR = DAC7678_LDAC_CH_NONE;
	if (DAC7678_set_ldac(device, channelW) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (DAC7678_read_ldac_value(device, &channelR) != DAC7678_OK)
	{
		return DAC7678_TST_FAIL;
	}
	if (channelW != channelR) return DAC7678_TST_FAIL;

	return DAC7678_TST_PASS;
}

#endif



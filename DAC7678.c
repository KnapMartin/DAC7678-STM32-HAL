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

	switch (device->m_writeOptions)
	{
		case DAC7678_WRT_UPDATE_ON:
		{
			data[0] = (DAC7678_CMD_WRITE_UPDATE << 4) | channel;
			break;
		}
		case DAC7678_WRT_UPDATE_OFF:
		{
			data[0] = (DAC7678_CMD_WRITE_IN_REG << 4) | channel;
			break;
		}
		case DAC7678_WRT_UPDATE_ALL:
		{
			data[0] = (DAC7678_CMD_WRITE_UPDATE_ALL << 4) | channel;
			break;
		}
		default:
		{
			return DAC7678_ERROR;
		}
	}

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

DAC7678_State DAC7678_set_internal_reference_static(DAC7678 *device, const DAC7678_ReferenceStaticOptions options)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_REF_STATIC << 4;
	data[1] = 0x00;

	switch (options)
	{
		case DAC7678_REF_S_ON:
		{
			data[2] = 0x10;
			break;
		}
		case DAC7678_REF_S_OFF:
		{
			data[2] = 0x00;
			break;
		}
		default:
		{
			return DAC7678_ERROR;
		}
	}

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
	data[2] = 0x00;

	switch (options)
	{
		case DAC7678_REF_F_SYNCH_DAC:
		{
			data[1] = 0b01000000;
			break;
		}
		case DAC7678_REF_F_ALWAYS_ON:
		{
			data[1] = 0b01010000;
			break;
		}
		case DAC7678_REF_F_ALWAYS_OFF:
		{
			data[1] = 0b01100000;
			break;
		}
		case DAC7678_REF_F_AS_STATIC:
		{
			data[1] = 0b00000000;
			break;
		}
		default:
		{
			return DAC7678_ERROR;
		}
	}

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

DAC7678_State DAC7678_clear_code(DAC7678 *device, const DAC7678_ClearOptions options)
{
	if (!sInit) return DAC7678_ERROR;

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

DAC7678_State DAC7678_set_ldac(DAC7678 *device, const DAC7678_LdacChannel channelMask)
{
	if (!sInit) return DAC7678_ERROR;

	uint8_t data[3];
	data[0] = DAC7678_CMD_WRITE_LDAC;
	data[1] = (uint8_t)channelMask;
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
	data[0] = DAC7678_CMD_RESET;
	data[1] = (uint8_t)options;
	data[2] = 0x00;

	if (HAL_I2C_Master_Transmit(device->m_hi2c, device->m_address << 1, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

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


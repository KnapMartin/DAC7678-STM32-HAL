/*
 * DAC7678.c
 *
 *  Created on: Jan 24, 2025
 *      Author: knap-linux
 */

#include "DAC7678.h"

#include <stdio.h>
#include <string.h>

static const uint8_t sDeviceAddress = 0x48 << 1;
static uint8_t sInit = 0;

DAC7678_State DAC7678_init(DAC7678 *device, I2C_HandleTypeDef *hi2c)
{
	device->m_hi2c = hi2c;
	sInit = 1;

	return DAC7678_OK;
}

DAC7678_State DAC7678_set_value(DAC7678 *device, const DAC7678_Channel channel, const DAC7678_WriteOptions options, const uint16_t value)
{
	if (!sInit) return DAC7678_ERROR;
	if (value > DAC7678_MAX_VALUE) return DAC7678_ERROR;

	uint8_t data[3];
	if (options == DAC7678_UPDATE_ON)
	{
		data[0] = (DAC7678_CMD_WRITE_UPDATE << 4) | channel;
	}
	else if (options == DAC7678_UPDATE_OFF)
	{
		data[0] = (DAC7678_CMD_WRITE_IN_REG << 4) | channel;
	}
	else if (options == DAC7678_UPDATE_ALL)
	{
		data[0] = (DAC7678_CMD_WRITE_UPDATE_ALL << 4) | channel;
	}
	else
	{
		return DAC7678_ERROR;
	}

	data[1] = (uint8_t)(value >> 4);
	data[2] = (uint8_t)(value << 4);

	if (HAL_I2C_Master_Transmit(device->m_hi2c, sDeviceAddress, data, 3, DAC7678_TIMEOUT) != HAL_OK)
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

	if (HAL_I2C_Master_Transmit(device->m_hi2c, sDeviceAddress, data, 3, DAC7678_TIMEOUT) != HAL_OK)
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
	if (options == DAC7678_REF_ON)
	{
		data[2] = 0x10;
	}
	else if (options == DAC7678_REF_OFF)
	{
		data[2] = 0x00;
	}
	else
	{
		return DAC7678_ERROR;
	}

	if (HAL_I2C_Master_Transmit(device->m_hi2c, sDeviceAddress, data, 3, DAC7678_TIMEOUT) != HAL_OK)
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
		case DAC7678_REF_SYNCH_DAC:
		{
			data[1] = 0b01000000;
			break;
		}
		case DAC7678_REF_ALWAYS_ON:
		{
			data[1] = 0b01010000;
			break;
		}
		case DAC7678_REF_ALWAYS_OFF:
		{
			data[1] = 0b01100000;
			break;
		}
		case DAC7678_REF_AS_STATIC:
		{
			data[1] = 0b00000000;
			break;
		}
		default:
		{
			return DAC7678_ERROR;
		}
	}

	if (HAL_I2C_Master_Transmit(device->m_hi2c, sDeviceAddress, data, 3, DAC7678_TIMEOUT) != HAL_OK)
	{
		return DAC7678_ERROR_TX;
	}

	return DAC7678_OK;
}


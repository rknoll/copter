/*
 * Serial_Cmd.c
 *
 *  Created on: 30.01.2015
 *      Author: SteurerE
 */
#include "BluetoothProtocol.h"

//Helper to get Float values out of integer-bytes//
union
{
	int i;
	float f;
} u;

status_t maintainBluetoothInputBuffer(
		uint8_t *input_buffer,uint32_t *count,
		ControlValue *control_value, DataPacket* packet)
{

	static int packet_counter = 0;
	static int packets_to_be_received = 0;

	if (*count < PACKET_SIZE) {
		return RECEIVED_N_C; //must do another read operation
	}

	control_value->header = 0;
	control_value->speed = 0;
	control_value->z_rotate = 0.0;
	control_value->x_pitch = 0.0;
	control_value->y_roll = 0.0;
	control_value->checksum = 0;

	uint32_t checksum = input_buffer[0];
	checksum ^= ((input_buffer[1] << 8 | input_buffer[2]) & 0xFFFF);

	for (int i = 3; i < (PACKET_SIZE - PACKET_CHECKSUM) - 1; i += 4)
	{

		checksum ^= (input_buffer[i] << 24 | input_buffer[i + 1] << 16
				| input_buffer[i + 2] << 8 | input_buffer[i + 3]);

	}

	control_value->checksum = input_buffer[15] << 24 | input_buffer[16] << 16
			| input_buffer[17] << 8 | input_buffer[18];

	if (control_value->checksum == checksum) {
		if (input_buffer[0] == 0xC3) {
			control_value->header = input_buffer[0];
			control_value->speed = (input_buffer[1] << 8 | input_buffer[2]);
			u.i = ((input_buffer[3] << 24 | input_buffer[4] << 16
					| input_buffer[5] << 8 | input_buffer[6]));
			control_value->z_rotate = u.f;
			u.i = (input_buffer[7] << 24 | input_buffer[8] << 16
					| input_buffer[9] << 8 | input_buffer[10]);
			control_value->x_pitch = u.f;
			u.i = (input_buffer[11] << 24 | input_buffer[12] << 16
					| input_buffer[13] << 8 | input_buffer[14]);
			control_value->y_roll = u.f;

			//remove packet from buffer//
			for (uint32_t i = PACKET_SIZE; i < *count; ++i) {
				input_buffer[i - PACKET_SIZE] = input_buffer[i];
			}
			*count = *count - PACKET_SIZE;
			return RECEIVED_CONTROL_PACKET; //Got a valid Control-Package
		}
	}

	packet_counter = 0; //if it happens between data-packages...
	//Cleanup Buffer//
	for (uint32_t i = 1; i < *count; ++i) {
		if (input_buffer[i] == 0xC3) {
			for (uint32_t j = i; j < *count; ++j) {
				input_buffer[j - i] = input_buffer[j];
			}
			*count = *count - i;
			return CHECKSUM_ERROR; //Wrong checksum
		}
	}

	*count = 0;
	return UNDEFINED_ERROR; //Error by default

}


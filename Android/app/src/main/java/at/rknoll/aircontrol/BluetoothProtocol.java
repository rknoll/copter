package at.rknoll.aircontrol;

public class BluetoothProtocol {

	public static byte[] sendCommand(int throttle, float rotate, float pitch, float roll) {
		if (throttle < 0) throttle = 0;
		if (throttle > 100) throttle = 100;

		byte[] command = new byte[19];

		command[0] = (byte) 0xC3; // CONTROL Package

		command[1] = (byte) ((throttle >> 8) & 0xFF);
		command[2] = (byte) ((throttle) & 0xFF);

		// rotate
		int rotateBits = Float.floatToIntBits(rotate);
		command[3] = (byte) (rotateBits >> 24);
		command[4] = (byte) (rotateBits >> 16);
		command[5] = (byte) (rotateBits >> 8);
		command[6] = (byte) (rotateBits);

		// pitch
		int pitchBits = Float.floatToIntBits(pitch);
		command[7] = (byte) (pitchBits >> 24);
		command[8] = (byte) (pitchBits >> 16);
		command[9] = (byte) (pitchBits >> 8);
		command[10] = (byte) (pitchBits);

		// roll
		int rollBits = Float.floatToIntBits(roll);
		command[11] = (byte) (rollBits >> 24);
		command[12] = (byte) (rollBits >> 16);
		command[13] = (byte) (rollBits >> 8);
		command[14] = (byte) (rollBits);

		int checksum = calculateChecksum(command);

		command[15] = (byte) ((checksum >> 24) & 0xFF);
		command[16] = (byte) ((checksum >> 16) & 0xFF);
		command[17] = (byte) ((checksum >> 8) & 0xFF);
		command[18] = (byte) ((checksum) & 0xFF);

		return command;
	}

	public static byte[] sendCommand(int throttle) {
		return sendCommand(throttle, 0, 0, 0);
	}

	private static int calculateChecksum(byte[] command) {
		int checksum = (command[0] & 0xFF);
		checksum ^= (((command[1] & 0xFF) << 8 | (command[2] & 0xFF)) & 0xFFFF);

		for (int i = 3; i < 14; i += 4) {
			checksum ^= ((command[i] & 0xFF) << 24 | (command[i + 1] & 0xFF) << 16
					| (command[i + 2] & 0xFF) << 8 | (command[i + 3] & 0xFF));
		}

		return checksum;
	}
}

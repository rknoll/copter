package at.rknoll.aircontrol;

public class BluetoothProtocol {
	public static byte[] sendCommand(int throttle) {
		if (throttle < 0) throttle = 0;
		if (throttle > 100) throttle = 100;

		byte[] command = new byte[19];

		command[0] = 0; // CONTROL Package

		command[1] = (byte) ((throttle >> 8) & 0xFF);
		command[2] = (byte) ((throttle) & 0xFF);

		// rotate
		command[3] = 0;
		command[4] = 0;
		command[5] = 0;
		command[6] = 0;

		// pitch
		command[7] = 0;
		command[8] = 0;
		command[9] = 0;
		command[10] = 0;

		// roll
		command[11] = 0;
		command[12] = 0;
		command[13] = 0;
		command[14] = 0;

		int checksum = calculateChecksum(command);

		command[15] = (byte) ((checksum >> 24) & 0xFF);
		command[16] = (byte) ((checksum >> 16) & 0xFF);
		command[17] = (byte) ((checksum >> 8) & 0xFF);
		command[18] = (byte) ((checksum) & 0xFF);

		return command;
	}

	private static int calculateChecksum(byte[] command) {
		int checksum = command[0];
		checksum ^= ((command[1] << 8 | command[2]) & 0xFFFF);

		for (int i = 3; i < 14; i += 4) {
			checksum ^= (command[i] << 24 | command[i + 1] << 16
					| command[i + 2] << 8 | command[i + 3]);
		}

		return checksum;
	}
}

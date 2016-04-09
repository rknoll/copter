package at.rknoll.aircontrol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

	private BluetoothAdapter ba;
	private OutputStream stream = null;
	private BluetoothConnector.BluetoothSocketWrapper connect = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Button start = (Button) findViewById(R.id.btnStart);
		Button stop = (Button) findViewById(R.id.btnStop);
		SeekBar seekBar = (SeekBar) findViewById(R.id.throttle);

		if (start != null) {
			start.setOnClickListener(
					new View.OnClickListener() {
						@Override
						public void onClick(View v) {
							startDrone();
						}
					}
			);
		}
		if (stop != null) {
			stop.setOnClickListener(
					new View.OnClickListener() {
						@Override
						public void onClick(View v) {
							stopDrone();
						}
					}
			);
		}
		if (seekBar != null) {
			seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
					sendCommand(BluetoothProtocol.sendCommand(progress));
				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {

				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {

				}
			});
		}
	}

	private void connect() {
		if (stream != null) return;
		Toast.makeText(this, "Connected", Toast.LENGTH_SHORT).show();

		ba = BluetoothAdapter.getDefaultAdapter();
		Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		startActivityForResult(turnOn, 0);
		Set<BluetoothDevice> pairedDevices;
		pairedDevices = ba.getBondedDevices();
		for (BluetoothDevice device : pairedDevices) {
			if (device.getName().equals("xRemote")) {

				BluetoothConnector connector = new BluetoothConnector(device, true, ba, null);
				try {
					connect = connector.connect();
				} catch (IOException e) {
					Toast.makeText(this, "Error 1 " + e.getMessage(), Toast.LENGTH_SHORT).show();
				}

				ba.cancelDiscovery();

				try {
					stream = connect.getOutputStream();
				} catch (IOException e) {
					Toast.makeText(this, "Error 2 " + e.getMessage(), Toast.LENGTH_SHORT).show();
				}

				break;
			}
		}
	}

	@Override
	protected void onStop() {
		super.onStop();
		disconnect();
	}

	private void disconnect() {
		if (stream == null) return;
		try {
			stream.close();
		} catch (IOException e) {
			Toast.makeText(this, "Error 3 " + e.getMessage(), Toast.LENGTH_SHORT).show();
		}
		try {
			connect.close();
		} catch (IOException e) {
			Toast.makeText(this, "Error 4 " + e.getMessage(), Toast.LENGTH_SHORT).show();
		}

		stream = null;
		connect = null;
		Toast.makeText(this, "Disconnected", Toast.LENGTH_SHORT).show();
	}

	private void startDrone() {
		sendCommand(BluetoothProtocol.sendCommand(10));
	}


	private void stopDrone() {
		sendCommand(BluetoothProtocol.sendCommand(0));
	}


	private void sendCommand(byte[] command) {
		connect();
		if (stream == null) return;

		try {
			stream.write(command);
		} catch (IOException e) {
			Toast.makeText(this, "Error 5 " + e.getMessage(), Toast.LENGTH_SHORT).show();
		}
	}
}

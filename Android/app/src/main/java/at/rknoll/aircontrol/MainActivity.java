package at.rknoll.aircontrol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;

import static android.hardware.SensorManager.SENSOR_DELAY_GAME;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

	private OutputStream stream = null;
	private BluetoothConnector.BluetoothSocketWrapper connect = null;
	private Thread t;
	private SensorManager mSensorManager;
	private Sensor mSensor;
	private TextView mTextView;

	private boolean started = false;
	private float yawCalib = 0, pitchCalib = 0, rollCalib = 0;

	@Override
	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_GAME_ROTATION_VECTOR) {
			final float[] mRotationMatrix = new float[16];
			final float[] mRotationMatrix2 = new float[16];
			final float[] mOrientationValues = new float[4];

			SensorManager.getRotationMatrixFromVector(
					mRotationMatrix, event.values);

			SensorManager
					.remapCoordinateSystem(mRotationMatrix,
							SensorManager.AXIS_X, SensorManager.AXIS_Z,
							mRotationMatrix2);

			SensorManager.getOrientation(mRotationMatrix2, mOrientationValues);

			final float yaw = mOrientationValues[0]; // turn
			final float pitch = mOrientationValues[1]; // forward backward
			final float roll = mOrientationValues[2]; // left right

			synchronized (data) {
				if (!started) {
					yawCalib = yaw;
					pitchCalib = pitch;
					rollCalib = roll;
				}
				data.rotate = (float) ((yaw - yawCalib) * 180 / Math.PI);
				data.pitch = -(float) ((pitch - pitchCalib) * 180 / Math.PI);
				data.roll = (float) ((roll - rollCalib) * 180 / Math.PI);
			}

			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					final StringBuilder sb = new StringBuilder();
					synchronized (data) {
						sb.append(data.throttle);
						sb.append("\n");
						sb.append(data.rotate);
						sb.append("\n");
						sb.append(data.pitch);
						sb.append("\n");
						sb.append(data.roll);
					}
					mTextView.setText(sb.toString());
				}
			});
		}

	}

	@Override
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
	}

	private class ControlData {
		int throttle;
		float rotate;
		float pitch;
		float roll;
	}

	private final ControlData data = new ControlData();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		Button start = (Button) findViewById(R.id.btnStart);
		Button stop = (Button) findViewById(R.id.btnStop);
		final SeekBar seekBar = (SeekBar) findViewById(R.id.throttle);
		mTextView = (TextView) findViewById(R.id.textView);

		if (start != null) {
			start.setOnClickListener(
					new View.OnClickListener() {
						@Override
						public void onClick(View v) {
							if (seekBar != null) {
								seekBar.setProgress(10);
							}
							synchronized (data) {
								started = true;
								data.throttle = 10;
							}
						}
					}
			);
		}
		if (stop != null) {
			stop.setOnClickListener(
					new View.OnClickListener() {
						@Override
						public void onClick(View v) {
							synchronized (data) {
								started = false;
								data.throttle = 0;
							}
						}
					}
			);
		}
		if (seekBar != null) {
			seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
				@Override
				public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
					synchronized (data) {
						data.throttle = progress;
					}
				}

				@Override
				public void onStartTrackingTouch(SeekBar seekBar) {
				}

				@Override
				public void onStopTrackingTouch(SeekBar seekBar) {
				}
			});
		}

		mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR);
	}

	@Override
	protected void onStart() {
		super.onStart();

		t = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!Thread.interrupted()) {
					try {
						Thread.sleep(50);
					} catch (InterruptedException e) {
						return;
					}
					int throttle;
					float rotate, pitch, roll;
					synchronized (data) {
						if (started) {
							throttle = data.throttle;
							rotate = data.rotate / 3;
							pitch = data.pitch / 3;
							roll = data.roll / 3;
						} else {
							throttle = 0;
							rotate = 0;
							pitch = 0;
							roll = 0;
						}
					}

					sendCommand(BluetoothProtocol.sendCommand(throttle, rotate, pitch, roll));
				}
			}
		});
		t.start();

		mSensorManager.registerListener(this, mSensor, SENSOR_DELAY_GAME);
	}

	private void showToast(final String message) {
		final Context context = this;
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
			}
		});
	}

	private void connect() {
		if (stream != null) return;
		showToast("Connecting");

		BluetoothAdapter ba = BluetoothAdapter.getDefaultAdapter();
		Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		startActivityForResult(turnOn, 0);
		Set<BluetoothDevice> pairedDevices;
		pairedDevices = ba.getBondedDevices();
		for (BluetoothDevice device : pairedDevices) {
			if (device.getName().equals("xRemote")) {
				try {
					BluetoothConnector connector = new BluetoothConnector(device, true, ba, null);
					connect = connector.connect();
					ba.cancelDiscovery();
					stream = connect.getOutputStream();
					showToast("Connected");
				} catch (IOException e) {
					showToast("Error " + e.getMessage());
					connect = null;
					stream = null;
				}

				break;
			}
		}
	}

	@Override
	protected void onStop() {
		super.onStop();
		disconnect();
		try {
			t.interrupt();
			t.join();
		} catch (InterruptedException ignored) {
		}
		mSensorManager.unregisterListener(this);
	}

	private void disconnect() {
		if (stream == null) return;
		try {
			stream.close();
		} catch (IOException e) {
			showToast("Error " + e.getMessage());
		}
		try {
			connect.close();
		} catch (IOException e) {
			showToast("Error " + e.getMessage());
		}

		connect = null;
		stream = null;
		showToast("Disconnected");
	}

	private void sendCommand(byte[] command) {
		connect();
		if (stream == null) return;

		try {
			stream.write(command);
		} catch (IOException e) {
			showToast("Error " + e.getMessage());
			connect = null;
			stream = null;
		}
	}
}

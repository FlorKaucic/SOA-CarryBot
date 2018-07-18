package com.carrybot.server.activities;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;
import android.widget.Toast;

import com.carrybot.server.R;
import com.carrybot.server.model.BluetoothThread;
import com.carrybot.server.model.DeviceListAdapter;
import com.carrybot.server.model.ServerData;

import java.io.IOException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.UUID;

public class BluetoothListActivity extends AppCompatActivity {
    private static final int TURN_BLUETOOTH_ON_REQUEST_CODE = 1000;
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<BluetoothDevice> deviceList;
    private Menu menu;
    private DeviceListAdapter deviceListAdapter;
    private int devicePosition;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_list);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        Toolbar toolbar = findViewById(R.id.bluetoothToolbar);
        setSupportActionBar(toolbar);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        deviceList = new ArrayList<>();
        deviceListAdapter = new DeviceListAdapter(deviceList, this);
        deviceListAdapter.setListener(connectButtonClickListener);

        ListView deviceList = findViewById(R.id.deviceListView);
        deviceList.setAdapter(deviceListAdapter);

        IntentFilter filter = new IntentFilter();

        filter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);

        registerReceiver(receiver, filter);
    }

    private DeviceListAdapter.ConnectButtonClickListener connectButtonClickListener =
            new DeviceListAdapter.ConnectButtonClickListener() {
                @Override
                public void onClick(int position) {
                    BluetoothListActivity.this.devicePosition = position;
                    BluetoothDevice device = deviceList.get(position);
                    if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                        Method createBondMethod = null;
                        try {
                            Class class1 = Class.forName("android.bluetooth.BluetoothDevice");
                            createBondMethod = class1.getMethod("createBond");
                            Log.i("BLUETOOTH", String.valueOf(createBondMethod.invoke(device)));
                        } catch (Exception e) {
                            Log.e("BLUETOOTH 1", e.getMessage());
                        }
                    } else {
                        try {
                            Log.i("BLUETOOTH", device.getName());
                            device = bluetoothAdapter.getRemoteDevice(device.getAddress());
                            BluetoothSocket bluetoothSocket = device.createRfcommSocketToServiceRecord(BTMODULEUUID);
                            bluetoothSocket.connect();
                            //Log.i("BLUETOOTH", "The cake is a lie");
                            BluetoothThread bluetoothThread = new BluetoothThread(bluetoothSocket, BluetoothListActivity.this);
                            bluetoothThread.start();
                            ServerData.setBluetoothThread(bluetoothThread);
                            ServerData.sendToCarrybot("PING\r\n");
                        } catch (Exception e) {
                            try {
                                e.printStackTrace();
                                Log.e("BLUETOOTH 2", e.getMessage());
                                BluetoothSocket bluetoothSocket = (BluetoothSocket) device.getClass().getMethod("createRfcommSocket", new Class<?>[] {Integer.TYPE}).invoke(device, 4);
                                bluetoothSocket.connect();
                                BluetoothThread bluetoothThread = new BluetoothThread(bluetoothSocket, BluetoothListActivity.this);
                                bluetoothThread.start();
                                ServerData.setBluetoothThread(bluetoothThread);
                                ServerData.sendToCarrybot("PING\r\n");
                            } catch (Exception er) {
                                er.printStackTrace();
                                Log.e("BLUETOOTH 3", er.getMessage());
                            }
                        }
                    }
                }
            };

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        this.menu = menu;
        getMenuInflater().inflate(R.menu.menu_bluetooth_list, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        switch (id) {
            case R.id.action_sync:
                if(item.getTitle().equals(getString(R.string.action_sync_start))) {
                    if (!bluetoothAdapter.isEnabled()) {
                        Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(intent, TURN_BLUETOOTH_ON_REQUEST_CODE);
                    } else {
                        startScanningForDevices();
                    }
                } else {
                    stopScanningForDevices();
                }
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    private void goToSettingsActivity() {
        Intent intent = new Intent(this, SettingsActivity.class);
        startActivity(intent);
    }

    private void stopScanningForDevices() {
        bluetoothAdapter.cancelDiscovery();
    }

    private void startScanningForDevices() {
        bluetoothAdapter.startDiscovery();
    }

    private void setSyncActionStart() {
        MenuItem syncAction = this.menu.getItem(0);
        syncAction.setIcon(R.drawable.ic_sync_start);
        syncAction.setTitle(R.string.action_sync_start);
    }

    private void setSyncActionStop() {
        MenuItem syncAction = this.menu.getItem(0);
        syncAction.setIcon(R.drawable.ic_sync_stop);
        syncAction.setTitle(R.string.action_sync_stop);
    }

    private final BroadcastReceiver receiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {

            String action = intent.getAction();

            switch (action) {
                case BluetoothAdapter.ACTION_STATE_CHANGED:
                    handleBluetoothStateChange(intent);
                    break;
                case BluetoothAdapter.ACTION_DISCOVERY_STARTED:
                    handleDiscoveryStarted();
                    break;
                case BluetoothAdapter.ACTION_DISCOVERY_FINISHED:
                    handleDiscoveryFinished();
                    break;
                case BluetoothDevice.ACTION_FOUND:
                    handleFoundDevice(intent);
                    break;
                case BluetoothDevice.ACTION_BOND_STATE_CHANGED:
                    handleBondChange(intent);
                    break;
            }
        }

        private void handleBondChange(Intent intent) {
            final int state = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, BluetoothDevice.ERROR);
            final int prevState = intent.getIntExtra(BluetoothDevice.EXTRA_PREVIOUS_BOND_STATE, BluetoothDevice.ERROR);
            Toast.makeText(BluetoothListActivity.this, "BOND CHANGE", Toast.LENGTH_SHORT).show();

            if (state == BluetoothDevice.BOND_BONDED && prevState == BluetoothDevice.BOND_BONDING)
            {
                Toast.makeText(BluetoothListActivity.this, "BONDED", Toast.LENGTH_SHORT).show();
                BluetoothDevice device = deviceListAdapter.getItem(devicePosition);

                BluetoothSocket bluetoothSocket;
                try {
                    bluetoothSocket = device.createRfcommSocketToServiceRecord(BTMODULEUUID);
                    bluetoothSocket.connect();
                    BluetoothThread bluetoothThread = new BluetoothThread(bluetoothSocket, BluetoothListActivity.this);
                    bluetoothThread.start();
                    ServerData.setBluetoothThread(bluetoothThread);
                    ServerData.sendToCarrybot("PING\r\n");
                } catch (IOException e) {
                    Toast.makeText(BluetoothListActivity.this, R.string.socket_failed, Toast.LENGTH_SHORT).show();
                }
            }
        }

        private void handleDiscoveryFinished() {
            setSyncActionStart();
        }

        private void handleDiscoveryStarted() {
            setSyncActionStop();
            deviceList.clear();
            deviceListAdapter.notifyDataSetChanged();
        }

        private void handleBluetoothStateChange(Intent intent) {
            final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);

            if (state == BluetoothAdapter.STATE_ON) {
                startScanningForDevices();
            }
        }

        private void handleFoundDevice(Intent intent) {
            BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
            deviceList.add(device);
            deviceListAdapter.notifyDataSetChanged();
        }
    };
}

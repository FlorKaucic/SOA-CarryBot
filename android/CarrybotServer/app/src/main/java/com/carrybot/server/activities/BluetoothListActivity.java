package com.carrybot.server.activities;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

import com.carrybot.server.R;
import com.carrybot.server.model.DeviceListAdapter;

import java.util.ArrayList;

public class BluetoothListActivity extends AppCompatActivity {
    private static final int TURN_BLUETOOTH_ON_REQUEST_CODE = 1000;
    private BluetoothAdapter bluetoothAdapter;
    private ArrayList<BluetoothDevice> deviceList;
    private Menu menu;
    private DeviceListAdapter deviceListAdapter;

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

        registerReceiver(receiver, filter);
    }

    private DeviceListAdapter.ConnectButtonClickListener connectButtonClickListener =
            new DeviceListAdapter.ConnectButtonClickListener() {
                @Override
                public void onClick(int position) {
                    goToSettingsActivity();
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

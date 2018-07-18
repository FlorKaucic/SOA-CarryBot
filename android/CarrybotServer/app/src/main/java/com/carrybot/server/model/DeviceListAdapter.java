package com.carrybot.server.model;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;

import com.carrybot.server.R;

import java.util.ArrayList;

public class DeviceListAdapter extends BaseAdapter {
    private ArrayList<BluetoothDevice> deviceList;
    private Context context;
    private ConnectButtonClickListener listener;

    public DeviceListAdapter(ArrayList<BluetoothDevice> deviceList, Context context){
        this.deviceList  = deviceList;
        this.context = context;
    }

    public void setListener(ConnectButtonClickListener listener) {
        this.listener = listener;
    }

    @Override
    public int getCount() {
        return deviceList.size();
    }

    @Override
    public BluetoothDevice getItem(int position) {
        return deviceList.get(position);
    }

    @Override
    public long getItemId(int arg0) {
        return arg0;
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        BluetoothDevice device = getItem(position);

        View listItem = convertView;

        if(listItem == null)
            listItem = LayoutInflater.from(context).inflate(R.layout.devicelist_item, parent,false);

        TextView deviceName = listItem.findViewById(R.id.deviceNameTextView);
        deviceName.setText(device.getName());

        TextView deviceMacAddress = listItem.findViewById(R.id.deviceMacAddressTextView);
        deviceMacAddress.setText(device.getAddress());

        ImageButton deviceConnectButton = listItem.findViewById(R.id.deviceConnectButton);
        deviceConnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (listener != null) {
                    listener.onClick(position);
                }
            }
        });

        return listItem;
    }


    public interface ConnectButtonClickListener {
        void onClick(int position);
    }
}


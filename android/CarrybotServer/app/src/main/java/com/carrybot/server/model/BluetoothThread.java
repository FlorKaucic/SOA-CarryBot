package com.carrybot.server.model;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Handler;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class BluetoothThread extends Thread {
    private static final int BLUETOOTH_HANDLER_CODE = 10;
    private StringBuilder messageStringBuilder = new StringBuilder();
    private Handler bluetoothHandler;
    private InputStream bluetoothInStream;
    private OutputStream bluetoothOutStream;
    private Context context;

    public BluetoothThread(BluetoothSocket socket, Context context) {
        this.context = context;
        bluetoothHandler = getBluetoothHandler();
        try {
            bluetoothInStream = socket.getInputStream();
            bluetoothOutStream = socket.getOutputStream();
        } catch (IOException e) { }
    }

    public void run() {
        byte[] buffer = new byte[256];
        int bytes;

        while (true) {
            try {
                bytes = bluetoothInStream.read(buffer);
                String readMessage = new String(buffer, 0, bytes);

                bluetoothHandler.obtainMessage(BLUETOOTH_HANDLER_CODE, bytes, -1, readMessage).sendToTarget();
            } catch (IOException e) {
                break;
            }
        }
    }

    public void write(String input) {
        byte[] msgBuffer = input.getBytes();
        try {
            bluetoothOutStream.write(msgBuffer);
        } catch (IOException e) {
            Toast.makeText(context,"La conexion falló", Toast.LENGTH_LONG).show();
        }
    }
    @SuppressLint("HandlerLeak")
    private Handler getBluetoothHandler() {
        return new Handler() {
            public void handleMessage(android.os.Message msg) {
                if (msg.what == BLUETOOTH_HANDLER_CODE) {
                    String readMessage = (String) msg.obj;
                    messageStringBuilder.append(readMessage);
                    int endOfLineIndex = messageStringBuilder.indexOf("\r\n");

                    if (endOfLineIndex > 0)
                    {
                        String message = messageStringBuilder.substring(0, endOfLineIndex);
                        handleMessageReceived(message);
                        messageStringBuilder.delete(0, messageStringBuilder.length());
                    }
                }
            }

            private void handleMessageReceived(String message) {
                Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
            }
        };
    }
}

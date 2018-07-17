package com.carrybot.server.activities;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.carrybot.server.R;
import com.carrybot.server.model.ServerData;

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        String macAddress = this.getMacAddress();
        ServerData.setMacAddress(macAddress);

        TextView macAddressTextView = findViewById(R.id.macAddress);
        macAddressTextView.setText(macAddress);

    }

    private String getMacAddress() {
        return android.provider.Settings.Secure.getString(getContentResolver(),
                getString(R.string.properties_bluetooth_address));
    }

    public void goToWaitingScreen(View view) {
        Intent intent = new Intent(this, WaitingActivity.class);
        startActivity(intent);
        this.finish();
    }
}

package com.carrybot.server.activities;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.carrybot.server.R;
import com.carrybot.server.model.FetchMenuTask;
import com.carrybot.server.model.ServerData;

public class SettingsActivity extends AppCompatActivity {
    private static boolean FLAG_MENU_FETCHED = false;
    private static boolean FLAG_CONFIG_DONE = true;

    private FetchMenuTask.OnFetchMenuListener onFetchMenuListener = new FetchMenuTask.OnFetchMenuListener() {
        @Override
        public void onSuccess() {
            FLAG_MENU_FETCHED = true;

            if(FLAG_CONFIG_DONE) {
                goToWaitingScreen();
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        String macAddress = this.getMacAddress();
        ServerData.setMacAddress(macAddress);

        TextView macAddressTextView = findViewById(R.id.macAddress);
        macAddressTextView.setText(macAddress);

        final Button nextButton = findViewById(R.id.nextbutton);
        nextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                nextButton.setEnabled(false);

                FetchMenuTask task = new FetchMenuTask();
                task.setOnFetchMenuListener(onFetchMenuListener);
                task.run();

                // TODO: add config call here
            }
        });

    }

    private String getMacAddress() {
        return android.provider.Settings.Secure.getString(getContentResolver(),
                getString(R.string.properties_bluetooth_address));
    }

    public void goToWaitingScreen() {
        Intent intent = new Intent(this, WaitingActivity.class);
        startActivity(intent);
        this.finish();
    }
}

package com.carrybot.server.activities;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import com.carrybot.server.R;
import com.carrybot.server.services.MessagingService;

import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class WaitingActivity extends AppCompatActivity {
    public static final int LIGHT_LEVEL_THRESHOLD = 30;
    private SensorManager sensorManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_waiting);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        Intent intent = new Intent(this, MessagingService.class);
        startService(intent);

        sensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);

        Sensor lightSensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        if(lightSensor != null) {
            sensorManager.registerListener(
                    lightSensorListener,
                    lightSensor,
                    SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    public void goToOrderList(View view) {
        sensorManager.unregisterListener(lightSensorListener);
        Intent intent = new Intent(this, OrdersActivity.class);
        startActivity(intent);
    }

    @Override
    public void onBackPressed() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setMessage(this.getResources().getString(R.string.exit_attemp_message));
        dialog.setPositiveButton(this.getResources().getString(R.string.button_exit), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface paramDialogInterface, int paramInt) {
                moveTaskToBack(true);
                android.os.Process.killProcess(android.os.Process.myPid());
                System.exit(1);
            }
        });
        dialog.setNegativeButton(this.getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface paramDialogInterface, int paramInt) {}
        });
        dialog.show();
    }

    private final SensorEventListener lightSensorListener = new SensorEventListener() {
        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {}

        @Override
        public void onSensorChanged(SensorEvent event) {
            if(event.sensor.getType() == Sensor.TYPE_LIGHT){
                float lightLevel = event.values[0];
                ((TextView)findViewById(R.id.waitingText)).setText("Level: "+lightLevel+String.valueOf(lightLevel>LIGHT_LEVEL_THRESHOLD));
                if(lightLevel > LIGHT_LEVEL_THRESHOLD) {
                    goToOrderList(findViewById(R.id.goToOrderListButton));
                }
            }
        }

    };
}

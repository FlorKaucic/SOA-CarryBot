package com.carrybot.server.activities;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.view.Menu;
import android.widget.ListView;

import com.carrybot.server.R;
import com.carrybot.server.model.Item;
import com.carrybot.server.model.Order;
import com.carrybot.server.model.OrderListAdapter;
import com.carrybot.server.model.ServerData;
import com.carrybot.server.model.User;

public class OrdersActivity extends AppCompatActivity implements SensorEventListener {
    private SensorManager sensorManager;
    private long lastUpdate;
    OrderListAdapter orderListAdapter;
    private Sensor lightSensor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_orders);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        Toolbar toolbar = findViewById(R.id.ordersToolbar);
        setSupportActionBar(toolbar);

        orderListAdapter = new OrderListAdapter(ServerData.getOrderList(), this);

        ListView orderListView = findViewById(R.id.orderListView);
        orderListView.setAdapter(orderListAdapter);

        // TODO: delete this
        User client = new User("1", "Juan", 2,"FFFFFF:FFFF");
        Item item = new Item();
        item.setId(1);
        item.setQuantity(3);
        Order order = new Order(client, item);
        ServerData.addOrder(order);
        client = new User("5", "Pepe", 2,"FFFFFF:FFFF");
        item = new Item();
        item.setId(3);
        item.setQuantity(5);
        order = new Order(client, item);
        ServerData.addOrder(order);
        client = new User("2", "Diego", 2,"FFFFFF:FFFF");
        item = new Item();
        item.setId(5);
        item.setQuantity(1);
        order = new Order(client, item);
        ServerData.addOrder(order);
        client = new User("3", "Maria", 2,"FFFFFF:FFFF");
        item = new Item();
        item.setId(9);
        item.setQuantity(2);
        order = new Order(client, item);
        ServerData.addOrder(order);
        client = new User("4", "Paula", 2,"FFFFFF:FFFF");
        item = new Item();
        item.setId(10);
        item.setQuantity(1);
        order = new Order(client, item);
        ServerData.addOrder(order);

        ServerData.registerOrderListChangeListener(new OrderChangeListener() {
            @Override
            public void onChange() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        orderListAdapter.notifyDataSetChanged();
                    }
                });
            }
        });

        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        lastUpdate = System.currentTimeMillis();

        lightSensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        if(lightSensor != null) {
            sensorManager.registerListener(
                    this,
                    lightSensor,
                    SensorManager.SENSOR_DELAY_NORMAL);
        }
    }

    @Override
    public void onBackPressed() {
        sensorManager.unregisterListener(this, lightSensor);
        goToWaitingActivity();
        finish();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_orders, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        Intent intent;

        //noinspection SimplifiableIfStatement
        switch(id) {
            case R.id.action_about:
                intent = new Intent(this, AboutCarrybotActivity.class);
                startActivity(intent);
                break;
            case R.id.action_manual:
                intent = new Intent(this, ManualControlActivity.class);
                startActivity(intent);
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    public void goToClientList(View view) {
        Intent intent = new Intent(this, ClientListActivity.class);
        startActivity(intent);
    }

    public interface OrderChangeListener {
        void onChange();
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            getAccelerometer(event);
        } else if(event.sensor.getType() == Sensor.TYPE_LIGHT){
            float lightLevel = event.values[0];
            if(lightLevel <= WaitingActivity.LIGHT_LEVEL_THRESHOLD) {
                sensorManager.unregisterListener(this, lightSensor);
                goToWaitingActivity();
                finish();
            }
        }

    }

    private void goToWaitingActivity() {
        Intent intent = new Intent(this, WaitingActivity.class);
        startActivity(intent);
    }

    private void getAccelerometer(SensorEvent event) {
        float[] values = event.values;

        // Movement
        float x = values[0];
        float y = values[1];
        float z = values[2];

        float accelerationSquareRoot = (x * x + y * y + z * z)
                / (SensorManager.GRAVITY_EARTH * SensorManager.GRAVITY_EARTH);
        long actualTime = event.timestamp;
        if (accelerationSquareRoot >= 2) //
        {
            if (actualTime - lastUpdate < 200) {
                return;
            }
            lastUpdate = actualTime;

            final Order removedOrder = ServerData.removeOrder(ServerData.FIRST_POSITION);
            if(removedOrder != null) {
                Snackbar.make(
                        findViewById(R.id.orderListView),
                        R.string.orders_canceled,
                        Snackbar.LENGTH_LONG
                ).setAction(
                        R.string.button_cancel,
                        new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                ServerData.putOrderBack(removedOrder);
                            }
                        }
                ).show();
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    @Override
    protected void onResume() {
        super.onResume();
        // register this class as a listener for the orientation and
        // accelerometer sensors
        sensorManager.registerListener(this,
                sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
                SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        // unregister listener
        super.onPause();
        sensorManager.unregisterListener(this);
    }
}

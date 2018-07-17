package com.carrybot.server.activities;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
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

public class OrdersActivity extends AppCompatActivity {
    OrderListAdapter orderListAdapter;

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

        User client = new User("1", "Juan", 2,"FFFFFF:FFFF");
        Item item = new Item();
        item.setId(1);
        item.setQuantity(2);
        Order order = new Order(client, item);
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
}

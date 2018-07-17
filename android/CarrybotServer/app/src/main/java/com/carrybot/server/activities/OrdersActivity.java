package com.carrybot.server.activities;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Menu;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.carrybot.server.R;
import com.carrybot.server.model.Order;
import com.carrybot.server.model.ServerData;
import com.carrybot.server.model.Product;

import java.util.ArrayList;

public class OrdersActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_orders);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        Toolbar toolbar = findViewById(R.id.ordersToolbar);
        setSupportActionBar(toolbar);
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

    private class OrderMapAdapter extends BaseAdapter {
        private ArrayList<Order> orderList;
        private ArrayList<Product> menu;

        OrderMapAdapter(ArrayList<Order> orderList){
            this.orderList  = orderList;
            this.menu = ServerData.getMenu();
        }

        @Override
        public int getCount() {
            return orderList.size();
        }

        @Override
        public Order getItem(int position) {
            return orderList.get(position);
        }

        @Override
        public long getItemId(int arg0) {
            return arg0;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            Order order = getItem(position);

            View listItem = convertView;

            if(listItem == null)
                listItem = LayoutInflater.from(getApplicationContext()).inflate(R.layout.clientlist_item, parent,false);

            int itemId = order.getItem().getId();
            TextView itemName = listItem.findViewById(R.id.itemNameTextView);
            itemName.setText("");

            return listItem;
        }
    }
}

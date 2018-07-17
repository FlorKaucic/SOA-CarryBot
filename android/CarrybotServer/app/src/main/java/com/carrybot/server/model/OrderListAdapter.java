package com.carrybot.server.model;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.carrybot.server.R;

import java.util.ArrayList;

public class OrderListAdapter extends BaseAdapter {
    private ArrayList<Order> orderList;
    private Context context;

    public OrderListAdapter(ArrayList<Order> orderList, Context context){
        this.orderList  = orderList;
        this.context = context;
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
            listItem = LayoutInflater.from(context).inflate(R.layout.orderlist_item, parent,false);

        int itemId = order.getItem().getId();
        int itemQuantity = order.getItem().getQuantity();

        Product product = ServerData.getProduct(itemId);

        ImageView itemImageView = listItem.findViewById(R.id.itemImageView);
        itemImageView.setImageBitmap(product.getImageBitmap());

        TextView itemQuantityTextView = listItem.findViewById(R.id.itemQuantityTextView);
        itemQuantityTextView.setText(String.valueOf(itemQuantity));

        TextView itemName = listItem.findViewById(R.id.itemNameTextView);
        itemName.setText(product.getDescription());

        TextView userNameTextView = listItem.findViewById(R.id.userNameTextView);
        userNameTextView.setText(order.getClient().getName());

        TextView userMacAddressTextView = listItem.findViewById(R.id.userMacAddressTextView);
        userMacAddressTextView.setText(order.getClient().getMacAddress());

        return listItem;
    }
}


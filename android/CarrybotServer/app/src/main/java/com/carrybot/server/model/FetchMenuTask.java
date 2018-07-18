package com.carrybot.server.model;

import android.support.annotation.NonNull;

import com.carrybot.server.model.DownloadImageTask;
import com.carrybot.server.model.Product;
import com.carrybot.server.model.ServerData;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class FetchMenuTask extends Thread implements Runnable {
    private OnFetchMenuListener onFetchMenuListener;
    private DatabaseReference menuReference;

    private ValueEventListener menuListener = new ValueEventListener() {
        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
            menuReference.removeEventListener(menuProductListener);
            menuReference.removeEventListener(menuListener);
            onFetchMenuListener.onSuccess();
        }
        public void onCancelled(@NonNull DatabaseError error) { }
    };

    private ChildEventListener menuProductListener = new ChildEventListener() {
        @Override
        public void onChildAdded(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {
            Product addedProduct = dataSnapshot.getValue(Product.class);
            new DownloadImageTask(addedProduct).execute();
            ServerData.addProduct(addedProduct);
        }

        @Override
        public void onChildRemoved(@NonNull DataSnapshot dataSnapshot) {}

        @Override
        public void onChildChanged(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {}

        @Override
        public void onChildMoved(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {}

        @Override
        public void onCancelled(@NonNull DatabaseError error) {}
    };

    public void setOnFetchMenuListener(OnFetchMenuListener onFetchMenuListener) {
        this.onFetchMenuListener = onFetchMenuListener;
    }

    @Override
    public void run() {
        menuReference = FirebaseDatabase.getInstance().getReference("items");
        menuReference.addChildEventListener(menuProductListener);
        menuReference.addListenerForSingleValueEvent(menuListener);
    }

    public interface OnFetchMenuListener {
        void onSuccess();
    }
}

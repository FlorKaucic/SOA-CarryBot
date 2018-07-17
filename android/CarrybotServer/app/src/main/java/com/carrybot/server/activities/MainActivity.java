package com.carrybot.server.activities;

import android.Manifest;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.carrybot.server.R;
import com.carrybot.server.model.Haversine;
import com.carrybot.server.model.Restaurant;
import com.carrybot.server.model.ServerData;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    private final int ACCESS_FINE_LOCATION_PERMISSION_REQUEST = 5000;
    private static boolean FLAG_LOCATION = false;
    private static boolean FLAG_RESTAURANT_LIST = false;
    private LocationListener locationListener;
    private LocationManager locationManager;
    private Double latitude = null;
    private Double longitude = null;

    private ArrayList<Restaurant> restaurantList;

    private DatabaseReference restaurants;

    private ValueEventListener restaurantListListener = new ValueEventListener() {
        public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
            restaurants.removeEventListener(restaurantInstanceListener);
            restaurants.removeEventListener(restaurantListListener);

            FLAG_RESTAURANT_LIST = true;

            if(FLAG_LOCATION) {
                setCurrentRestaurant();
            }
        }
        public void onCancelled(@NonNull DatabaseError error) { }
    };

    private ChildEventListener restaurantInstanceListener = new ChildEventListener() {
        @Override
        public void onChildAdded(@NonNull DataSnapshot dataSnapshot, String prevChildKey) {
            Restaurant addedLocation = dataSnapshot.getValue(Restaurant.class);
            restaurantList.add(addedLocation);
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    @Override
    protected void onStart() {
        super.onStart();

        if(!ServerData.isServerIdSet()) {
            restaurantList = new ArrayList<>();
            this.requestRestaurants();
            this.requestLocation();
        } else {
            this.updateUI(ServerData.getRestaurantName());
            this.goToWaiting();
        }
    }

    private void setCurrentRestaurant() {
        Double minDistance = null;
        Integer minDistanceServerId = null;
        String restaurantName = "";

        for(Restaurant restaurant : restaurantList) {
            double distance = Haversine.getDistance(latitude, longitude,
                    restaurant.getLatitude(), restaurant.getLongitude());

            if((minDistance == null) || (distance < minDistance)) {
                minDistance = distance;
                minDistanceServerId = restaurant.getId();
                restaurantName = restaurant.getName();
            }
        }

        //noinspection ConstantConditions
        ServerData.setServerId(minDistanceServerId);
        ServerData.setRestaurantName(restaurantName);

        this.updateUI(restaurantName);
    }

    private void updateUI(String restaurantName) {
        ProgressBar spinner = findViewById(R.id.spinner);
        spinner.setVisibility(View.INVISIBLE);

        TextView statusTextView = findViewById(R.id.statusTextView);
        statusTextView.setText(getString(R.string.main_restaurant_found));

        TextView restaurantTextView = findViewById(R.id.restaurantTextView);
        restaurantTextView.setText(getString(R.string.main_restaurant, restaurantName));

        Button continueButton = findViewById(R.id.continueButton);
        continueButton.setEnabled(true);
    }

    private void requestLocation() {
        locationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);

        boolean gpsEnabled = false;
        boolean networkEnabled = false;

        try {
            gpsEnabled = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
        } catch(Exception ignored) {}

        try {
            networkEnabled = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        } catch(Exception ignored) {}

        if(!gpsEnabled && !networkEnabled) {
            // notify user
            AlertDialog.Builder dialog = new AlertDialog.Builder(this);
            dialog.setMessage(this.getResources().getString(R.string.gps_network_not_enabled));
            dialog.setPositiveButton(this.getResources().getString(R.string.open_location_settings), new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface paramDialogInterface, int paramInt) {
                    // TODO Auto-generated method stub
                    Intent myIntent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
                    startActivity(myIntent);
                    //get gps
                }
            });
            dialog.setNegativeButton(this.getString(R.string.button_cancel), new DialogInterface.OnClickListener() {

                @Override
                public void onClick(DialogInterface paramDialogInterface, int paramInt) {}
            });
            dialog.show();
        }

        locationListener = new LocationListener() {
            public void onLocationChanged(Location location) {
                latitude = location.getLatitude();
                longitude = location.getLongitude();

                locationManager.removeUpdates(locationListener);

                FLAG_LOCATION = true;

                if(FLAG_RESTAURANT_LIST) {
                    setCurrentRestaurant();
                }
            }

            public void onStatusChanged(String provider, int status, Bundle extras) {
            }

            public void onProviderEnabled(String provider) {
            }

            public void onProviderDisabled(String provider) {
            }
        };

        this.requestLocationUpdates();
    }

    private void requestLocationUpdates() {
        int permissionCheck = ContextCompat.checkSelfPermission(this,
                Manifest.permission.ACCESS_FINE_LOCATION);

        if (permissionCheck == PackageManager.PERMISSION_GRANTED) {
            locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, locationListener);
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
        } else {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    ACCESS_FINE_LOCATION_PERMISSION_REQUEST);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {
        switch (requestCode) {
            case ACCESS_FINE_LOCATION_PERMISSION_REQUEST: {
                requestLocationUpdates();
                break;
            }
        }
    }

    public void requestRestaurants() {
        restaurants = FirebaseDatabase.getInstance().getReference("restaurants");
        restaurants.addChildEventListener(restaurantInstanceListener);
        restaurants.addListenerForSingleValueEvent(restaurantListListener);
    }

    public void goToBTList(View view) {
        Intent intent = new Intent(this, SettingsActivity.class);
        startActivity(intent);
        this.finish();
    }

    public void goToWaiting() {
        Intent intent = new Intent(this, WaitingActivity.class);
        startActivity(intent);
        this.finish();
    }
}

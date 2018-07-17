package com.carrybot.server.model;

import android.graphics.Bitmap;

public class Product {
    private int id;
    private String description;
    private String photoUrl;
    private Bitmap image;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public String getPhotoUrl() {
        return photoUrl;
    }

    public void setPhotoUrl(String photoUrl) {
        this.photoUrl = photoUrl;
    }

    public void setImageBitmap(Bitmap image) {
        this.image = image;
    }

    public Bitmap getImageBitmap() {
        return image;
    }
}


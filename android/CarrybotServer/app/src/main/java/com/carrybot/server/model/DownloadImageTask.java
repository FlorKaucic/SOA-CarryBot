package com.carrybot.server.model;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;

import java.io.InputStream;

public class DownloadImageTask extends AsyncTask<String, Void, Bitmap> {
    private Product product;

    public DownloadImageTask(Product product) {
        this.product = product;
    }

    protected Bitmap doInBackground(String... params) {
        Bitmap image = null;
        try {
            InputStream in = new java.net.URL(product.getPhotoUrl()).openStream();
            image = BitmapFactory.decodeStream(in);
        } catch (Exception ignored) {}
        return image;
    }

    protected void onPostExecute(Bitmap result) {
        product.setImageBitmap(result);
        ServerData.notifyChange();
    }
}

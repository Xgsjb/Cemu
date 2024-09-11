package info.cemu.Cemu;

import android.content.Context;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class AssetFileManager {

    private final Context context;

    public AssetFileManager(Context context) {
        this.context = context;
    }

    public void copyProdKeys() {
        copyAsset("keys.txt");
    }

    private void copyAsset(String assetName) {
        File destinationFile = new File(context.getExternalFilesDir(null), assetName);

        if (!destinationFile.exists()) {
            try (InputStream inputStream = context.getAssets().open(assetName);
                 FileOutputStream outputStream = new FileOutputStream(destinationFile)) {
                 
                byte[] buffer = new byte[1024];
                int length;
                while ((length = inputStream.read(buffer)) > 0) {
                    outputStream.write(buffer, 0, length);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
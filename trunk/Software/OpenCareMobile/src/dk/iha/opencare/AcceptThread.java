package dk.iha.opencare;

import java.io.IOException;
import java.util.UUID;

import com.google.inject.Inject;

import dk.iha.opencare.wban.AndroidBluetoothConnection;
import dk.iha.opencare.wban.Controller;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;

public class AcceptThread extends Thread {

  static final UUID                   SPP_UUID     = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
  static final String                 SERVICE_NAME = "WBANCOM";

  private final BluetoothServerSocket mServerSocket;
  private final BluetoothAdapter      mAdapter;
  private @Inject Controller          mController;

  public AcceptThread(BluetoothAdapter adapter) throws IOException {
    this.mAdapter = adapter;

    // MY_UUID is the app's UUID string, also used by the client code
    mServerSocket = mAdapter.listenUsingRfcommWithServiceRecord(SERVICE_NAME, SPP_UUID);
  }

  public void run() {
    BluetoothSocket socket = null;

    // Keep listening until exception occurs or a socket is returned
    while (true) {
      try {
        socket = mServerSocket.accept();
      } catch (IOException e) {
        break;
      }
      
      // If a connection was accepted
      if (socket != null) {
        // Do work to manage the connection (in a separate thread)
        mController.handleConnection(new AndroidBluetoothConnection(socket));
        break;
      }
    }
  }

  /** Will cancel the listening socket, and cause the thread to finish */
  public void cancel() {
    try {
      mServerSocket.close();
    } catch (IOException e) {}
  }
}
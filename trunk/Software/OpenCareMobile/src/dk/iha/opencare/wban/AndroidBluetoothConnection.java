package dk.iha.opencare.wban;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import dk.iha.opencare.Connection;

import android.bluetooth.BluetoothSocket;

public class AndroidBluetoothConnection implements Connection {

  private final BluetoothSocket mSocket;

  public AndroidBluetoothConnection(BluetoothSocket socket) {
    mSocket = socket;
  }

  @Override
  public void connect() throws IOException {
    mSocket.connect();
  }

  @Override
  public void close() throws IOException {
    mSocket.close();
  }

  @Override
  public InputStream getInputStream() throws IOException {
    return mSocket.getInputStream();
  }

  @Override
  public OutputStream getOutputStream() throws IOException {
    return mSocket.getOutputStream();
  }

}

package dk.iha.opencare.wban;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;

import dk.iha.opencare.Connection;

public class JsrBluetoothConnection implements Connection {
  
  private StreamConnection mStreamConnection;
  private final String mConnectionString;
  
  public JsrBluetoothConnection(String connectionString) {
    mConnectionString = connectionString;
  }

  @Override public void connect() throws IOException {
    mStreamConnection = (StreamConnection) Connector.open(mConnectionString);
  }

  @Override public void close() throws IOException {
    if(mStreamConnection == null)
      throw new IOException("connect() first");
    mStreamConnection.close();
  }

  @Override public InputStream getInputStream() throws IOException {
    if(mStreamConnection == null)
      throw new IOException("connect() first");
    return mStreamConnection.openInputStream();
  }

  @Override public OutputStream getOutputStream() throws IOException {
    if(mStreamConnection == null)
      throw new IOException("connect() first");
    return mStreamConnection.openOutputStream();
  }
  
}

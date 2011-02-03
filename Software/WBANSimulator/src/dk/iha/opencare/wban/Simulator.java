package dk.iha.opencare.wban;

import java.io.DataOutputStream;
import java.io.IOException;

import javax.bluetooth.BluetoothStateException;

import dk.iha.opencare.Connection;

public class Simulator {
  public static void main(String[] args) {
    try {
      for (String connectionString : ServicesSearch.scan()) {
        try {
          Connection connection = new JsrBluetoothConnection(connectionString);
          connection.connect();
          DataOutputStream dataOutputStream = new DataOutputStream(connection.getOutputStream());
          System.out.println("Connection established!");
          int i = 0;
          while(true) {
            dataOutputStream.writeUTF("Hello world " + i++);
            Thread.sleep(2000);
          }
        } catch (IOException e) {
          e.printStackTrace();
        } catch (InterruptedException e) {
          e.printStackTrace();
        }
      }
    } catch (BluetoothStateException e) {
      e.printStackTrace();
    }
  }
}

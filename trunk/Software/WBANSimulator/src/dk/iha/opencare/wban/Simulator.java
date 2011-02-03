package dk.iha.opencare.wban;

import java.io.IOException;

import javax.bluetooth.BluetoothStateException;
import javax.microedition.io.Connector;

public class Simulator {
  public static void main(String[] args) {
    try {
      for (String connectionString : ServicesSearch.scan()) {
        try {
          Connector.open(connectionString);
        } catch (IOException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
      }
    } catch (BluetoothStateException e) {
      e.printStackTrace();
    }
  }
}

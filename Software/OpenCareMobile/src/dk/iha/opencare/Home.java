package dk.iha.opencare;

import java.io.IOException;

import roboguice.activity.RoboActivity;
import android.bluetooth.BluetoothAdapter;
import android.os.Bundle;

public class Home extends RoboActivity {

  private AcceptThread mAcceptThread; 
  
  /** Called when the activity is first created. */
  @Override public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main);
    
    try {
      mAcceptThread = new AcceptThread(BluetoothAdapter.getDefaultAdapter());
      mAcceptThread.start();
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
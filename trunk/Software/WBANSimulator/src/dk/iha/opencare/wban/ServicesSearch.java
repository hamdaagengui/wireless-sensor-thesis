package dk.iha.opencare.wban;

import java.io.IOException;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import javax.bluetooth.BluetoothStateException;
import javax.bluetooth.DataElement;
import javax.bluetooth.DeviceClass;
import javax.bluetooth.DiscoveryAgent;
import javax.bluetooth.DiscoveryListener;
import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.ServiceRecord;
import javax.bluetooth.UUID;

/**
 * 
 * Minimal Services Search example.
 */
public class ServicesSearch {

  private static final UUID OBEX_SERIAL_PORT       = new UUID(0x1101);
  private static final int  ATTRIBUTE_SERVICE_NAME = 0x0100;

  public static ArrayList<String> scan() throws BluetoothStateException {

    final ArrayList<String> serviceFound = new ArrayList<String>();
    final CountDownLatch latch = new CountDownLatch(1);
    final LocalDevice localDevice = LocalDevice.getLocalDevice();
    final DiscoveryAgent discoveryAgent = localDevice.getDiscoveryAgent();

    final DiscoveryListener listener = new DiscoveryListener() {
      @Override public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod)
      {
        String friendlyName = "N/A";
        try {
          friendlyName = btDevice.getFriendlyName(false);
        } catch (IOException e) {
          e.printStackTrace();
        }

        System.out.format("Device discovered: %s %s\n", btDevice.getBluetoothAddress(), friendlyName);

      }

      @Override public void serviceSearchCompleted(int transID, int respCode) {
        System.out.format("Service search completed [id:%d]: %d\n", transID, respCode);
        latch.countDown();
      }

      @Override public void inquiryCompleted(int discType) {
        System.out.format("Inquiry completed: %d\n", discType);
        RemoteDevice[] devices = discoveryAgent.retrieveDevices(DiscoveryAgent.CACHED);
        if (devices == null) {
          System.out.println("No devices found!");
          return;
        }
        UUID[] uuids = new UUID[] { OBEX_SERIAL_PORT };
        for (RemoteDevice remoteDevice : devices) {
          try {
            discoveryAgent.searchServices(null, uuids, remoteDevice, this);
          } catch (BluetoothStateException e) {
            e.printStackTrace();
          }
        }
      }

      @Override public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
        System.out.format("Services discovered [id:%d]:\n", transID);
        for (int i = 0; i < servRecord.length; i++) {
          ServiceRecord serviceRecord = servRecord[i];
          DataElement serviceName = serviceRecord.getAttributeValue(ATTRIBUTE_SERVICE_NAME);
          String url = serviceRecord.getConnectionURL(ServiceRecord.AUTHENTICATE_ENCRYPT, false);
          if (url == null) {
            continue;
          }

          serviceFound.add(url);
          if (serviceName != null) {
            System.out.println("service " + serviceName.getValue() + " found " + url);
          } else {
            System.out.println("service found " + url);
          }
        }
      }
    };
    new Thread(new Runnable() {
      @Override public void run() {
        try {
          discoveryAgent.startInquiry(DiscoveryAgent.GIAC, listener);
        } catch (BluetoothStateException e) {
          e.printStackTrace();
        }
      }
    }).start();
    try {
      while (latch.await(100, TimeUnit.MILLISECONDS) == false) {}
    } catch (InterruptedException e) {
      e.printStackTrace();
    }
    System.out.println("Scan completed!");
    return serviceFound;
  }
}
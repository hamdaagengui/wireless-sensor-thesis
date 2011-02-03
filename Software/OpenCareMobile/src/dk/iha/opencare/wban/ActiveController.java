package dk.iha.opencare.wban;

import dk.iha.opencare.Connection;
import roboguice.util.Ln;

public class ActiveController implements Controller {

  @Override public void handleConnection(Connection c) {
    Ln.i("Got a new connection attempt!");
  }

}

package dk.iha.opencare;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface Connection {
  void connect() throws IOException;
  void close() throws IOException;
  InputStream getInputStream() throws IOException;
  OutputStream getOutputStream() throws IOException;
}
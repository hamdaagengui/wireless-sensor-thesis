package dk.iha.opencare;

import java.util.List;

import roboguice.application.RoboApplication;
import roboguice.config.AbstractAndroidModule;

import com.google.inject.Module;

import dk.iha.opencare.wban.ActiveController;
import dk.iha.opencare.wban.Controller;

public class Application extends RoboApplication {
  protected void addApplicationModules(List<Module> modules) {
    modules.add(new AbstractAndroidModule() {
      @Override protected void configure() {
        bind(Controller.class).to(ActiveController.class);
      }
    });
  }
}
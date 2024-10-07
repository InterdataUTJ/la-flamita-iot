import { SerialPort, ReadlineParser } from "serialport";

const laFlamita = {
  port: null,
  path: null,
  parser: null,
  
  callback: {
    name: null,
    action: null,
  },

  callbackSucces: {
    "START_CONFIG_MODE": "[CONFIGURATOR] Configuration mode started.",
    "STOP_CONFIG_MODE": "[CONFIGURATOR] Stoping config mode.",
    "CLEAR_CONFIG": "[CONFIGURATOR] Config cleared.",
    "APISERVER?": "[CONFIGURATOR] API_SERVER updated: ",
    "APIKEY?": "[CONFIGURATOR] API_KEY updated: ",
    "PASS?": "[CONFIGURATOR] WIFI_PASS updated: ",
    "SSID?": "[CONFIGURATOR] WIFI_SSID updated: ",
  },

  list: async () => {
    return SerialPort.list();
  },
  
  open: async (_, path) => {
    return new Promise((resolve, _) => {
      if (laFlamita.port) {
        laFlamita.port.close();
        laFlamita.port = null;
      }

      laFlamita.port = new SerialPort({ path, baudRate: 115200 });
      laFlamita.parser = laFlamita.port.pipe(new ReadlineParser({ delimiter: '\r\n' }));
      laFlamita.path = path;

      laFlamita.parser.on("data", (data) => {
        const response = data.toString();
        console.log(response);
        if (response.startsWith(laFlamita.callbackSucces[laFlamita.callback.name])) {
          if (laFlamita.callback.action) laFlamita.callback.action(true);
          laFlamita.callback.name = null;
          laFlamita.callback.action = null;
        }
      });

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        laFlamita.port?.close();
        laFlamita.port = null;
        laFlamita.path = null;
        laFlamita.parser = null;
        resolve(false);
      }, 10000);

      laFlamita.port.write("START_CONFIG_MODE");
      laFlamita.callback.name = "START_CONFIG_MODE";
      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      };
    });
  },

  close: async () => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) {
        laFlamita.path = null;
        resolve(false);
      }

      laFlamita.port.write("STOP_CONFIG_MODE");
      laFlamita.callback.name = "STOP_CONFIG_MODE";
      laFlamita.callback.action = () => {
        laFlamita.port.close();
        laFlamita.port = null;
        laFlamita.path = null;
        laFlamita.parser = null;
        resolve(true);
      };

      setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);
    });
  },

  setSSID: async (_, WIFI_SSID) => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) resolve(false);
      laFlamita.port.write(`SSID?${WIFI_SSID}`);
      laFlamita.callback.name = "SSID?";

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);

      
      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      };
    });
  },

  setPass: async (_, WIFI_PASS) => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) resolve(false);
      laFlamita.port.write(`PASS?${WIFI_PASS}`);
      laFlamita.callback.name = "PASS?";

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);

      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      }
    });
  },

  setApiKey: async (_, API_KEY) => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) resolve(false);
      laFlamita.port.write(`APIKEY?${API_KEY}`);
      laFlamita.callback.name = "APIKEY?";

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);

      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      }
    });
  },

  setApiServer: async (_, API_SERVER) => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) resolve(false);
      laFlamita.port.write(`APISERVER?${API_SERVER}`);
      laFlamita.callback.name = "APISERVER?";

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);

      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      }
    });
  },

  clear: async () => {
    return new Promise((resolve, _) => {
      if (!laFlamita.port) resolve(false);
      laFlamita.port.write("CLEAR_CONFIG");
      laFlamita.callback.name = "CLEAR_CONFIG";

      const timeout = setTimeout(() => {
        laFlamita.callback.action = null;
        laFlamita.callback.name = null;
        resolve(false);
      }, 10000);

      laFlamita.callback.action = (state) => {
        clearTimeout(timeout);
        resolve(state);
      }
    });
  }
}

export default laFlamita;
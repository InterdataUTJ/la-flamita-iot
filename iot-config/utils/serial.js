import { SerialPort } from "serialport";

const ports = {};

async function list() {
  return SerialPort.list();
}


function open(_, path) {
  return new Promise((resolve, reject) => {
    const port = new SerialPort({ path, baudRate: 115200 });
    ports[path] = port;
    let messageSended = false;
    let failTimeout;

    port.on("error", (err) => {
      close(null, path, true);
      clearTimeout(failTimeout);
      reject(err);
    });

    // Start the configuration mode
    port.on("open", () => {
      setTimeout(() => {
        if (!messageSended) {
          port.write("START_CONFIG_MODE");
          messageSended = true;
        }
      }, 1000);
    });

    failTimeout = setTimeout(() => {
      close(null, path, true);
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      console.log(`[${path}] ${data.toString()}`);
      const isCorrect = data.toString().includes("[CONFIGURATOR] Configuration mode started.");
      clearTimeout(failTimeout);
      resolve(isCorrect);
    });

  });
}

function close(_, path, instant = false) {
  return new Promise((resolve, reject) => {
    if (instant) {
      ports[path]?.close();
      ports[path] = undefined;
    }

    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) {
      ports[path] = undefined;
      reject(new Error("Port is not open"));
    }

    port.write("STOP_CONFIG_MODE");

    let timeout = setTimeout(() => {
      // if (port.isOpen) port.close();
      // ports[path] = undefined;
      reject(new Error("Timeout"));
    }, 10000);
    
    port.on("data", (data) => {
      clearTimeout(timeout);
      const state = data.toString().includes("[CONFIGURATOR] Stoping config mode.");
      
      if (state) {
        close(null, path, true);
        ports[path] = undefined;
      }

      resolve(state);
    });
    
  });
}

function setSSID(_, path, WIFI_SSID) {
  return new Promise((resolve, reject) => {
    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) reject(new Error("Port is not open"));
    if (!WIFI_SSID) reject(new Error("SSID is required"));
    if (WIFI_SSID.length > 50) reject(new Error("SSID is too long"));
    if (WIFI_SSID.length < 1) reject(new Error("SSID is too short"));

    port.write(`SSID?${WIFI_SSID}`);

    let timeout = setTimeout(() => {
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      clearTimeout(timeout);
      const isCorrect = data.toString().includes(`[CONFIGURATOR] WIFI_SSID updated: ${WIFI_SSID}`);
      resolve(isCorrect);
    });
  });
}


function setPass(_, path, WIFI_PASS) {
  return new Promise((resolve, reject) => {
    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) reject(new Error("Port is not open"));
    if (!WIFI_PASS) reject(new Error("WIFI_PASS is required"));
    if (WIFI_PASS.length > 50) reject(new Error("WIFI_PASS is too long"));
    if (WIFI_PASS.length < 1) reject(new Error("WIFI_PASS is too short"));

    port.write(`PASS?${WIFI_PASS}`);

    let timeout = setTimeout(() => {
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      clearTimeout(timeout);
      const isCorrect = data.toString().includes(`[CONFIGURATOR] WIFI_PASS updated: ${WIFI_PASS}`);
      resolve(isCorrect);
    });
  });
}


function setApiKey(_, path, API_KEY) {
  return new Promise((resolve, reject) => {
    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) reject(new Error("Port is not open"));
    if (!API_KEY) reject(new Error("API_KEY is required"));
    if (API_KEY.length > 50) reject(new Error("API_KEY is too long"));
    if (API_KEY.length < 1) reject(new Error("API_KEY is too short"));

    port.write(`APIKEY?${API_KEY}`);

    let timeout = setTimeout(() => {
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      clearTimeout(timeout);
      const isCorrect = data.toString().includes(`[CONFIGURATOR] API_KEY updated: ${API_KEY}`);
      resolve(isCorrect);
    });
  });
}


function setApiServer(_, path, API_SERVER ) {
  return new Promise((resolve, reject) => {
    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) reject(new Error("Port is not open"));
    if (!API_SERVER ) reject(new Error("API_SERVER  is required"));
    if (API_SERVER .length > 50) reject(new Error("API_SERVER  is too long"));
    if (API_SERVER .length < 1) reject(new Error("API_SERVER  is too short"));

    port.write(`APISERVER?${API_SERVER }`);

    let timeout = setTimeout(() => {
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      clearTimeout(timeout);
      const isCorrect = data.toString().includes(`[CONFIGURATOR] API_SERVER updated: ${API_SERVER}`);
      resolve(isCorrect);
    });
  });
}

function clear(_, path ) {
  return new Promise((resolve, reject) => {
    const port = ports[path];
    if (!port) reject(new Error("Port not found"));
    if (!port.isOpen) reject(new Error("Port is not open"));

    port.write(`CLEAR_CONFIG`);

    let timeout = setTimeout(() => {
      reject(new Error("Timeout"));
    }, 10000);

    port.on("data", (data) => {
      clearTimeout(timeout);
      const isCorrect = data.toString().includes(`[CONFIGURATOR] Config cleared.`);
      resolve(isCorrect);
    });
  });
}

export default {list, open, close, setSSID, setPass, setApiKey, setApiServer, clear};
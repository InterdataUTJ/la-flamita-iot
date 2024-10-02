const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("laFlamita", { 
  serial: { 
    list: () => ipcRenderer.invoke("serial:list"),
    open: (path) => ipcRenderer.invoke("serial:open", path),
    close: (path) => ipcRenderer.invoke("serial:close", path),
    setSSID: (path, ssid) => ipcRenderer.invoke("serial:setSSID", path, ssid),
    setPass: (path, pass) => ipcRenderer.invoke("serial:setPass", path, pass),
    setApiKey: (path, apiKey) => ipcRenderer.invoke("serial:setApiKey", path, apiKey),
    setApiServer: (path, apiServer) => ipcRenderer.invoke("serial:setApiServer", path, apiServer),
    clear: (path) => ipcRenderer.invoke("serial:clear", path),
  },

  wifi: {
    list: () => ipcRenderer.invoke("wifi:list"),
    check: () => ipcRenderer.invoke("wifi:check"),
  }, 
});
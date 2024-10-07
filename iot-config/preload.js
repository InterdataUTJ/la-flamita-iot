const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("laFlamita", { 
  serial: { 
    list: () => ipcRenderer.invoke("serial:list"),
    open: (path) => ipcRenderer.invoke("serial:open", path),
    close: () => ipcRenderer.invoke("serial:close"),
    setSSID: (ssid) => ipcRenderer.invoke("serial:setSSID", ssid),
    setPass: (pass) => ipcRenderer.invoke("serial:setPass", pass),
    setApiKey: (apiKey) => ipcRenderer.invoke("serial:setApiKey", apiKey),
    setApiServer: (apiServer) => ipcRenderer.invoke("serial:setApiServer", apiServer),
    clear: () => ipcRenderer.invoke("serial:clear"),
  },

  wifi: {
    list: () => ipcRenderer.invoke("wifi:list"),
    check: () => ipcRenderer.invoke("wifi:check"),
  }, 
});
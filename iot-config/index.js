import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import { __dirname } from './utils/dirname.js';

import serial from './utils/serial.js';
import wifi from './utils/wifi.js';

const createWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
    },
  });

  win.loadFile("public/index.html");
}

app.whenReady().then(() => {
  ipcMain.handle('serial:list', serial.list);
  ipcMain.handle('serial:open', serial.open);
  ipcMain.handle('serial:close', serial.close);
  ipcMain.handle('serial:setSSID', serial.setSSID);
  ipcMain.handle('serial:setPass', serial.setPass);
  ipcMain.handle('serial:setApiKey', serial.setApiKey);
  ipcMain.handle('serial:setApiServer', serial.setApiServer);
  ipcMain.handle('serial:clear', serial.clear);

  ipcMain.handle('wifi:list', wifi.list);
  ipcMain.handle('wifi:check', wifi.check);
  createWindow();
});
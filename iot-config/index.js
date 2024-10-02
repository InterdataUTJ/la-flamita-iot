import { app, BrowserWindow, ipcMain } from 'electron';
import path from 'path';
import { __dirname } from './utils/dirname.js';

import { list, open, close, setSSID, setPass, setApiKey, setApiServer, clear } from './utils/serial.js';

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
  ipcMain.handle('serial:list', list);
  ipcMain.handle('serial:open', open);
  ipcMain.handle('serial:close', close);
  ipcMain.handle('serial:setSSID', setSSID);
  ipcMain.handle('serial:setPass', setPass);
  ipcMain.handle('serial:setApiKey', setApiKey);
  ipcMain.handle('serial:setApiServer', setApiServer);
  ipcMain.handle('serial:clear', clear);
  createWindow();
});
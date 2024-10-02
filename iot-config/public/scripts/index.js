const loader = document.querySelector('.loader');
const deviceContainer = document.getElementById('devices-list');
const estado = {
  puertos: [],
}

async function refresh() {
  loader.style.display = 'block';
  
  // Remove spinner
  setTimeout(() => {
    loader.style.display = 'none';
    refreshPorts();
  }, 1000);
}


async function refreshPorts() {
  const ports = await window?.laFlamita?.serial?.list() || [];
  if (JSON.stringify(estado.puertos) === JSON.stringify(ports)) return;
  console.log("[SYSTEM] Refreshing ports...");
  estado.puertos = ports;
  deviceContainer.innerHTML = '';

  estado.puertos.forEach((port) => {
    const deviceItem = document.createElement('device-item');
    deviceItem.setAttribute('name', port.friendlyName);
    deviceItem.setAttribute('path', port.path);
    deviceContainer.appendChild(deviceItem);
  });
}

// Auto refresh ports every 2 seconds
setInterval(async () => {
  await refreshPorts();
}, 2000);
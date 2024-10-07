async function goBack() {
  // Set the cursor to loading
  cursorLoading(true);
  
  // Close the port
  await window.laFlamita.serial.close().catch(() => {});
  
  // Redirect to index.html
  cursorLoading(false);
  window.location.replace('index.html');
}

document.querySelector("form").addEventListener("submit", async event => {
  event.preventDefault();
  console.log(event.target);
  const formData = new FormData(event.target);
  const data = {};
  formData.forEach((value, key) => {
    if (!value) return;
    if (value === "") return;
    data[key] = value;
  });

  console.log(data); 

  if (data.ssid === "" || data.ssid.lenght < 1 || data.ssid === undefined || data.ssid.length > 50) return alert("El nombre de la red wifi no es válido");
  if (data.pass === "" || data.pass.lenght < 1 || data.pass === undefined || data.pass.length > 50) return alert("La contraseña no es válida");
  if (data.APIKEY === "" || data.APIKEY.lenght < 1 || data.APIKEY === undefined || data.APIKEY.length > 50) return alert("La clave de la API no es válida");
  if (data.APISERVER === "" || data.APISERVER.lenght < 1 || data.APISERVER === undefined || data.APISERVER.length > 50) return alert("La dirección del servidor de la API no es válida");

  // Set the cursor to loading
  cursorLoading(true);
  let success = true;

  // 
  success = await window.laFlamita.serial.setSSID(data.ssid).catch(err => false);
  if (!success) {
    alert("No se pudo establecer la red wifi");
    cursorLoading(false);
    return;
  }
  
  succes = await window.laFlamita.serial.setPass(data.pass).catch(err => false);
  if (!success) {
    alert("No se pudo establecer la red wifi");
    cursorLoading(false);
    return;
  }
  
  succes =  await window.laFlamita.serial.setApiKey(data.APIKEY).catch(err => false);
  if (!success) {
    alert("No se pudo establecer la clave de la API");
    cursorLoading(false);
    return;
  }
  
  succes = await window.laFlamita.serial.setApiServer(data.APISERVER).catch(err => false);
  if (!success) {
    alert("No se pudo establecer la dirección del servidor de la API");
    cursorLoading(false);
    return;
  }
  
    cursorLoading(false);
    alert("Configuración guardada");
    await window.laFlamita.serial.close().catch(err => { console.error(err) });
    window.location.replace('index.html');
});

document.querySelectorAll(".pass-toggle").forEach(elemento => {
  elemento.addEventListener("click", () => {
    const input = elemento.previousElementSibling;
    input.type = input.type === "password" ? "text" : "password";
    elemento.textContent = input.type === "password" ? "Mostrar" : "Ocultar";
    input.focus();
  });
});

document.getElementById("useSSID").addEventListener("change", () => {
  document.getElementById("wifi-list").style.display = document.getElementById("useSSID").checked ? "block" : "none";
  document.getElementById("ssid-input").style.display = !document.getElementById("useSSID").checked ? "flex" : "none";
  document.getElementById("ssid-input").disabled = document.getElementById("useSSID").checked;

});

window.laFlamita.wifi.list().then(networks => {
  networks.forEach(network => {
    const item = document.createElement('wifi-item');
    item.setAttribute('name', network.ssid);
    item.setAttribute('signal', network.quality);
    document.getElementById("wifi-list").appendChild(item);
  });
}).catch(err => {
  console.error(err);
  document.getElementById("wifi-list").textContent = "No se pudo obtener la lista de redes wifi";
});
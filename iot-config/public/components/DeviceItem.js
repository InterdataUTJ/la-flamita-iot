class DeviceItem extends HTMLElement {
    constructor() {
      super();
    }

    connectedCallback() {
      this.name = this.getAttribute('name') || 'Unknown serial device';
      this.path = this.getAttribute('path');
      this.classList.add('device');

      const name = document.createElement('span');
      name.classList.add('device-name');
      name.textContent = this.name;

      const loader = document.createElement('span');
      loader.classList.add('loader');

      const connectButton = document.createElement('button');
      connectButton.classList.add('device-connect');
      connectButton.innerHTML = '<svg  xmlns="http://www.w3.org/2000/svg"  width="24"  height="24"  viewBox="0 0 24 24"  fill="none"  stroke="currentColor"  stroke-width="2"  stroke-linecap="round"  stroke-linejoin="round"  class="icon icon-tabler icons-tabler-outline icon-tabler-plug-connected"><path stroke="none" d="M0 0h24v24H0z" fill="none"/><path d="M7 12l5 5l-1.5 1.5a3.536 3.536 0 1 1 -5 -5l1.5 -1.5z" /><path d="M17 12l-5 -5l1.5 -1.5a3.536 3.536 0 1 1 5 5l-1.5 1.5z" /><path d="M3 21l2.5 -2.5" /><path d="M18.5 5.5l2.5 -2.5" /><path d="M10 11l-2 2" /><path d="M13 14l-2 2" /></svg>';
      connectButton.innerHTML += ' Conectar';
      connectButton.addEventListener('click', async () => {
        if (this.hasAttribute("disabled")) return;
        cursorLoading(true);
        this.setAttribute('loading', '');
        document.querySelectorAll('device-item.device').forEach((device) => {
          device.setAttribute('disabled', '');
        });

        await window.laFlamita.serial.open(this.path)
          .then(isAvailable => {
            if (isAvailable) window.location.replace('device.html');
            else alert('El dispositivo no es compatible');
          })
          .catch(err => {
            alert('El dispositivo no es compatible');
            console.error(err);
          });
        
        cursorLoading(false);
        this.removeAttribute('loading');
        document.querySelectorAll('device-item.device').forEach((device) => {
          device.removeAttribute('disabled');
        });
      });

      this.appendChild(name);
      this.appendChild(loader);
      this.appendChild(connectButton);
    }
}

customElements.define('device-item', DeviceItem);
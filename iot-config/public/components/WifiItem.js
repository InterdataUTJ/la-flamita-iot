class WifiItem extends HTMLElement {
  constructor() {
    super();
  }

  connectedCallback() {
    this.name = this.getAttribute('name') || 'Unknown wifi network';
    this.signal = this.getAttribute('signal') || '50';
    this.classList.add('wifi-item');

    const input = document.createElement('input');
    input.setAttribute('type', 'radio');
    input.setAttribute('name', 'ssid');
    input.setAttribute('id', this.name);
    input.setAttribute('value', this.name);
    input.setAttribute('class', 'wifi-item-input');
    this.appendChild(input);

    const label = document.createElement('label');
    label.setAttribute('for', this.name);
    label.setAttribute('class', 'wifi-item-label');
    label.innerHTML = this.getIcon();

    const name = document.createElement('span');
    name.textContent = this.name;
    label.appendChild(name);
    this.appendChild(label);
  }

  getIcon() {
    if (this.signal >= 75) return `<svg  xmlns="http://www.w3.org/2000/svg"  width="24"  height="24"  viewBox="0 0 24 24"  fill="none"  stroke="currentColor"  stroke-width="2"  stroke-linecap="round"  stroke-linejoin="round"  class="icon icon-tabler icons-tabler-outline icon-tabler-wifi"><path stroke="none" d="M0 0h24v24H0z" fill="none"/><path d="M12 18l.01 0" /><path d="M9.172 15.172a4 4 0 0 1 5.656 0" /><path d="M6.343 12.343a8 8 0 0 1 11.314 0" /><path d="M3.515 9.515c4.686 -4.687 12.284 -4.687 17 0" /></svg>`;
    if (this.signal >= 50) return `<svg  xmlns="http://www.w3.org/2000/svg"  width="24"  height="24"  viewBox="0 0 24 24"  fill="none"  stroke="currentColor"  stroke-width="2"  stroke-linecap="round"  stroke-linejoin="round"  class="icon icon-tabler icons-tabler-outline icon-tabler-wifi-2"><path stroke="none" d="M0 0h24v24H0z" fill="none"/><path d="M12 18l.01 0" /><path d="M9.172 15.172a4 4 0 0 1 5.656 0" /><path d="M6.343 12.343a8 8 0 0 1 11.314 0" /></svg>`;
    if (this.signal >= 25) return `<svg  xmlns="http://www.w3.org/2000/svg"  width="24"  height="24"  viewBox="0 0 24 24"  fill="none"  stroke="currentColor"  stroke-width="2"  stroke-linecap="round"  stroke-linejoin="round"  class="icon icon-tabler icons-tabler-outline icon-tabler-wifi-1"><path stroke="none" d="M0 0h24v24H0z" fill="none"/><path d="M12 18l.01 0" /><path d="M9.172 15.172a4 4 0 0 1 5.656 0" /></svg>`;
    return `<svg  xmlns="http://www.w3.org/2000/svg"  width="24"  height="24"  viewBox="0 0 24 24"  fill="none"  stroke="currentColor"  stroke-width="2"  stroke-linecap="round"  stroke-linejoin="round"  class="icon icon-tabler icons-tabler-outline icon-tabler-wifi-0"><path stroke="none" d="M0 0h24v24H0z" fill="none"/><path d="M12 18l.01 0" /></svg>`;
  }
}

customElements.define('wifi-item', WifiItem);
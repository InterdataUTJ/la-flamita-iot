import wifi from "node-wifi";

function list() {
  return new Promise((resolve, reject) => {
    wifi.init({
      iface: null
    });
    
    wifi.scan((err, networks) => {
      if (err) return reject(err);
      networks.sort((a, b) => b.quality - a.quality);
      resolve(networks);
    });
  });
}

export default { list };
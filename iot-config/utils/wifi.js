import { scan } from "node-wifi-scanner";

function check() {
  return new Promise((resolve, _) => {
    scan((err, _) => {
      if (err) resolve(false);
      else resolve(true);
    });
  });
}

function list() {
  return new Promise((resolve, _) => {
    scan((err, networks) => {
      if (err) resolve([]);
      else resolve(networks);
    });
  });
}

export default { list, check };
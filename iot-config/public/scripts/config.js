async function goBack() {
  // Set the cursor to loading
  cursorLoading(true);
  
  // Close the port
  const port = decodeURIComponent(new URLSearchParams(window.location.search).get('path'));
  await window.laFlamita.serial.close(port).catch(() => {});
  
  // Redirect to index.html
  cursorLoading(false);
  window.location.replace('index.html');
}
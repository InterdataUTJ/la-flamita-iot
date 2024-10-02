function cursorLoading(isLoading) {
  const normal = isLoading ? 'wait' : 'default';
  const pointer = isLoading ? 'wait' : 'pointer';

  document.body.style.cursor = normal;
  if (document.querySelector('.back')) {
    document.querySelector('.back').style.cursor = pointer;
  }

  document.querySelectorAll('.device-connect')?.forEach((button) => {
    button.style.cursor = pointer;
  });
}
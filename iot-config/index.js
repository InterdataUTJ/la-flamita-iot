const express = require('express')
const app = express()
const port = 5500

app.use(express.json());

app.get('/', (req, res) => {
  console.log(req.url);
  res.send('OK');
})

app.post('/', (req, res) => {
  console.log(req.body);
  res.status(201).send('OK');
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
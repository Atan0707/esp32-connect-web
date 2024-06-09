const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const fs = require('fs').promises; // Use fs.promises for async file operations

const app = express();
const port = 3000;

app.use(cors());
app.use(bodyParser.json());

app.post('/postData', async (req, res) => {
    const { value } = req.body;
    console.log('Received value:', value);

    // Define the path and content for the JSON file
    const filePath = './data.json';
    const content = JSON.stringify({ value }, null, 2); // Pretty print the JSON

    try {
        // Write the content to the JSON file
        await fs.writeFile(filePath, content, 'utf8');
        res.status(200).send({ message: 'Data received and saved successfully' });
    } catch (error) {
        console.error('Error writing to the file:', error);
        res.status(500).send({ message: 'Failed to save data' });
    }
});

app.get('/getData', async (req, res) => {
    const filePath = './data.json';
    try {
        const data = await fs.readFile(filePath, 'utf8');
        res.status(200).send(JSON.parse(data));
    } catch (error) {
        console.error('Error reading the file:', error);
        res.status(500).send({ message: 'Failed to read data' });
    }
});

app.listen(port, () => {
    console.log(`Server is running on http://192.168.1.2:${port}`);
});
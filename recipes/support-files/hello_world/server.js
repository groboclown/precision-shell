// Runs an HTTP server that responds with Hello World.

var http = require('http');

// Load the configuration using the simplest reader.
var config = require('./config.json');

// Configure our HTTP server to respond with Hello World to all requests.
var server = http.createServer(function (request, response) {
  response.writeHead(200, {"Content-Type": "text/plain"});
  response.end("Hello World\n");
});

// Listen on port 8000, IP defaults to 127.0.0.1
var listen_port = config.listen || 8000;
server.listen(listen_port);

// Put a friendly message on the terminal
console.log(`Server running at http://127.0.0.1:${listen_port}/`);

// Runs an HTTP server that responds with Hello World.

const http = require('http');
const process = require('process');
const os = require('os');

// Load the configuration using the simplest reader.
var config = require('./config.json');

// Configure our HTTP server to respond with Hello World to all requests.
var server = http.createServer(function (request, response) {
  if (request.url == '/halt') {
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.end("Goodbye World\n");
    process.kill(process.pid, 'SIGTERM');
    return;
  }
  if (request.url == '/pid') {
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.end("" + process.pid + "\n");
    return;
  }
  if (request.url == '/uid') {
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.end("" + os.userInfo().uid + "\n");
    return;
  }
  if (request.url == '/gid') {
    response.writeHead(200, {"Content-Type": "text/plain"});
    response.end("" + os.userInfo().gid + "\n");
    return;
  }
  response.writeHead(200, {"Content-Type": "text/plain"});
  response.end("Hello World\n");
});

process.on('SIGTERM', () => {
  server.close(() => {
    console.log('Server terminated');
  });
});

// Listen on port 8000, IP defaults to 127.0.0.1
var listen_port = config.listen || 8000;
server.listen(listen_port);

// Put a friendly message on the terminal
console.log(`Server running at http://127.0.0.1:${listen_port}/`);

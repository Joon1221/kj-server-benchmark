const express = require('../../api/node_modules/express');
const bodyParser = require('../../api/node_modules/body-parser');
const queue = require('../../api/node_modules/express-queue');

const https = require('https');
const fs = require('fs');

const port = 7443;

// intialize pemkey and certificate
var key = fs.readFileSync('pemkey.key');
var cert = fs.readFileSync('certificate.crt');
var options = {
key: key,
cert: cert
};


app = express()
app.use(express.bodyParser());
app.use(queue({ activeLimit: 10000, queuedLimit: 10000 }));

//------------------------------------------------------------------------------
// Handle Get Request
//------------------------------------------------------------------------------
app.get('/', (req, res) => {
        res.send('hello\n');
        });

//------------------------------------------------------------------------------
// Handle Post Request
//------------------------------------------------------------------------------
app.post('/', (req, res) => {
         var id = req.body.id;
         //---------------------------------------------------------------------
         // Process Data
         //---------------------------------------------------------------------
         
         // process data according to request type
         if (id == 0) { // Video Request
                res.send('Video Request\n');
         }
         else if (id == 1) { // Chat Request
         //---------------------------------------------------------------------
         // chatPacket
         //---------------------------------------------------------------------
         // custom made protocol based off websocket
         // uint8_t       header;
         // uint8_t       body_length[3]; /* lenght of the body */
         // uint8_t       chat_id[3]; /* reserved, must be "\0\0\0" */
         // uint8_t       body[192]; /* data body */
         //---------------------------------------------------------------------

                var value = Math.floor(Math.random() * Math.floor(192));
         
                // open sample text file
                fs.open('../shared/chat_message.txt', 'r', function(status, fd) {
                        if (status) {
                            console.log(status.message);
                            return;
                        }
                        var buffer = Buffer.alloc(value);
                        // send data as json string
                        fs.read(fd, buffer, 0, value, 0, function(err, num) {
                            var json = "{\"header\":0, \"bodyLength\":" + value + ", \"chatId\":0, \"body\":\"" + buffer.toString('utf8', 0, num) + "\"}";
                                res.send(json);
//                            console.log(buffer.toString('utf8', 0, num));
                        });
                });
         }
         });

var server = https.createServer(options, app);

server.listen(port, () => {
              console.log("server starting on port : " + port)
              });

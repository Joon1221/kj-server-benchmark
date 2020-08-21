// Import required java libraries
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import com.google.gson.*;
import java.util.Random;

// Extend HttpServlet class
public class server extends HttpServlet {
    
    private String message;
    
    
    //--------------------------------------------------------------------------
    // Intialize Server
    //--------------------------------------------------------------------------
    public void init() throws ServletException {
        // Do required initialization
        message = "Hello World";
    }
    
    //--------------------------------------------------------------------------
    // Handle Get Request
    //--------------------------------------------------------------------------
    public void doGet(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
        
        // Set response content type
        response.setContentType("text/html");
        
        // Output message
        PrintWriter out = response.getWriter();
        out.println("<h1>" + message + "</h1>");
    }
    
    //--------------------------------------------------------------------------
    // Handle Post Request
    //--------------------------------------------------------------------------
    public void doPost(HttpServletRequest request, HttpServletResponse response)
    throws ServletException, IOException {
    
        // Set response content type
        response.setContentType("text/html");
        
        //----------------------------------------------------------------------
        // Read Data from Client
        //----------------------------------------------------------------------

        // read and store message from client
        StringBuilder buffer = new StringBuilder();
        BufferedReader reader = request.getReader();
        String line;
        while ((line = reader.readLine()) != null) {
            buffer.append(line);
        }
        String data = buffer.toString();

        // parse the json message
        JsonParser parser = new JsonParser();
        JsonElement jsonElement = parser.parse(data);
        JsonObject rootObject = jsonElement.getAsJsonObject();
        String message = rootObject.get("id").getAsString();
        
        PrintWriter out = response.getWriter();

        //----------------------------------------------------------------------
        // Process Data
        //----------------------------------------------------------------------

        // process data according to request type
        if (message.equals("0")) { // Video Request
            //------------------------------------------------------------------
            // videoStreamingPacket
            //------------------------------------------------------------------
            // custom made protocol
            // uint8_t       type;
            // uint8_t       body_length[3]; /* lenght of the body */
            // uint8_t       stream_id[3]; /* reserved, must be "\0\0\0" */
            // uint8_t       body[192]; /* data body */
            //------------------------------------------------------------------
            
            // Open sample video file
            byte []chatMessage = new byte[192];
            InputStream is = new FileInputStream("../shared/1280x720.mp4");
            
            int readBytes = 0;
            
            // Main loop
            while ((readBytes  = is.read(chatMessage)) != -1) {
                // Send data as json string
                String json = "{\"header\":0, \"bodyLength\":" + readBytes + ", \"chatId\":0, \"body\":\"" + new String(chatMessage) + "\"}";
                out.println(json);
            }
            is.close();
        }
        else if (message.equals("1")) { // Chat Request
            //------------------------------------------------------------------
            // chatPacket
            //------------------------------------------------------------------
            // custom made protocol based off websocket
            // uint8_t       header;
            // uint8_t       body_length[3]; /* lenght of the body */
            // uint8_t       chat_id[3]; /* reserved, must be "\0\0\0" */
            // uint8_t       body[192]; /* data body */
            //------------------------------------------------------------------


            
            // Open sample text file
            byte []chatMessage = new byte[value];
            InputStream is = new FileInputStream("../shared/chat_message.txt");
            
            int readBytes = 0;
            
            is.read(chatMessage);
            is.close();
            
            Random rand = new Random();
            int value = rand.nextInt(192);
            
            // Send data as json string
            String json = "{\"header\":0, \"bodyLength\":" + value + ", \"chatId\":0, \"body\":\"" + new String(chatMessage) + "\"}";

            out.println(json);
        }
    }
    
    public void destroy() {
        // do nothing.
    }
}

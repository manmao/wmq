
package demo;

import org.wmq.client.NetConnection;
import org.wmq.client.WmqTemplate;


public class PublishMessageClient {
	
    public static String HOST = "192.168.21.207";  
    public static int PORT = 9090;  
    
    
    public static void main(String[] args) throws Exception {  
    	
    	WmqTemplate wmqTemplate=new WmqTemplate(new NetConnection(HOST, PORT));
    	for(int i=0;i<1000;i++){
    		wmqTemplate.pulishMessage("manmao", "39428759832498345");
    	}
    	System.exit(0);
    }
    
    
}
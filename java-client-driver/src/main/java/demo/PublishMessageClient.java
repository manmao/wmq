
package demo;

import org.wmq.client.NetConnection;
import org.wmq.client.WmqTemplate;


public class PublishMessageClient {
	
    public static String HOST = "192.168.21.207";  
    public static int PORT = 9090;  
    
    
    public static void main(String[] args) throws Exception {  
    
    	WmqTemplate wmqTemplate=new WmqTemplate(new NetConnection(HOST, PORT));
    	for(int i=0;i<10000;i++){
    		wmqTemplate.pulishMessage("manmao888", "其实是挺简单的：每个数据包定义成 ”包大小+包内容“，比如4个字节表示包的大小，然后是包数据。发送的时候，“包大小+包内容”组合到一起发送，接收的时候，先接收固定的4个字节，获取到包的size，然后再接收size字节的数据，这样一个包就接收完成。大致伪代码如下：");
    	}
    	
    }
    
    
}
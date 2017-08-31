package org.wmq.client;

import org.wmq.message.SocketData;
import org.wmq.message.SocketDataDecoder;
import org.wmq.message.SocketDataEncoder;
import org.wmq.vo.Constants;

import io.netty.bootstrap.Bootstrap;
import io.netty.buffer.ByteBuf;
import io.netty.buffer.Unpooled;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;


public class TcpClient {
	
    public static String HOST = "192.168.21.207";  
    public static int PORT = 9090;  
    
    public static Bootstrap bootstrap = getBootstrap();  
    public static Channel channel = getChannel(HOST,PORT);  
	
    
	/**
	 *  
	 *  初始化Bootstrap 
     *  @return 
     *
     */  
    public static final Bootstrap getBootstrap(){  
        EventLoopGroup group = new NioEventLoopGroup();  
        Bootstrap b = new Bootstrap();  
        b.group(group).channel(NioSocketChannel.class);  
        b.handler(new ChannelInitializer<Channel>() {  
            @Override  
            protected void initChannel(Channel ch) throws Exception {  
                ChannelPipeline pipeline = ch.pipeline();  
				// 设置特殊分隔符  
                ByteBuf buf = Unpooled.copiedBuffer(Constants.delimiter.getBytes());  
                pipeline.addLast(new DelimiterBasedFrameDecoder(1024*1024*10, buf));  
                pipeline.addLast("decoder", new SocketDataDecoder());  
                pipeline.addLast("encoder", new SocketDataEncoder());  
                pipeline.addLast("handler", new TcpClientHandler());
            }  
        });  
        b.option(ChannelOption.SO_KEEPALIVE, true); 
        b.option(ChannelOption.TCP_NODELAY, true);
        return b;  
    }  
    

    
    public static final Channel getChannel(String host,int port){  
        Channel channel = null;  
        try {  
            channel = bootstrap.connect(host, port).sync().channel();  
        } catch (Exception e) {  
            System.out.printf(String.format("连接Server(IP[%s],PORT[%s])失败", host,port),e);  
            return null;  
        }  
        return channel;  
    }  
  
    
    
    public static void sendMsg(Object msg) throws Exception {  
        if(channel!=null){  
            channel.writeAndFlush(msg).sync();  
        }else{  
            System.out.println("消息发送失败,连接尚未建立!");  
        }  
    } 
    
    
    
    public static void main(String[] args) throws Exception {  
    	
        try {  
        	String topicString="manmao1";
        	TcpClient.sendMsg(createSubTopicPkg(topicString,"windows"));
            long t0 = System.nanoTime();  
            
            for (int i = 0; i < 100; i++) {
            	Thread.sleep(10);
  
            	String msgString=topicString+"\t可";

            	TcpClient.sendMsg(creatMsgPkg(msgString,topicString,"windows"));
            }  
            long t1 = System.nanoTime();  
            System.out.println((t1-t0)/1000000.0);  
            System.exit(0);
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
    
    
    public static SocketData creatMsgPkg(String message,String topic,String from){
    	SocketData data=new SocketData();
    	data.setVersion(1);
    	data.setCode(0x0004);
    	data.setFd(0);
    	data.setCmd(0x0004);
    	data.setDataLen(message.getBytes().length);
    	data.setChecksum(128);
    	data.setTopic(topic.getBytes());
    	data.setFrom(from.getBytes());
    	data.setMsg(message.getBytes());
    	return data;
    }
    
    public static SocketData createSubTopicPkg(String topic,String from){
    	SocketData data=new SocketData();
    	data.setVersion(1);
    	data.setCode(0x0004); //编码方式
    	data.setFd(0);
    	data.setCmd(0x0003); //注册topic
    	data.setDataLen(0);
    	data.setChecksum(128);
    	data.setTopic(topic.getBytes());
    	data.setFrom(from.getBytes());
    	data.setMsg("".getBytes());
    	return data;
    }
}

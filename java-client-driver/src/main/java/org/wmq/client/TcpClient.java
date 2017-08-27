package org.wmq.client;

import org.wmq.message.SocketData;
import org.wmq.message.SocketDataDecoder;
import org.wmq.message.SocketDataEncoder;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;


public class TcpClient {
	
	
    public static String HOST = "192.16.137.53";  
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
                //pipeline.addLast("frameDecoder", new LengthFieldBasedFrameDecoder(Integer.MAX_VALUE, 0, 4, 0, 4));  
                //pipeline.addLast("frameEncoder", new LengthFieldPrepender(4));  
				   // 设置特殊分隔符  
                //ByteBuf buf = Unpooled.copiedBuffer("$_".getBytes());  
                //pipeline.addLast(new DelimiterBasedFrameDecoder(1024*100, buf));  
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
    
    
    
    public static void subTopic(Object msg) throws InterruptedException{
    	  if(channel!=null){  
              channel.writeAndFlush(msg).sync();  
          }else{  
              System.out.println("消息发送失败,连接尚未建立!");  
          }  
    }
    
    
    
    public static void main(String[] args) throws Exception {  
    	
        try {  
            long t0 = System.nanoTime();  
          /*  TcpClient.sendMsg(createSubTopicPkg("manmao123"));*/
            for (int i = 0; i < 10; i++) {
            	Thread.sleep(10);
            	String msgString="manmao last time test";
            	//String msgString="可以按需创建集群，即离线作业运行结束就可以释放集群。可以动态的在需要的时候增加节点。 也可以选择包年包月长期运行";

            	TcpClient.sendMsg(creatMsgPkg("windows",msgString,"manmao123"));
            }  
            long t1 = System.nanoTime();  
            System.out.println((t1-t0)/1000000.0);  
            System.exit(0);
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
    
    
    public static SocketData creatMsgPkg(String from,String message,String topic){
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

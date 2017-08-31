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

public class WmqMessageTopic {
	
	private NetConnection netConnection;
	
	private  Channel channel ;
	
	private String topic;
	
	private WmqMessageListener listener; 
	
	public WmqMessageTopic(NetConnection netConnection,WmqMessageListener listener){
		this.netConnection=netConnection;
		channel = getChannel(this.netConnection.getIp(),this.netConnection.getPort());  
		subTopic("client",topic);
	}
	
	private void subTopic(String fromUser,String topic) {
		try {
			sendMsg(createSubTopicPkg(fromUser,topic));
		} catch (Exception e) {
			System.out.println("注册topic失败！！");
			e.printStackTrace();
		}
	}
	
	private  void sendMsg(Object msg) throws Exception {
		if (this.channel!= null) {
			this.channel.writeAndFlush(msg).sync();
		} else {
			System.out.println("消息发送失败,连接尚未建立!");
		}
	}
	
	private  final Channel getChannel(String host,int port){  
	       Channel channel = null;  
	       try {  
	           channel = getBootstrap().connect(host, port).sync().channel();  
	       } catch (Exception e) {  
	           System.out.printf(String.format("连接Server(IP[%s],PORT[%s])失败", host,port),e);  
	           return null;  
	       }  

	       return channel;  
	}
	
	
   private  Bootstrap getBootstrap(){  
       EventLoopGroup group = new NioEventLoopGroup();  
       Bootstrap b = new Bootstrap();  
       b.group(group).channel(NioSocketChannel.class);  
       b.handler(new ChannelInitializer<Channel>() {  
           @Override  
           protected void initChannel(Channel ch) throws Exception {  
               ChannelPipeline pipeline = ch.pipeline();  
			   //设置特殊分隔符  
               ByteBuf buf = Unpooled.copiedBuffer(Constants.delimiter.getBytes());  
               pipeline.addLast(new DelimiterBasedFrameDecoder(1024*1024*10, buf));  
               pipeline.addLast("decoder", new SocketDataDecoder());  
               pipeline.addLast("encoder", new SocketDataEncoder());  
               pipeline.addLast("handler", listener);
           }  
       });  
       b.option(ChannelOption.SO_KEEPALIVE, true); 
       b.option(ChannelOption.TCP_NODELAY, true);
       return b;  
   }
   
   private static SocketData createSubTopicPkg(String fromUser,String topic){
   		SocketData data=new SocketData();
   		data.setVersion(1);
   		data.setCode(0x0004); //编码方式
   		data.setFd(0);
   		data.setCmd(0x0003); //注册topic
   		data.setDataLen(0);
   		data.setChecksum(0);
   		data.setTopic(topic.getBytes());
   		data.setFrom(fromUser.getBytes());
   		data.setMsg("".getBytes());
   		return data;
   }
	
   
}

package org.wmq.client;

import org.wmq.message.SocketDataDecoder;
import org.wmq.message.SocketDataEncoder;
import org.wmq.vo.Constants;

import io.netty.bootstrap.Bootstrap;
import io.netty.channel.Channel;
import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelOption;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioSocketChannel;



public class WmqTemplate {
	
	private NetConnection netConnection;
	
	private  Channel channel ;
	
	public  WmqTemplate(NetConnection netConnection){
		this.netConnection=netConnection;
		channel = getChannel(this.netConnection.getIp(),this.netConnection.getPort());  
	}
	
	
	public  void sendMsg(Object msg) throws Exception {
		if (this.channel!= null) {
			this.channel.writeAndFlush(msg).sync();
		} else {
			System.out.println("消息发送失败,连接尚未建立!");
		}
	}
	
	public  final Channel getChannel(String host,int port){  
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
	               pipeline.addLast("encoder", new SocketDataEncoder());
	           }  
	       });  
	       b.option(ChannelOption.SO_KEEPALIVE, true); 
	       b.option(ChannelOption.TCP_NODELAY, true);
	       return b;  
	   }  

}

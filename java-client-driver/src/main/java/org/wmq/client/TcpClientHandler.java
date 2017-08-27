package org.wmq.client;
  
import io.netty.channel.ChannelHandlerContext;  
import io.netty.channel.SimpleChannelInboundHandler;  
  
public class TcpClientHandler extends SimpleChannelInboundHandler<Object> {  
  
  
    @Override  
    protected void channelRead0(ChannelHandlerContext ctx, Object msg)  
            throws Exception {  
        //messageReceived方法,名称很别扭，像是一个内部方法.  
        System.out.println("client接收到服务器返回的消息:"+msg);  
          
    }  
  
}  
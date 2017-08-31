package org.wmq.client;

import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;

public abstract class WmqMessageListener extends SimpleChannelInboundHandler<Object>{

	
	public abstract void onMessage(Object msg);
	
	@Override
	protected void channelRead0(ChannelHandlerContext ctx, Object msg)
			throws Exception {
		onMessage(msg);
		
	}
	
}

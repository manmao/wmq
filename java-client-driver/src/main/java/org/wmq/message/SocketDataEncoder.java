package org.wmq.message;


import java.nio.ByteOrder;

import struct.JavaStruct;
import io.netty.buffer.ByteBuf;
import io.netty.channel.ChannelHandlerContext;
import io.netty.handler.codec.MessageToByteEncoder;

public class SocketDataEncoder extends MessageToByteEncoder<SocketData> {
	
    @Override
    protected void encode(ChannelHandlerContext ctx, SocketData data, ByteBuf out) throws Exception {
    	
    	byte[] output=JavaStruct.pack(data,ByteOrder.LITTLE_ENDIAN);
    	out.writeBytes(output);
    	
        ctx.flush();
    }
    
}
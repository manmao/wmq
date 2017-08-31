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
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*private byte[] covert(SocketData data){
    	byte[] datas=new byte[216+data.getDataLen()];//+
    	
    	int index=0;
    	
    	for(int i=0; i<4;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getVersion())[i];
    	}
    	
    	for(int i=0; i<4;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getCode())[i];
    	}
  
    	for(int i=0; i<4;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getCmd())[i];
    	}
    	
    	for(int i=0; i<4 ;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getFd())[i];
    	}
    	
    	for(int i=0; i<4;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getDataLen())[i];
    	}
    	
    	for(int i=0; i<4;i++,index++){
    		datas[index]=TypeUtils.int2Byte(data.getChecksum())[i];
    	}
    	for(int i=0;i<64 && i<data.getFrom().length;i++,index++){
    		datas[index]=data.getFrom()[i];
    	}
    	
    	for(int i=0;i<128 && i< data.getTopic().length;i++,index++){
    		datas[index]=data.getTopic()[i];
    	}
    	
    	for(int i=0;i<data.getDataLen();i++,index++){
    		if(data.getMsg()!=null)
    			datas[index]=data.getMsg()[i];
    	}
    	
    	return datas;
    	
    } */
}
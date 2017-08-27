package org.wmq.message;

import struct.StructClass;
import struct.StructField;


@StructClass
public class SocketData {
	
	@StructField(order = 0)
	private int version;
	
	@StructField(order = 1)
	private int code;
	
	@StructField(order = 2)
	private int cmd;
	
	@StructField(order = 3)
	private int  fd;
	
	@StructField(order = 4)
	private int dataLen;
	
	@StructField(order = 5)
	private int checksum;
	
	@StructField(order = 6)
	private byte topic[]=new byte[128];
	
	@StructField(order = 7)
	private byte from[]=new byte[64];
	
	@StructField(order = 8)
	private byte msg[];
	
	
	public SocketData(){
		
	}
	
	public int getVersion() {
		return version;
	}

	public void setVersion(int version) {
		this.version = version;
	}

	public int getCode() {
		return code;
	}

	public void setCode(int code) {
		this.code = code;
	}

	public int getCmd() {
		return cmd;
	}

	public void setCmd(int cmd) {
		this.cmd = cmd;
	}

	public int getFd() {
		return fd;
	}

	public void setFd(int fd) {
		this.fd = fd;
	}

	public int getDataLen() {
		return dataLen;
	}

	public void setDataLen(int dataLen) {
		this.dataLen = dataLen;
	}

	public int getChecksum() {
		return checksum;
	}

	public void setChecksum(int checksum) {
		this.checksum = checksum;
	}

	public byte[] getFrom() {
		return from;
	}

	public void setFrom(byte[] from) {
		for(int i=0;i<64 && i<from.length;i++){
			this.from[i] = from[i];
		}
		
	}

	public byte[] getTopic() {
		
		return topic;
	}

	public void setTopic(byte[] topic) {
		for(int i=0;i<128&& i<topic.length;i++){
			this.topic[i] = topic[i];
		}
		
	}

	public byte[] getMsg() {
		return msg;
	}

	public void setMsg(byte[] msg) {
		this.msg = msg;
	}


	
}

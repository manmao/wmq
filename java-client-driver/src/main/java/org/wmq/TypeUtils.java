package org.wmq;
import java.nio.ByteBuffer; 
import java.nio.FloatBuffer; 


/**
 * java 基本类型和 
 * @author Administrator
 *
 */
public class TypeUtils { 
 
    public static byte[] int2Byte(int l) { 
        byte[] b = new byte[4]; 
        for (int i = 0; i < b.length; i++) { 
            b[i] = new Integer(l).byteValue(); 
            l = l >> 8; 
        } 
        return b; 
    } 
 
    public static int byte2Int(byte[] b) { 
        int l = 0; 
        l = b[0]; 
        l &= 0xff; 
        l |= ((int) b[1] << 8); 
        l &= 0xffff; 
        l |= ((int) b[2] << 16); 
        l &= 0xffffff; 
        l |= ((int) b[3] << 24); 
        l &= 0xffffffff; 
        return l; 
    } 
 
 
    public static byte[] longToByte(long l) { 
        byte[] b = new byte[8]; 
        for (int i = 0; i < b.length; i++) { 
            b[i] = new Long(l).byteValue(); 
            l = l >> 8; 
        } 
        return b; 
    } 
 
    public static long byteToLong(byte[] b) { 
        long l = 0; 
        l |= (((long) b[7] & 0xff) << 56); 
        l |= (((long) b[6] & 0xff) << 48); 
        l |= (((long) b[5] & 0xff) << 40); 
        l |= (((long) b[4] & 0xff) << 32); 
        l |= (((long) b[3] & 0xff) << 24); 
        l |= (((long) b[2] & 0xff) << 16); 
        l |= (((long) b[1] & 0xff) << 8); 
        l |= ((long) b[0] & 0xff); 
        return l; 
    } 
 
 
    public static byte[] float2Byte(float f) { 
        byte[] b = new byte[4]; 
        int l = Float.floatToIntBits(f); 
        for (int i = 0; i < b.length; i++) { 
            b[i] = new Integer(l).byteValue(); 
            l = l >> 8; 
        } 
        return b; 
    } 
 
    public static float byte2Float(byte[] b) { 
        int l = 0; 
        l = b[0]; 
        l &= 0xff; 
        l |= ((int) b[1] << 8); 
        l &= 0xffff; 
        l |= ((int) b[2] << 16); 
        l &= 0xffffff; 
        l |= ((int) b[3] << 24); 
        l &= 0xffffffffl; 
        return Float.intBitsToFloat(l); 
    } 
 
    public static byte[] doubleToByte(double d) { 
        byte[] b = new byte[8]; 
        long l = Double.doubleToLongBits(d); 
        for (int i = 0; i < b.length; i++) { 
            b[i] = new Long(l).byteValue(); 
            l = l >> 8; 
        } 
        return b; 
    } 
 
 
 
    public static char[] bytesToChars(byte[] bytes,int offset, int count) { 
        char chars[] = new char[count]; 
        for(int i = 0;i< count;i++){ 
            chars[i] = (char)bytes[i]; 
        } 
        return chars; 
    } 
 
    public static byte[] charsToBytes(char[] chars,int offset,int count) { 
        byte bytes[] = new byte[count]; 
        for(int i = 0;i< count;i++){ 
            bytes[i] = (byte)chars[i]; 
        } 
        return bytes; 
    } 
 
    public static byte[] floatToByte(float v) { 
        ByteBuffer bb = ByteBuffer.allocate(4); 
        byte[] ret = new byte[4]; 
        FloatBuffer fb = bb.asFloatBuffer(); 
        fb.put(v); 
        bb.get(ret); 
        return ret; 
    } 
 
    public static float byteToFloat(byte[] v) { 
        ByteBuffer bb = ByteBuffer.wrap(v); 
        FloatBuffer fb = bb.asFloatBuffer(); 
        return fb.get(); 
    } 
    
    public static byte[] shortToByte(short number){ 
        int temp = number; 
        byte[] b =new byte[2]; 
        for(int i =0; i < b.length; i++){ 
            b[i]=new Integer(temp &0xff).byteValue();
            temp = temp >>8;// 向右移8位 
        } 
        return b; 
    } 
    
    public static short byteToShort(byte[] b){ 
        short s =0; 
        short s0 =(short)(b[0]&0xff);// 最低位 
        short s1 =(short)(b[1]&0xff); 
        s1 <<=8; 
        s =(short)(s0 | s1); 
        return s; 
    }
 
} 
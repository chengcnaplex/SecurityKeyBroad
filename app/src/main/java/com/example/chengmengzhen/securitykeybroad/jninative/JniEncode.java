package com.example.chengmengzhen.securitykeybroad.jninative;

/**
 * Created by chengmengzhen on 16/7/1.
 */
public class JniEncode {
    public native static String encryption(byte[] pwd);
    public native static byte[] decryption(byte[] pwd);
    static {
        System.loadLibrary("encryp");
    }
}

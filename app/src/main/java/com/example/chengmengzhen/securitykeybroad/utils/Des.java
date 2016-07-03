package com.example.chengmengzhen.securitykeybroad.utils;

import java.io.File;


import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.crypto.spec.IvParameterSpec;




/**
 * Created by chengmengzhen on 16/7/3.
 */
public class Des {
        /** 加密、解密key. */
        public static final String PASSWORD_CRYPT_KEY = "12345678";

        /** 加密算法,可用 DES,DESede,Blowfish. */
        private final static String ALGORITHM = "DES";
        /**j加密码填充模式另种1，"DES/ECB/NOPADDING" 2."DES/CBC/PKCS5Padding" */
        private final static String ENCIPHER_MODEL="DES/ECB/NOPADDING";


        /**将明文补位变成8的倍数位然后分别加密组合，返回加密串
         * @param sourceData
         * @return
         * @throws Exception
         * @author cdl
         * @date Mar 31, 20145:38:29 PM
         *
         */
//        public static byte[] encrypt8byte(String sourceData ) throws Exception{
//            int number=0;
//            int len =sourceData.length();
//            StringBuffer data=new StringBuffer();//补位后数据
//            data.append(sourceData);
//            int seamlen=8-len%8;//计算补多少位
//            len+=seamlen;//总位数
//            for(int i=0;i<seamlen;i++){//补位？
//                data.append("?");
//            }
//            System.out.println(data.toString());
//            number =len/8;//加密次数
//            System.out.println(len);
//            System.out.println(number);
//
//            System.out.println(data.substring(16, 24));
//            byte []bb=new byte[number*8];
//
//            for(int i=0;i<number;i++){//循环加密
//
//                byte[]aa=encrypt(data.substring(i*8, i*8+8));
//                for(int j=0;j<aa.length;j++){
//                    bb[(i*8)+j]=aa[j];
//                }
//                System.out.println("长度："+aa.length);
//                System.out.println(new String(aa));
//
//            }
//            FileUtils.writeByteArrayToFile(new File("f:/a.txt"), bb);
//            for(int j=0;j<bb.length;j++){
//                System.out.println(bb[j]);
//            }
//
//
//            System.out.println(number);
//            return bb;
//
//        }
        /**把8位明文加密
         * @param message
         * @param key
         * @return
         * @throws Exception
         * @author cdl
         * @date Apr 1, 20144:27:33 PM
         *
         */
//        public static byte[] encrypt(String message) throws Exception {
//            //Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");//有填充
//            Cipher cipher = Cipher.getInstance(ENCIPHER_MODEL);//不填充规定明文必须8位
//
//            DESKeySpec desKeySpec = new DESKeySpec(PASSWORD_CRYPT_KEY.getBytes());
//
//            SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(ALGORITHM);
//            SecretKey secretKey = keyFactory.generateSecret(desKeySpec);
//            //IvParameterSpec iv = new IvParameterSpec(PASSWORD_CRYPT_KEY.getBytes());//有填充
//            //cipher.init(Cipher.ENCRYPT_MODE, secretKey, iv);//有填充
//            cipher.init(Cipher.ENCRYPT_MODE, secretKey );
//            // BASE64Encoder b = new BASE64Encoder();
//            // System.out.println(b.encode(cipher.doFinal(message.getBytes())));
//            // FileUtils.writeByteArrayToFile(new File("f:/c.txt"), message.getBytes());
//            // FileUtils.writeByteArrayToFile(new File("f:/a.txt"), cipher.doFinal(message.getBytes()));
//            byte []ci=cipher.doFinal(message.getBytes());
//            for(int i=0;i<ci.length;i++){//加密的密文按字节打印出来 为了与c加密的密文核对
//                System.out.print(ci[i]+"  ");
//            }
//            return cipher.doFinal(message.getBytes());
//            // return bytesToHex(cipher.doFinal(message.getBytes()));
//        }

        /**将8位密文解密
         * @param message
         * @param key
         * @return
         * @throws Exception
         * @author cdl
         * @date Apr 1, 20144:40:11 PM
         *
         */
        public static byte[] decrypt(String message,String key) throws Exception {
            //Cipher cipher = Cipher.getInstance("DES/CBC/PKCS5Padding");//有填充
            Cipher cipher = Cipher.getInstance(ENCIPHER_MODEL);
            DESKeySpec desKeySpec = new DESKeySpec(PASSWORD_CRYPT_KEY.getBytes());
            SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(ALGORITHM);
            SecretKey secretKey = keyFactory.generateSecret(desKeySpec);
            //IvParameterSpec iv = new IvParameterSpec(PASSWORD_CRYPT_KEY.getBytes());//有填充

            //cipher.init(Cipher.DECRYPT_MODE, secretKey, iv);//有填充
            cipher.init(Cipher.DECRYPT_MODE, secretKey );

            return cipher.doFinal(message.getBytes());

        }


//        public static void main(String[] args) {
//            try {
//                //加密返回加密串
//                byte[] aa= encrypt8byte("qwertyuioasdfghjkl");
//                System.out.println(new String(aa));
//                System.out.println(decrypt("d6364ffa354ae641"));
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
}
